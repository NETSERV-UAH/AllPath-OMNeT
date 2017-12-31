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

#include "LearningTableWAPB.h"

#include <map>

using namespace inet;
namespace wapb {

#define MAX_LINE    100

Define_Module(LearningTableWAPB);


std::ostream& operator<<(std::ostream& os, const LearningTableWAPB::AddressEntry& entry)
{
    os << "{Port=" << entry.port <<"Port to send="<<entry.portToSend<< ", Next-Hop=" << entry.next_hop << ", Timer=" << entry.inTime <<", Status="<<entry.status<<", IP="<<entry.ip<<"}";
    return os;
}


void LearningTableWAPB::initialize()
{
    agingTime = par("agingTime");

    WATCH_MAP(addressTable);
}


void LearningTableWAPB::handleMessage(cMessage *)
{
    throw cRuntimeError("This module doesn't process messages");
}

//method to update an entry in LT
bool LearningTableWAPB::updateTable(MACAddress address, AddressEntry entry)
{
    /*
    AddressTable::iterator iter = addressTable.find(address);
    bool found = false;
    if (iter != addressTable.end())
    {
        iter->second.inTime = entry.inTime;
        iter->second.ip = entry.ip;
        iter->second.port = entry.port;
        iter->second.portToSend = entry.portToSend;
        iter->second.status = entry.status;
        found = true;
    }
    return found;
    */
    addressTable[address] = entry;   // for both updateTable & updateTablew
    return true;
}


bool LearningTableWAPB::isPortInTable(int port) //(unsigned int port)
{
    bool found = false;
    for (AddressTable::iterator iter = addressTable.begin(); iter!=addressTable.end(); iter++)
        if(iter->second.port == port)
            found = true;
    return found;

}

int LearningTableWAPB::getPortByAddress(MACAddress address)
{

    AddressTable::iterator iter = addressTable.find(address);

    if (iter != addressTable.end())
        return iter->second.port;
    else
        return -1;

}

void LearningTableWAPB::getTableMapping(std::map<MACAddress, int>& mapping)
{
    AddressTable::iterator iter;

    for (iter = addressTable.begin(); iter!=addressTable.end(); ++iter)
    {
        mapping[iter->first] = iter->second.port;

    }
}

//Method to remove obsolete entries from LT (Lookup/Learning Table)
//and it will also change entry status from 'locked' to 'learnt' when the blocking timer expires
void LearningTableWAPB::updateAndRemoveAgedEntriesFromLT()
{
    EV << "    LT (" << this->getParentModule()->getFullName() << ") (" << addressTable.size() << " entries):\n";
    EV << "      ----------------------------------------------------------------------------------" << endl;
    EV << "      | Address                     | Next-Hop                   | Timer | Status  | IP                  |" << endl;
    EV << "      ----------------------------------------------------------------------------------" << endl;

    for (AddressTable::iterator iter = addressTable.begin(); iter != addressTable.end();)
    {
        AddressTable::iterator cur = iter++; // iter will get invalidated after erase()
        AddressEntry& entry = cur->second;

        EV << "      | " << cur->first << " |    " << entry.next_hop << "|";// [ " << entry.portToSend << " ] | ";
                float time = simTime().dbl()-entry.inTime.dbl();
                char buffer[20];
                sprintf(buffer, "%.03f | ", time);
                EV << buffer; //Print timer just with 3 decimals
                if(entry.status == locked) EV << "locked | ";
                else EV << "learnt | ";
                EV << entry.ip << "    |";

  /*      if(linkDownForPort(entry.port))
        {
            EV << " -> Removing entry from LT (link is down!)";
            addressTable.erase(cur); //erase does not return the following iterator
        }
        else if (entry.inTime + agingTime <= simTime()) //First we remove any aged entry
        {
            EV << " -> Removing aged learnt entry from LT with time: " << simTime()-entry.inTime << " (over " << agingTime << ")";
            addressTable.erase(cur); //erase does not return the following iterator
        }
        else if (entry.status == locked && entry.inTime + blockingTime <= simTime()) //Later, we check if some 'locked' entry (still not completely aged) needs to update to 'learnt' status
        {
            EV << " -> Updating locked entry to learnt status with time: " << simTime()-entry.inTime << " (over " << blockingTime << ")";
            addressTable[cur->first].status = learnt;
            if(protocolVersion == 3)
            {
                EV << " -> Updating portToSend! (current: " << entry.port << " previous: " << entry.portToSend << ")";
                addressTable[cur->first].portToSend = addressTable[cur->first].port;
            }
        }
        EV << endl;  */
    }
}



//Method to add an entry in the address table, given its MAC address and next_hop
void LearningTableWAPB::addEntryw(AddressEntry entry, MACAddress address, MACAddress next_h, unsigned int protocolVersion)
{
    entry.inTime = simTime();
    entry.next_hop = next_h;
    entry.status = locked;
    if(protocolVersion == 3 && entry.portToSend == -1) //If v3 and portOrig still not set, set it to the same value than port
        entry.portToSend = entry.port;
    addressTable[address] = entry;
}


//Method to add an entry in the address table, given its MAC address and inputport
void LearningTableWAPB::addEntry(AddressEntry entry, MACAddress address, int inputport, unsigned int protocolVersion)
{
    entry.inTime = simTime();
    entry.port = inputport;
    entry.status = locked;
    if(protocolVersion == 3 && entry.portToSend == -1) //If v3 and portOrig still not set, set it to the same value than port
        entry.portToSend = entry.port;
    addressTable[address] = entry;
}


//Method to find an entry in the address table, given its MAC address.
bool LearningTableWAPB::findEntry(MACAddress address, AddressEntry& entry)
{
    bool found = false;
    AddressTable::iterator iter = addressTable.find(address);

    if (iter != addressTable.end())
    {
        entry.inTime = iter->second.inTime;
        entry.ip = iter->second.ip;
        entry.port = iter->second.port;
        entry.portToSend = iter->second.portToSend;
        entry.status = iter->second.status;
        found = true;
    }
    else
    {
        entry.port = -1;
        entry.portToSend = -1;
    }
    return found;
}


//Method to find an entry in the address table, given its MAC address.
bool LearningTableWAPB::findEntryw(MACAddress address, AddressEntry& entry)
{
    bool found = false;
    AddressTable::iterator iter = addressTable.find(address);

    if (iter != addressTable.end())
    {
        entry.inTime = iter->second.inTime;
        entry.ip = iter->second.ip;
        entry.next_hop = iter->second.next_hop;
        entry.status = iter->second.status;
        found = true;
    }
    else
    {
        entry.next_hop = entry.next_hop.UNSPECIFIED_ADDRESS;
    }
    return found;
}

int LearningTableWAPB::getNumberOfEntriesForPort(int port){
     AddressTable::iterator iter;
     int number=0;
     for (iter = addressTable.begin(); iter!=addressTable.end(); ++iter)
     {
         if (iter->second.port == port ){//&& !strcmp(iter->second.status,"learnt")){
             number++;
         }
     }
    return number;
}


void LearningTableWAPB::clearLT()
{
    addressTable.clear();
}

//Method to print the contents of the address table (LT).
void LearningTableWAPB::printLT()
{
    AddressTable::iterator iter;
    EV << endl;
    EV << "Address Table (" << this->getParentModule()->getFullName() << ") (" << addressTable.size() << " entries):\n";
    EV << "  ----------------------------------------------------------------------------------" << endl;
    EV << "  | Address                     | Next-Hop                      | Timer | Status  | IP                  |" << endl;
    EV << "  ----------------------------------------------------------------------------------" << endl;
    for (iter = addressTable.begin(); iter!=addressTable.end(); iter++)
    {
        EV << "  | " << iter->first << " |  " << iter->second.next_hop << " | ";
                float time = simTime().dbl()-iter->second.inTime.dbl();
                char buffer[20];
                sprintf(buffer, "%.03f | ", time);
                EV << buffer; //Print timer just with 3 decimals
                if(iter->second.status == locked) EV << "locked | ";
                else EV << "learnt | ";
                EV << iter->second.ip << "    |" << endl;
     //   EV << "  ---------------------------------------------------------------" << endl;
    }
}


} // namespace wapb

