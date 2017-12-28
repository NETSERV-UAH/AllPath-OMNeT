/*
 * ArpPathMulticastMacAddressTable.h
 *
 *  Created on: ?? «—œ?»Â‘  ???? Âû.‘.
 *      Author: Simject
 */

#ifndef SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_ARPPATHMULTICASTMACADDRESSTABLE_H_
#define SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_ARPPATHMULTICASTMACADDRESSTABLE_H_

#include "inet/common/INETDefs.h"
#include "inet/linklayer/common/MACAddress.h"
using namespace inet;

namespace allpath {
class ArpPathMulticastMacAddressTable
{
  protected:
    struct TableEntry
    {
        unsigned int vid = 0;    // VLAN ID
        int portno = -1;    // Input port
        uint64 lateList = 0;  // save port of late frames
        simtime_t insertionTime;    // Arrival time of Lookup Address Table entry
        TableEntry() {}
        TableEntry(unsigned int vid, int portno, uint64 lateList, simtime_t insertionTime) :
            vid(vid), portno(portno),lateList(lateList), insertionTime(insertionTime) {}
    };


    struct MAC_compare
    {
        bool operator()(const MACAddress& u1, const MACAddress& u2) const { return u1.compareTo(u2) < 0; }
    };

    typedef std::map<MACAddress, TableEntry, MAC_compare> ArpPathTable;
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

    ArpPathMulticastMacAddressTable();
    ArpPathMulticastMacAddressTable(simtime_t agingTime);
    ~ArpPathMulticastMacAddressTable();

  public:

     // Table management

     void setAgingTime(simtime_t agingTime);



    /**
     * @brief For a known arriving port, V-TAG and destination MAC. It finds out the port where relay component should deliver the message
     * @param address MAC destination
     * @param vid VLAN ID
     * @return Output port for address, or -1 if unknown.
     */
    virtual int getPortForAddress(MACAddress address, unsigned int vid = 0);

    virtual uint64 getLateListForAddress(MACAddress address, unsigned int vid = 0);

    /**
     * @brief Register a new MAC address at ArpPathMactableTable.
     * @return True if refreshed. False if it is new.
     */
    virtual bool updateTableWithAddress(int portno, MACAddress& address, uint64 latePort = 0,  unsigned int vid = 0);

    bool updateLateListWithAddress(int latePort, MACAddress& address, unsigned int vid = 0);


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

    virtual MACAddress *getAddressesAssociatedToPort(int portnumber, unsigned int vid = 0);
    void removeMACAddressFromVlan(MACAddress adr, unsigned int vid=0);
};

}
#endif /* SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_ARPPATHMULTICASTMACADDRESSTABLE_H_ */
