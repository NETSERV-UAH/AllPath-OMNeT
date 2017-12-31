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

#include "BlockingTableWAPB.h"
#include "src/linklayer/ethernet/ARPPathSwitch/MACRelayUnitWAPB.h"
#include <map>

using namespace inet;
namespace wapb {

#define MAX_LINE    100

Define_Module(BlockingTableWAPB);

/** BT(Blocking/Broadcast Table) **/

std::ostream& operator<<(std::ostream& os, const BlockingTableWAPB::AddressEntryBasic& entry)
{
    os << "{Port=" << entry.port << ", Next-Hop=" << entry.next_hop << ", Timer=" << entry.inTime <<"}";
    return os;
}


void BlockingTableWAPB::initialize()
{
    blockingTime = par("blockingTime");

    WATCH_MAP(addressTableBasic);
}

void BlockingTableWAPB::handleMessage(cMessage *)
{
    throw cRuntimeError("This module doesn't process messages");
}

//method to update an entry in BT
bool BlockingTableWAPB::updateTableBasic(MACAddress address, AddressEntryBasic entryBasic)
{
    /*
    AddressTableBasic::iterator iter = addressTableBasic.find(address);
    bool found = false;
    if (iter != addressTable.end())
    {
        entryBasic.inTime = iter->second.inTime;
        entryBasic.port = iter->second.port;
        found = true;
    }
    return found;
    */
    addressTableBasic[address] = entryBasic;  // for both updateTableBasic & updateTableBasicw
    return true;
}

//Method to remove obsolete entries from BT (Blocking/Broadcast Table)
void BlockingTableWAPB::updateAndRemoveAgedEntriesFromBT()
{
    EV << "    BT (" << this->getParentModule()->getFullName() << ") (" << addressTableBasic.size() << " entries):\n";
    EV << "      ------------------------------------------------" << endl;
    EV << "      | Address                     | Next-Hop                   | Timer |" << endl;
    EV << "      ------------------------------------------------" << endl;

    for (AddressTableBasic::iterator iter = addressTableBasic.begin(); iter != addressTableBasic.end();)
    {
        AddressTableBasic::iterator cur = iter++; // iter will get invalidated after erase()
        AddressEntryBasic& entry = cur->second;

        EV << "      | " << cur->first << " |    " << entry.next_hop << "    | ";
        float time = simTime().dbl()-entry.inTime.dbl();
        char buffer[20];
        sprintf(buffer, "%.03f |", time);
        EV << buffer; //Print timer just with 3 decimals

//new change. this must be cheched
   /*     MACRelayUnitWAPB * pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getSubmodule("relayUnit"));
        if(pRelayUnitWAPB->linkDownForPort(entry.port))  //if(linkDownForPort(entry.port))
        {
            EV << " -> Removing entry from BT (link is down!)";
            addressTableBasic.erase(cur); //erase does not return the following iterator
        }
        else if (entry.inTime + blockingTime <= simTime())
        {
            EV << "  -> Removing aged entry from BT with time: " << simTime()-entry.inTime << " (over " << blockingTime << ")";
            addressTableBasic.erase(cur);
        }
        EV << endl;  */
    }
}

void BlockingTableWAPB::addEntryBasicw(AddressEntryBasic entry, MACAddress address, MACAddress next_h)
{
    entry.inTime = simTime();
    entry.next_hop = next_h;
    addressTableBasic[address] = entry;
}

//Method to add an entry in the special broadcast address table, given its MAC address and inputport
void BlockingTableWAPB::addEntryBasic(AddressEntryBasic entry, MACAddress address, int inputport)
{
    entry.inTime = simTime();
    entry.port = inputport;
    addressTableBasic[address] = entry;
}


//Method to find an entry in the special broadcast address table, given its MAC address.
bool BlockingTableWAPB::findEntryBasic(MACAddress address, AddressEntryBasic& entryBasic)
{
    bool found = false;
    AddressTableBasic::iterator iter = addressTableBasic.find(address);

    if (iter != addressTableBasic.end())
    {
        entryBasic.inTime = iter->second.inTime;
        entryBasic.port = iter->second.port;
        found = true;
    }
    else
    {
        entryBasic.port = -1;
    }
    return found;
}

//Method to find an entry in the special broadcast address table, given its MAC address.
bool BlockingTableWAPB::findEntryBasicw(MACAddress address, AddressEntryBasic& entryBasic)
{
    bool found = false;
    AddressTableBasic::iterator iter = addressTableBasic.find(address);

    if (iter != addressTableBasic.end())
    {
        entryBasic.inTime = iter->second.inTime;
        entryBasic.next_hop = iter->second.next_hop;
        found = true;
    }
    else
    {
        entryBasic.next_hop = entryBasic.next_hop.UNSPECIFIED_ADDRESS;
    }
    return found;
}

void BlockingTableWAPB::clearBT()
{
    addressTableBasic.clear();
}

//Method to print the contents of BT
void BlockingTableWAPB::printBT()
{
    EV << endl;
    EV << "    BT (" << this->getParentModule()->getFullName() << ") (" << addressTableBasic.size() << " entries):\n";
    EV << "      ------------------------------------------------" << endl;
    EV << "      | Address                     | Port | Timer |" << endl;
    EV << "      ------------------------------------------------" << endl;

    for (AddressTableBasic::iterator iter = addressTableBasic.begin(); iter != addressTableBasic.end(); iter++)
    {
        AddressEntryBasic& entry = iter->second;

        EV << "      | " << iter->first << " |    " << entry.port << "    | ";
                float time = simTime().dbl()-entry.inTime.dbl();
                char buffer[20];
                sprintf(buffer, "%.03f |", time);
                EV << buffer << endl; //Print timer just with 3 decimals
    }
}

} // namespace wapb

