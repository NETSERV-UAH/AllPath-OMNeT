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

#ifndef SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_ARPPATHIPANDMACADDRESSTABLE_H_
#define SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_ARPPATHIPANDMACADDRESSTABLE_H_
#include "inet/common/INETDefs.h"
#include "inet/linklayer/common/MACAddress.h"
#include "inet/networklayer/contract/ipv4/IPv4Address.h"
using namespace inet;


namespace allpath {
class ArpPathIPandMacAddressTable {
protected:
  struct TableEntry
  {
      unsigned int vid = 0;    // VLAN ID
      int portno = -1;    // Input port
      simtime_t insertionTime;    // Arrival time of Lookup Address Table entry
      TableEntry() {}
      TableEntry(unsigned int vid, int portno, simtime_t insertionTime) :
          vid(vid), portno(portno), insertionTime(insertionTime) {}
  };
  typedef std::pair<MACAddress,IPv4Address> FlowAddress;

  struct FlowAddress_compare
  {
      bool operator()(const FlowAddress& u1, const FlowAddress& u2) const
      {
          if (u1.first.compareTo(u2.first) < 0)
              return true;
          else if (u1.first.compareTo(u2.first) > 0)
              return false;
          else
              if (u1.second < u2.second)
                  return true;
              else return false;
      }
  };

  typedef std::map<FlowAddress, TableEntry, FlowAddress_compare> ArpPathTable;
  typedef std::map<unsigned int, ArpPathTable *> VlanArpPathTable;

  simtime_t agingTime;    // Max idle time for address table entries
  simtime_t lastPurge;    // Time of the last call of removeAgedEntriesFromAllVlans()
  ArpPathTable *arpPathTable = nullptr;    // VLAN-unaware address lookup (vid = 0)
  VlanArpPathTable vlanArpPathTable;    // VLAN-aware address lookup

protected:


  /**
   * @brief Returns a MAC Address Table for a specified VLAN ID
   */
  ArpPathTable *getTableForVid(unsigned int vid);

public:

  ArpPathIPandMacAddressTable();
  ArpPathIPandMacAddressTable(simtime_t agingTime);
  ~ArpPathIPandMacAddressTable();

public:

   // Table management

   void setAgingTime(simtime_t agingTime);



  /**
   * @brief For a known arriving port, V-TAG and destination MAC. It finds out the port where relay component should deliver the message
   * @param address MAC destination
   * @param vid VLAN ID
   * @return Output port for address, or -1 if unknown.
   */
  virtual int getPortForAddress(MACAddress address1, IPv4Address address2, unsigned int vid = 0);


  /**
   * @brief Register a new MAC address at ArpPathMactableTable.
   * @return True if refreshed. False if it is new.
   */
  virtual bool updateTableWithAddress(int portno, MACAddress& address1, IPv4Address address2, unsigned int vid = 0);


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
   * For lifecycle: clears all entries from the vlanArpPathMactableTable.
   */
  virtual void clearTable();

  /*
   * Some (eg.: STP, RSTP) protocols may need to change agingTime
   */

  virtual bool isPortintable(int portno, unsigned int vid=0);
  virtual int getnumbersOfAddressesAssociated(int portno, unsigned int vid=0);

  /*
   * index=1 : first mac address that is associated to the port
   * index=2 : second mac address that is associated to the port
   */

  virtual FlowAddress *getAddressesAssociatedToPort(int portnumber, unsigned int vid = 0);
  void removeFlowAddressFromVlan(FlowAddress adr, unsigned int vid=0);
};

}

#endif /* SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_ARPPATHIPANDMACADDRESSTABLE_H_ */
