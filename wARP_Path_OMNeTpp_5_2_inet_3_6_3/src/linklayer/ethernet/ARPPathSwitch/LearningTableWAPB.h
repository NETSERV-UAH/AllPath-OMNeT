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

#ifndef WAPB_SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_LEARNINGTABLEWAPB_H
#define WAPB_SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_LEARNINGTABLEWAPB_H

#include "inet/linklayer/common/MACAddress.h"
#include "inet/networklayer/contract/ipv4/IPv4Address.h"


using namespace inet;
namespace wapb {

/** LT(Lookup/Learning Table) **/

class LearningTableWAPB : public cSimpleModule
{

protected:

       enum Status { none = 0, locked = 1, learnt = 2}; //Default: 0,1,2
public:
       struct AddressEntry //Entry of LT - key is MAC address to forward to
       {
           int port;                   // Input port, used to learn/send (v2) and only to learn (v3)
           int portToSend;             // Original (prioritary) input port, used to send (only v3)
           simtime_t inTime;           // Creation time of the entry (timer)
           int status;                 // Entry Status (locked, learnt)
           IPv4Address ip;             // host IP address (for EtherProxy)
           MACAddress next_hop;        // next hop (for ARPPath-wireless)
       };
protected:
       struct MAC_compare
       {
           bool operator()(const MACAddress& u1, const MACAddress& u2) const
           {return u1.compareTo(u2) < 0;}
       };

       typedef std::map<MACAddress, AddressEntry, MAC_compare> AddressTable;
       AddressTable addressTable;
       simtime_t agingTime;    // Max idle time for address table entries
       friend std::ostream& operator<<(std::ostream& os, const AddressEntry& entry);

protected:

       virtual void initialize() override;

       virtual void handleMessage(cMessage *msg) override;

public:
       virtual bool updateTable(MACAddress address, AddressEntry entry);  //new
       virtual bool isPortInTable(int port);  //new
       virtual int getPortByAddress(MACAddress address);  //new
       virtual void getTableMapping(std::map<MACAddress, int>& mapping);  //new
       virtual void updateAndRemoveAgedEntriesFromLT();

       virtual void addEntryw(AddressEntry entry, MACAddress address, MACAddress next_h, unsigned int protocolVersion);

       virtual void addEntry(AddressEntry entry, MACAddress address, int inputport, unsigned int protocolVersion);

       virtual bool findEntry(MACAddress address, AddressEntry& entry);

       virtual bool findEntryw(MACAddress address, AddressEntry& entry);

       virtual int getNumberOfEntriesForPort(int port);

       virtual void clearLT();  // new

       virtual void printLT();
};

} // namespace wapb

#endif // ifndef WAPB_SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_LEARNINGTABLEWAPB_H

