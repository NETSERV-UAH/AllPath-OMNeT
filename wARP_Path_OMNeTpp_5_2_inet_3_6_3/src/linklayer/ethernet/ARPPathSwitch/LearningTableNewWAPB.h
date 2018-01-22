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

#ifndef WAPB_LINKLAYER_ETHERNET_ARPPATHSWITCH_LEARNINGTABLENEWAPB_H_
#define WAPB_LINKLAYER_ETHERNET_ARPPATHSWITCH_LEARNINGTABLENEWAPB_H_

#include "inet/common/INETDefs.h"
#include "inet/linklayer/common/MACAddress.h"
#include "inet/linklayer/ethernet/switch/IMACAddressTable.h"

#include "inet/networklayer/contract/ipv4/IPv4Address.h"

namespace wapb {
using namespace inet;

/**
 * This module handles the mapping between ports and MAC addresses. See the NED definition for details.
 */
class LearningTableNewWAPB : public cSimpleModule
{
  protected:
       enum Status { none = 0, locked = 1, learnt = 2}; //Default: 0,1,2

  protected:
      struct AddressEntry
      {
          unsigned int vid = 0;                                   // VLAN ID
          int portno = -1;                                        // Input port, used to learn/send (v2) and only to learn (v3)
          int portToSend = -1;                                    // Original (prioritary) input port, used to send (only v3)
          simtime_t insertionTime;                                // Creation time of the entry (timer)
          int status = 0;                                         // Entry Status (locked, learnt)
          IPv4Address ip = IPv4Address::UNSPECIFIED_ADDRESS;       // host IP address (for EtherProxy)
          MACAddress next_hop = MACAddress::UNSPECIFIED_ADDRESS;   // next hop (for ARPPath-wireless)

          AddressEntry() {}
          AddressEntry(unsigned int vid, int portno, int portToSend, simtime_t insertionTime, int status, IPv4Address ip, MACAddress next_hop) :
              vid(vid), portno(portno), portToSend(portToSend), insertionTime(insertionTime), status(status), ip(ip), next_hop(next_hop) {}
      };

    friend std::ostream& operator<<(std::ostream& os, const AddressEntry& entry);

    struct MAC_compare
    {
        bool operator()(const MACAddress& u1, const MACAddress& u2) const { return u1.compareTo(u2) < 0; }
    };

    typedef std::map<MACAddress, AddressEntry, MAC_compare> AddressTable;
    typedef std::map<unsigned int, AddressTable *> VlanAddressTable;

    simtime_t agingTime;    // Max idle time for address table entries
    simtime_t lastPurge;    // Time of the last call of removeAgedEntriesFromAllVlans()
    AddressTable *addressTable = nullptr;    // VLAN-unaware address lookup (vid = 0)
    VlanAddressTable vlanAddressTable;    // VLAN-aware address lookup

  protected:

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    /**
     * @brief Returns a MAC Address Table for a specified VLAN ID
     */
    AddressTable *getTableForVid(unsigned int vid);

  public:

    LearningTableNewWAPB();
    ~LearningTableNewWAPB();

  public:
    // Table management

    /**
     * @brief For a known arriving port, V-TAG and destination MAC. It finds out the port where relay component should deliver the message
     * @param address MAC destination
     * @param vid VLAN ID
     * @return Output port for address, or -1 if unknown.
     */
    virtual int getPortForAddress(MACAddress& address, unsigned int vid = 0);

    // used for wireless ARP-Path
    virtual MACAddress getNextHopForAddress(MACAddress address, unsigned int vid = 0);

    /**
     * @brief Register a new MAC address at AddressTable.
     * @return True if refreshed. False if it is new.
     */
    // used for ARP-Path
    virtual bool updateTableWithAddress(int portno, MACAddress& address, int status = 0, unsigned int vid = 0);

    // used for wireless ARP-Path
    virtual bool updateTableWithAddress(MACAddress nextHop, MACAddress& address, int status = 0, unsigned int vid = 0);

    // used for wireless ARP-Path
    virtual bool refreshTimer(MACAddress& address, unsigned int vid = 0);

    /**
     *  @brief Clears portno cache
     */
    // TODO: find a better name
    virtual void flush(int portno);

    /**
     *  @brief Prints cached data
     */
    virtual void printState();

    /**
     * @brief Copy cache from portA to portB port
     */
    virtual void copyTable(int portA, int portB);

    /**
     * @brief Remove aged entries from a specified VLAN
     */
    virtual void removeAgedEntriesFromVlan(unsigned int vid = 0);
    /**
     * @brief Remove aged entries from all VLANs
     */
    virtual void removeAgedEntriesFromAllVlans();

    /*
     * It calls removeAgedEntriesFromAllVlans() if and only if at least
     * 1 second has passed since the method was last called.
     */
    virtual void removeAgedEntriesIfNeeded();

    /**
     * Pre-reads in entries for Address Table during initialization.
     */
   // virtual void readAddressTable(const char *fileName);

    /**
     * For lifecycle: clears all entries from the vlanAddressTable.
     */
    virtual void clearTable();

    /*
     * Some (eg.: STP, RSTP) protocols may need to change agingTime
     */
    virtual void setAgingTime(simtime_t agingTime);
    virtual void resetDefaultAging();
};

} // namespace wapb

#endif // ifndef WAPB_LINKLAYER_ETHERNET_ARPPATHSWITCH_LEARNINGTABLENEWAPB_H_

