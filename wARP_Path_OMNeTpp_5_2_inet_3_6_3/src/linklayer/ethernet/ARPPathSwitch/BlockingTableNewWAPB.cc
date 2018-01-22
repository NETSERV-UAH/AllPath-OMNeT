// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include <map>

#include "src/linklayer/ethernet/ARPPathswitch/BlockingTableNewWAPB.h"

namespace wapb {
using namespace inet;

#define MAX_LINE    100

Define_Module(BlockingTableNewWAPB);

std::ostream& operator<<(std::ostream& os, const BlockingTableNewWAPB::AddressEntry& entry)
{
    os << "{VID=" << entry.vid << ", port=" << entry.portno << ", insertionTime=" << entry.insertionTime << ", portToSend=" << entry.portToSend << ", status=" << entry.status << ", ip=" << entry.ip << ", next_hop=" << entry.next_hop << "}";
    return os;
}

BlockingTableNewWAPB::BlockingTableNewWAPB()
{
    addressTable = new AddressTable();
    // Set addressTable for VLAN ID 0
    vlanAddressTable[0] = addressTable;
}

void BlockingTableNewWAPB::initialize()
{
    agingTime = par("blockingTime");
    lastPurge = SIMTIME_ZERO;

    AddressTable& addressTable = *this->addressTable;    // magic to hide the '*' from the name of the watch below
    WATCH_MAP(addressTable);
}

/**
 * Function reads from a file stream pointed to by 'fp' and stores characters
 * until the '\n' or EOF character is found, the resultant string is returned.
 * Note that neither '\n' nor EOF character is stored to the resultant string,
 * also note that if on a line containing useful data that EOF occurs, then
 * that line will not be read in, hence must terminate file with unused line.
 */
static char *fgetline(FILE *fp)
{
    // alloc buffer and read a line
    char *line = new char[MAX_LINE];
    if (fgets(line, MAX_LINE, fp) == nullptr) {
        delete[] line;
        return nullptr;
    }

    // chop CR/LF
    line[MAX_LINE - 1] = '\0';
    int len = strlen(line);
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
        line[--len] = '\0';

    return line;
}

void BlockingTableNewWAPB::handleMessage(cMessage *)
{
    throw cRuntimeError("This module doesn't process messages");
}

/*
 * getTableForVid
 * Returns a MAC Address Table for a specified VLAN ID
 * or nullptr pointer if it is not found
 */

BlockingTableNewWAPB::AddressTable *BlockingTableNewWAPB::getTableForVid(unsigned int vid)
{
    if (vid == 0)
        return addressTable;

    auto iter = vlanAddressTable.find(vid);
    if (iter != vlanAddressTable.end())
        return iter->second;
    return nullptr;
}

/*
 * For a known arriving port, V-TAG and destination MAC. It generates a vector with the ports where relay component
 * should deliver the message.
 * returns false if not found
 */

int BlockingTableNewWAPB::getPortForAddress(MACAddress& address, unsigned int vid)
{
    Enter_Method("BlockingTableNewWAPB::getPortForAddress()");

    AddressTable *table = getTableForVid(vid);
    // VLAN ID vid does not exist
    if (table == nullptr)
        return -1;

    auto iter = table->find(address);

    if (iter == table->end()) {
        // not found
        return -1;
    }
    if (iter->second.insertionTime + agingTime <= simTime()) {
        // don't use (and throw out) aged entries
        EV << "Ignoring and deleting aged entry: " << iter->first << " --> port" << iter->second.portno << "\n";
        table->erase(iter);
        return -1;
    }
    return iter->second.portno;
}

// used for wireless ARP-Path
MACAddress BlockingTableNewWAPB::getNextHopForAddress(MACAddress address, unsigned int vid)
{
    Enter_Method("BlockingTableNewWAPB::getPortForAddress()");

    AddressTable *table = getTableForVid(vid);
    // VLAN ID vid does not exist
    if (table == nullptr)
        return MACAddress::UNSPECIFIED_ADDRESS;

    auto iter = table->find(address);

    if (iter == table->end()) {
        // not found
        return MACAddress::UNSPECIFIED_ADDRESS;
    }
    if (iter->second.insertionTime + agingTime <= simTime()) {
        // don't use (and throw out) aged entries
        EV << "Ignoring and deleting aged entry: " << iter->first << " --> next hop " << iter->second.next_hop << "\n";
        table->erase(iter);
        return MACAddress::UNSPECIFIED_ADDRESS;
    }
    return iter->second.next_hop;
}

/*
 * Register a new MAC address at addressTable.
 * True if refreshed. False if it is new.
 */

bool BlockingTableNewWAPB::updateTableWithAddress(int portno, MACAddress& address, int status, unsigned int vid)
{
    Enter_Method("BlockingTableNewWAPB::updateTableWithAddress()");
    if (address.isBroadcast())
        return false;

    AddressTable::iterator iter;
    AddressTable *table = getTableForVid(vid);

    if (table == nullptr) {
        // MAC Address Table does not exist for VLAN ID vid, so we create it
        table = new AddressTable();

        // set 'the addressTable' to VLAN ID 0
        if (vid == 0)
            addressTable = table;

        vlanAddressTable[vid] = table;
        iter = table->end();
    }
    else
        iter = table->find(address);

    if (iter == table->end()) {
        removeAgedEntriesIfNeeded();

        // Add entry to table
        EV << "Adding entry to Address Table: " << address << " --> port" << portno << " --> status  " << status<< "\n";
        (*table)[address] = AddressEntry(vid, portno, -1, simTime(), status, IPv4Address::UNSPECIFIED_ADDRESS, MACAddress::UNSPECIFIED_ADDRESS);

        return false;
    }
    else {
        // Update existing entry
        EV << "Updating entry in Address Table: " << address << " --> port" << portno << "\n";
        AddressEntry& entry = iter->second;
        entry.insertionTime = simTime();
        entry.portno = portno;
        entry.status = status;
    }
    return true;
}

//used for wireless ARP-Path
bool BlockingTableNewWAPB::updateTableWithAddress(MACAddress nextHop, MACAddress& address, int status, unsigned int vid)
{
    Enter_Method("BlockingTableNewWAPB::updateTableWithAddress()");
    if (address.isBroadcast())
        return false;

    AddressTable::iterator iter;
    AddressTable *table = getTableForVid(vid);

    if (table == nullptr) {
        // MAC Address Table does not exist for VLAN ID vid, so we create it
        table = new AddressTable();

        // set 'the addressTable' to VLAN ID 0
        if (vid == 0)
            addressTable = table;

        vlanAddressTable[vid] = table;
        iter = table->end();
    }
    else
        iter = table->find(address);

    if (iter == table->end()) {
        removeAgedEntriesIfNeeded();

        // Add entry to table
        EV << "Adding entry to Address Table: " << address << " --> next hop " << nextHop << " --> status  " << status<< "\n";
        (*table)[address] = AddressEntry(vid, -1, -1, simTime(), status, IPv4Address::UNSPECIFIED_ADDRESS, nextHop);
        return false;
    }
    else {
        // Update existing entry
        EV << "Updating entry in Address Table: " << address << " --> next hop " << nextHop << " --> status  " << status<< "\n";
        AddressEntry& entry = iter->second;
        entry.insertionTime = simTime();
        entry.next_hop = nextHop;
        entry.status = status;
    }
    return true;
}

/*
 * Clears portno MAC cache.
 */

void BlockingTableNewWAPB::flush(int portno)
{
    Enter_Method("BlockingTableNewWAPB::flush():  Clearing gate %d cache", portno);
    for (auto & elem : vlanAddressTable) {
        AddressTable *table = elem.second;
        for (auto j = table->begin(); j != table->end(); ) {
            auto cur = j++;
            if (cur->second.portno == portno)
                table->erase(cur);
        }
    }
}

/*
 * Prints verbose information
 */

void BlockingTableNewWAPB::printState()
{
    EV << endl << "MAC Address Table" << endl;
    EV << "VLAN ID    MAC    Port    Port to Send    Inserted      Status     IP    Next Hop" << endl;
    for (auto & elem : vlanAddressTable) {
        AddressTable *table = elem.second;
        for (auto & table_j : *table)
            EV << table_j.second.vid << "   " << table_j.first << "   " << table_j.second.portno << "   " << table_j.second.portToSend << "   " << table_j.second.insertionTime << "   " << table_j.second.status << "   " << table_j.second.ip << table_j.second.next_hop << endl;
    }
}

void BlockingTableNewWAPB::copyTable(int portA, int portB)
{
    for (auto & elem : vlanAddressTable) {
        AddressTable *table = elem.second;
        for (auto & table_j : *table)
            if (table_j.second.portno == portA)
                table_j.second.portno = portB;

    }
}

void BlockingTableNewWAPB::removeAgedEntriesFromVlan(unsigned int vid)
{
    AddressTable *table = getTableForVid(vid);
    if (table == nullptr)
        return;
    // TODO: this part could be factored out
    for (auto iter = table->begin(); iter != table->end(); ) {
        auto cur = iter++;    // iter will get invalidated after erase()
        AddressEntry& entry = cur->second;
        if (entry.insertionTime + agingTime <= simTime()) {
            EV << "Removing aged entry from Address Table: "
               << cur->first << " --> port" << cur->second.portno << "\n";
            table->erase(cur);
        }
    }
}

void BlockingTableNewWAPB::removeAgedEntriesFromAllVlans()
{
    for (auto & elem : vlanAddressTable) {
        AddressTable *table = elem.second;
        // TODO: this part could be factored out
        for (auto j = table->begin(); j != table->end(); ) {
            auto cur = j++;    // iter will get invalidated after erase()
            AddressEntry& entry = cur->second;
            if (entry.insertionTime + agingTime <= simTime()) {
                EV << "Removing aged entry from Address Table: "
                   << cur->first << " --> port" << cur->second.portno << "\n";
                table->erase(cur);
            }
        }
    }
}

void BlockingTableNewWAPB::removeAgedEntriesIfNeeded()
{
    simtime_t now = simTime();

    if (now >= lastPurge + 1)
        removeAgedEntriesFromAllVlans();

    lastPurge = simTime();
}

void BlockingTableNewWAPB::clearTable()
{
    for (auto & elem : vlanAddressTable)
        delete elem.second;

    vlanAddressTable.clear();
    addressTable = nullptr;
}

BlockingTableNewWAPB::~BlockingTableNewWAPB()
{
    for (auto & elem : vlanAddressTable)
        delete elem.second;
}

void BlockingTableNewWAPB::setAgingTime(simtime_t agingTime)
{
    this->agingTime = agingTime;
}

void BlockingTableNewWAPB::resetDefaultAging()
{
    agingTime = par("blockingTime");
}

} // namespace wapb

