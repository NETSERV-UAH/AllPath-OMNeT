/*
 * Copyright (C) 2004 Andras Varga
 * Copyright (C) 2010 Diego Rivera (parameter to show number of ARP recieved added)
 * Copyright (C) 2012 Elisa Rojas (parameters, structures, functions and 'friendship' with UDPFlowGenerator to be able to save routes created with ARPPacketRoute, and updated to inet2.0.0, which already has the parameters for counting ARP messages)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __INET_ARPNEW_H
#define __INET_ARPNEW_H

//#include <stdio.h>
//#include <string.h>
//#include <vector>
#include <map>

#include "INETDefs.h"

#include "MACAddress.h"
#include "ModuleAccess.h"
#include "IPv4Address.h"

// Forward declarations:
class ARPPacketRoute; //EXTRA
class IInterfaceTable;
class InterfaceEntry;
class IRoutingTable;

/**
 * ARP implementation.
 */
class INET_API ARPNew : public cSimpleModule
{
    friend class FlowGeneratorBase; //EXTRA

  public:
    struct ARPCacheEntry;
    typedef std::map<IPv4Address, ARPCacheEntry*> ARPCache;
    typedef std::vector<cMessage*> MsgPtrVector;

    // IPv4Address -> MACAddress table
    // TBD should we key it on (IPv4Address, InterfaceEntry*)?
    struct ARPCacheEntry
    {
        InterfaceEntry *ie; // NIC to send the packet to
        bool pending; // true if resolution is pending
        MACAddress macAddress;  // MAC address
        simtime_t lastUpdate;  // entries should time out after cacheTimeout
        int numRetries; // if pending==true: 0 after first ARP request, 1 after second, etc.
        cMessage *timer;  // if pending==true: request timeout msg
        MsgPtrVector pendingPackets;  // if pending==true: ptrs to packets waiting for resolution
                                      // (packets are owned by pendingQueue)
        ARPCache::iterator myIter;  // iterator pointing to this entry
    };

    /*EXTRA*/
    struct RouteEntry
    {
        std::string name;
        unsigned int counter;
    };

    struct HostRouteEntry
    {
        std::string srcHost;
        std::vector<RouteEntry> savedRoutes;
    };
    /*EXTRA*/

  protected:
    simtime_t retryTimeout;
    int retryCount;
    simtime_t cacheTimeout;
    bool doProxyARP;
    bool globalARP;

    long numResolutions;
    long numFailedResolutions;
    long numRequestsSent;
    long numRepliesSent;

    static simsignal_t sentReqSignal;
    static simsignal_t sentReplySignal;
    static simsignal_t failedResolutionSignal;
    static simsignal_t initiatedResolutionSignal;

    ARPCache arpCache;
    static ARPCache globalArpCache;
    static int globalArpCacheRefCnt;

    cQueue pendingQueue; // outbound packets waiting for ARP resolution
    int nicOutBaseGateId;  // id of the nicOut[0] gate

    IInterfaceTable *ift;
    IRoutingTable *rt;  // for Proxy ARP

    // Maps an IP multicast address to an Ethernet multicast address.
    MACAddress mapMulticastAddress(IPv4Address addr);

    /*EXTRA*/
    bool saveRouteStatistics; //Indicates wheter this module has to save route statistics or not
    std::vector<std::string> routeSrcHosts; //Source hosts for which this module will save the route (for statistics)
    std::vector<HostRouteEntry> savedHostRoutes; //Vector that has the saved routes for the source hosts required by the generator
    /*EXTRA*/

  public:
    ARPNew();
    virtual ~ARPNew();
    int numInitStages() const {return 5;}
    const MACAddress getDirectAddressResolution(const IPv4Address &) const;
    const IPv4Address getInverseAddressResolution(const MACAddress &) const;
    void setChangeAddress(const IPv4Address &);

  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

    virtual void processOutboundPacket(cMessage *msg);
    virtual void sendPacketToNIC(cMessage *msg, InterfaceEntry *ie, const MACAddress& macAddress, int etherType);
    virtual void sendPacketToNICForwarding(cMessage *msg, InterfaceEntry *ie, const MACAddress& macAddress,const MACAddress& macSRCAddress, int etherType);

    virtual void initiateARPResolution(ARPCacheEntry *entry);
    virtual void sendARPRequest(InterfaceEntry *ie, IPv4Address ipAddress);
    virtual void requestTimedOut(cMessage *selfmsg);
    virtual bool addressRecognized(IPv4Address destAddr, InterfaceEntry *ie);
    virtual void processARPPacket(ARPPacketRoute *arp); //EXTRA
    virtual void updateARPCache(ARPCacheEntry *entry, const MACAddress& macAddress);

    virtual void dumpARPPacket(ARPPacketRoute *arp); //EXTRA
    virtual void updateDisplayString();

    /*EXTRA*/
    virtual void activateRouteStatistics(std::vector<std::string> sources);
    virtual void saveRoute(std::string route);
    virtual void printRouteStatistics();
    /*EXTRA*/

};

class INET_API ArpAccess : public ModuleAccess<ARPNew>
{
  public:
    ArpAccess() : ModuleAccess<ARPNew>("arp") {}
};

#endif

