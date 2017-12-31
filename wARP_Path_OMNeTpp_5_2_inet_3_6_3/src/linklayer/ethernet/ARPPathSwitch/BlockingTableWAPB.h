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

#ifndef WAPB_SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_BLOCKINGTABLEWAPB_H
#define WAPB_SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_BLOCKINGTABLEWAPB_H

#include "inet/linklayer/common/MACAddress.h"

#include <map>

using namespace inet;
namespace wapb {

#define MAX_LINE    100

/** BT(Blocking/Broadcast Table) **/
class BlockingTableWAPB : public cSimpleModule
{
public:
    struct AddressEntryBasic //Entry of BT - key is MAC address to lock (to avoid loops)
    {
        int port;                   // Input port
        simtime_t inTime;           // Creation time of the entry (timer)
        MACAddress next_hop;
    };
protected:

    struct MAC_compare
    {
        bool operator()(const MACAddress& u1, const MACAddress& u2) const
        {return u1.compareTo(u2) < 0;}
    };

    typedef std::map<MACAddress, AddressEntryBasic, MAC_compare> AddressTableBasic;
    AddressTableBasic addressTableBasic;
    simtime_t blockingTime;    // Max idle time for address table entries


    friend std::ostream& operator<<(std::ostream& os, const AddressEntryBasic& entry);

protected:
    virtual void initialize();

    virtual void handleMessage(cMessage *);

public:

    virtual bool updateTableBasic(MACAddress address, AddressEntryBasic entryBasic); //new

    //Method to remove obsolete entries from BT (Blocking/Broadcast Table)
    virtual void updateAndRemoveAgedEntriesFromBT();

    virtual void addEntryBasicw(AddressEntryBasic entry, MACAddress address, MACAddress next_h);

    //Method to add an entry in the special broadcast address table, given its MAC address and inputport
    virtual void addEntryBasic(AddressEntryBasic entry, MACAddress address, int inputport);

    //Method to find an entry in the special broadcast address table, given its MAC address.
    virtual bool findEntryBasic(MACAddress address, AddressEntryBasic& entryBasic);

    //Method to find an entry in the special broadcast address table, given its MAC address.
    virtual bool findEntryBasicw(MACAddress address, AddressEntryBasic& entryBasic);

    virtual void clearBT();

    //Method to print the contents of BT
    virtual void printBT();
};
} // namespace wapb

#endif // ifndef WAPB_SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_BLOCKINGTABLEWAPB_H
