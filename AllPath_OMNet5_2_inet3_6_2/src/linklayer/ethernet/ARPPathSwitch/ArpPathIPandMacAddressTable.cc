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

#include "ArpPathIPandMacAddressTable.h"

using namespace inet;

namespace allpath {
ArpPathIPandMacAddressTable::ArpPathIPandMacAddressTable() {
    // TODO Auto-generated constructor stub
       lastPurge = SIMTIME_ZERO;
       arpPathTable = new ArpPathTable();
       // Set addressTable for VLAN ID 0
       vlanArpPathTable[0] = arpPathTable;
//below lines is used when we use a file for MAC Addresses
      // ArpPathTable& arpPathTable = *this->arpPathTable;    // magic to hide the '*' from the name of the watch below
      // WATCH_MAP(arpPathTable);

}

ArpPathIPandMacAddressTable::ArpPathIPandMacAddressTable(simtime_t agingTime):ArpPathIPandMacAddressTable(){
    this->agingTime = agingTime;

}
ArpPathIPandMacAddressTable::~ArpPathIPandMacAddressTable()
{
    for (auto & elem : vlanArpPathTable)
        delete elem.second;
}

void ArpPathIPandMacAddressTable::setAgingTime(simtime_t agingTime){
    this->agingTime=agingTime;

}

/*
 * getTableForVid
 * Returns a MAC Address Table for a specified VLAN ID
 * or nullptr pointer if it is not found
 */

ArpPathIPandMacAddressTable::ArpPathTable *ArpPathIPandMacAddressTable::getTableForVid(unsigned int vid)
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

int ArpPathIPandMacAddressTable::getPortForAddress(MACAddress address1,IPv4Address address2, unsigned int vid)
{
    //Enter_Method("ArpPathMacAddressTable::getPortForAddress()");

    ArpPathTable *table = getTableForVid(vid);
    // VLAN ID vid does not exist
    if (table == nullptr)
        return -1;

    auto iter = table->find(FlowAddress(address1,address2));

    if (iter == table->end()) {
        // not found
        return -1;
    }
    if (iter->second.insertionTime + agingTime <= simTime()) {
        // don't use (and throw out) aged entries
        EV << "Ignoring and deleting aged entry: " << iter->first.first <<" (IPAddress:"<<iter->first.second <<") " << " --> port" << iter->second.portno << "\n";
        table->erase(iter);
        return -1;
    }
    return iter->second.portno;
}

/*
 * Register a new MAC address at addressTable.
 * True if refreshed. False if it is new.
 */


bool ArpPathIPandMacAddressTable::updateTableWithAddress(int portno, MACAddress& address1, IPv4Address address2, unsigned int vid)
{
   // Enter_Method("ArpPathMacAddressTable::updateTableWithAddress()");
    if (address1.isBroadcast())
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
        iter = table->find(FlowAddress(address1, address2));

    if (iter == table->end()) {
        removeAgedEntriesIfNeeded();

        // Add entry to table
        EV << "-----Hedayat10----Adding entry to Address Table: IP--> "<<address2 <<" MAC--> " << address1 << " --> port" << portno << "\n";
        (*table)[FlowAddress(address1, address2)] = TableEntry(vid, portno, simTime());
        return false;
    }
    else {
        // Update existing entry
        EV << "----Hedayat10---Updating entry in Address Table: IP--> "<<address2 <<" MAC--> " << address1 << " --> port" << portno << "\n";
        TableEntry& entry = iter->second;
        entry.insertionTime = simTime();
        entry.portno = portno;
    }
    return true;
}


/*
 * Clears portno MAC cache.
 */

void ArpPathIPandMacAddressTable::flush(int portno)
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

void ArpPathIPandMacAddressTable::printState()
{
    EV << endl << "Arp Path MAC Address Table" << endl;
    EV << "VLAN ID    MAC    Port    Inserted" << endl;
    for (auto & elem : vlanArpPathTable) {     //s.h.h: for all elements of vlanArpPathTable array. each element is a ArpPathTable pointer(note that ArpPathTable is a array too). so each_element->second=is_a_TableEntry(equivalent to vlanArpPathTable[i]->second=is_a_TableEntry).
        ArpPathTable *table = elem.second;
        for (auto & table_j : *table)
            EV << table_j.second.vid << "   " << table_j.first.first << "   " << table_j.first.second << "   " << table_j.second.portno << "   " << table_j.second.insertionTime << endl;
    }
}

void ArpPathIPandMacAddressTable::copyTable(int portA, int portB)
{
    for (auto & elem : vlanArpPathTable) {
        ArpPathTable *table = elem.second;
        for (auto & table_j : *table)
            if (table_j.second.portno == portA)
                table_j.second.portno = portB;

    }
}

void ArpPathIPandMacAddressTable::removeAgedEntriesFromVlan(unsigned int vid)
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
               << cur->first.first <<" (IP Address: " << cur->first.second << ") " << " --> port" << cur->second.portno << "\n";
            table->erase(cur);
        }
    }
}

void ArpPathIPandMacAddressTable::removeAgedEntriesFromAllVlans()
{
    for (auto & elem : vlanArpPathTable) {
        ArpPathTable *table = elem.second;
        // TODO: this part could be factored out
        for (auto j = table->begin(); j != table->end(); ) {
            auto cur = j++;    // iter will get invalidated after erase()
            TableEntry& entry = cur->second;
            if (entry.insertionTime + agingTime <= simTime()) {
                EV << "Removing aged entry from Address Table: "
                   << cur->first.first << "( IP Address: "<< cur->first.second<<") " << " --> port" << cur->second.portno << "\n";
                table->erase(cur);
            }
        }
    }
}

void ArpPathIPandMacAddressTable::removeAgedEntriesIfNeeded()
{
    simtime_t now = simTime();

    if (now >= lastPurge + 1)
        removeAgedEntriesFromAllVlans();

    lastPurge = simTime();
}


void ArpPathIPandMacAddressTable::clearTable()
{
    for (auto & elem : vlanArpPathTable)
        delete elem.second;

    vlanArpPathTable.clear();
    arpPathTable = nullptr;
}

bool ArpPathIPandMacAddressTable::isPortintable(int portno, unsigned int vid)
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


int ArpPathIPandMacAddressTable::getnumbersOfAddressesAssociated(int portno, unsigned int vid)
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

std::pair<MACAddress,IPv4Address> *ArpPathIPandMacAddressTable::getAddressesAssociatedToPort(int portno,unsigned int vid)
{
    FlowAddress *adr=new FlowAddress[getnumbersOfAddressesAssociated(portno)];
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

void ArpPathIPandMacAddressTable::removeFlowAddressFromVlan(std::pair<MACAddress,IPv4Address> adr, unsigned int vid)
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

