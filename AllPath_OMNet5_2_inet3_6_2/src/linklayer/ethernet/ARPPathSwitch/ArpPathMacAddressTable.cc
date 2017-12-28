/*
 * ArpPathMactableTable.cc
 *
 *  Created on: ?? ÇÑÏ?ÈåÔÊ ???? åž.Ô.
 *      Author: Simject
 */

#include "./ArpPathMacAddressTable.h"
using namespace inet;

namespace allpath {
ArpPathMacAddressTable::ArpPathMacAddressTable() {
    // TODO Auto-generated constructor stub
       lastPurge = SIMTIME_ZERO;
       arpPathTable = new ArpPathTable();
       // Set addressTable for VLAN ID 0
       vlanArpPathTable[0] = arpPathTable;
//below lines is used when we use a file for MAC Addresses
      // ArpPathTable& arpPathTable = *this->arpPathTable;    // magic to hide the '*' from the name of the watch below
      // WATCH_MAP(arpPathTable);

}

ArpPathMacAddressTable::ArpPathMacAddressTable(simtime_t agingTime):ArpPathMacAddressTable(){
    this->agingTime = agingTime;

}
ArpPathMacAddressTable::~ArpPathMacAddressTable()
{
    for (auto & elem : vlanArpPathTable)
        delete elem.second;
}

void ArpPathMacAddressTable::setAgingTime(simtime_t agingTime){
    this->agingTime=agingTime;

}

/*
 * getTableForVid
 * Returns a MAC Address Table for a specified VLAN ID
 * or nullptr pointer if it is not found
 */

ArpPathMacAddressTable::ArpPathTable *ArpPathMacAddressTable::getTableForVid(unsigned int vid)
{
    if (vid == 0)
        return arpPathTable;

    auto iter = vlanArpPathTable .find(vid);
    if (iter != vlanArpPathTable.end())
        return iter->second;
    return nullptr;
}

/*
 * For a known arriving port, V-TAG and destination MAC. It generates a vector with the ports where relay component
 * should deliver the message.
 * returns false if not found
 */

int ArpPathMacAddressTable::getPortForAddress(MACAddress address, unsigned int vid)
{
    //Enter_Method("ArpPathMacAddressTable::getPortForAddress()");

    ArpPathTable *table = getTableForVid(vid);
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

/*
 * Register a new MAC address at addressTable.
 * True if refreshed. False if it is new.
 */

bool ArpPathMacAddressTable::updateTableWithAddress(int portno, MACAddress& address, unsigned int vid)
{
   // Enter_Method("ArpPathMacAddressTable::updateTableWithAddress()");
    if (address.isBroadcast())
        return false;

    ArpPathTable::iterator iter;
    ArpPathTable *table = getTableForVid(vid);

    if (table == nullptr) {
        // MAC Address Table does not exist for VLAN ID vid, so we create it
        table = new ArpPathTable();

        // set 'the addressTable' to VLAN ID 0
        if (vid == 0)
            arpPathTable = table;

        vlanArpPathTable[vid] = table;
        iter = table->end();
    }
    else
        iter = table->find(address);

    if (iter == table->end()) {
        removeAgedEntriesIfNeeded();

        // Add entry to table
        EV << "-----Hedayat0----Adding entry to Address Table: " << address << " --> port" << portno << "\n";
        (*table)[address] = TableEntry(vid, portno, simTime());
        return false;
    }
    else {
        // Update existing entry
        EV << "----Hedayat0---Updating entry in Address Table: " << address << " --> port" << portno << "\n";
        TableEntry& entry = iter->second;
        entry.insertionTime = simTime();
        entry.portno = portno;
    }
    return true;
}

/*
 * Clears portno MAC cache.
 */

void ArpPathMacAddressTable::flush(int portno)
{
 //   Enter_Method("ArpPathMacAddressTable::flush():  Clearing gate %d cache", portno);
    for (auto & elem : vlanArpPathTable) {
        ArpPathTable *table = elem.second;
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

void ArpPathMacAddressTable::printState()
{
    EV << endl << "Arp Path MAC Address Table" << endl;
    EV << "VLAN ID    MAC    Port    Inserted" << endl;
    for (auto & elem : vlanArpPathTable) {     //s.h.h: for all elements of vlanArpPathTable array. each element is a ArpPathTable pointer(note that ArpPathTable is a array too). so each_element->second=is_a_TableEntry(equivalent to vlanArpPathTable[i]->second=is_a_TableEntry).
        ArpPathTable *table = elem.second;
        for (auto & table_j : *table)
            EV << table_j.second.vid << "   " << table_j.first << "   " << table_j.second.portno << "   " << table_j.second.insertionTime << endl;
    }
}

void ArpPathMacAddressTable::copyTable(int portA, int portB)
{
    for (auto & elem : vlanArpPathTable) {
        ArpPathTable *table = elem.second;
        for (auto & table_j : *table)
            if (table_j.second.portno == portA)
                table_j.second.portno = portB;

    }
}

void ArpPathMacAddressTable::removeAgedEntriesFromVlan(unsigned int vid)
{
    ArpPathTable *table = getTableForVid(vid);
    if (table == nullptr)
        return;
    // TODO: this part could be factored out
    for (auto iter = table->begin(); iter != table->end(); ) {
        auto cur = iter++;    // iter will get invalidated after erase()
        TableEntry& entry = cur->second;
        if (entry.insertionTime + agingTime <= simTime()) {
            EV << "Removing aged entry from ARP Path Address Table: "
               << cur->first << " --> port" << cur->second.portno << "\n";
            table->erase(cur);
        }
    }
}

void ArpPathMacAddressTable::removeAgedEntriesFromAllVlans()
{
    for (auto & elem : vlanArpPathTable) {
        ArpPathTable *table = elem.second;
        // TODO: this part could be factored out
        for (auto j = table->begin(); j != table->end(); ) {
            auto cur = j++;    // iter will get invalidated after erase()
            TableEntry& entry = cur->second;
            if (entry.insertionTime + agingTime <= simTime()) {
                EV << "Removing aged entry from Address Table: "
                   << cur->first << " --> port" << cur->second.portno << "\n";
                table->erase(cur);
            }
        }
    }
}

void ArpPathMacAddressTable::removeAgedEntriesIfNeeded()
{
    simtime_t now = simTime();

    if (now >= lastPurge + 1)
        removeAgedEntriesFromAllVlans();

    lastPurge = simTime();
}


void ArpPathMacAddressTable::clearTable()
{
    for (auto & elem : vlanArpPathTable)
        delete elem.second;

    vlanArpPathTable.clear();
    arpPathTable = nullptr;
}

bool ArpPathMacAddressTable::isPortintable(int portno, unsigned int vid)
{
    ArpPathTable *table = getTableForVid(vid);
        if (table == nullptr)
            return false;

        for (auto iter = table->begin(); iter != table->end();iter++ ) {

            TableEntry& entry = iter->second;
            if (entry.portno == portno) {
               return true;
            }
        }
        return false;
}


int ArpPathMacAddressTable::getnumbersOfAddressesAssociated(int portno, unsigned int vid)
{
    int numbers = 0;
    ArpPathTable *table = getTableForVid(vid);
        if (table == nullptr)
            return -1;

        for (auto iter = table->begin(); iter != table->end();iter++ )
        {

            TableEntry& entry = iter->second;
            if (entry.portno == portno)
            {
                numbers++;
            }
        }
        return numbers;
}

    /*
     * index=1 : first mac address that is associated to the port
     * index=2 : second mac address that is associated to the port
     */

MACAddress *ArpPathMacAddressTable::getAddressesAssociatedToPort(int portno,unsigned int vid)
{
    MACAddress *adr=new MACAddress[getnumbersOfAddressesAssociated(portno)];
    int index = 0;
    ArpPathTable *table = getTableForVid(vid);
    for (auto iter = table->begin(); iter != table->end();iter++ )
        {

            TableEntry& entry = iter->second;
            if (entry.portno == portno)  //if (entry.portno == portno)&&(++thisindex==index)
            {
                adr[index++]=iter->first;
            }
        }
    return adr;
}

void ArpPathMacAddressTable::removeMACAddressFromVlan(MACAddress adr, unsigned int vid)
{
    ArpPathTable *table = getTableForVid(vid);
    // VLAN ID vid does not exist
    if (table == nullptr)
        return ;

    auto iter = table->find(adr);

    if (iter == table->end()) {
        // not found
        return ;
    }
    if (iter->first == adr) {
        // don't use (and throw out) aged entries
        table->erase(iter);
        return ;
    }
    return ;
}

}
