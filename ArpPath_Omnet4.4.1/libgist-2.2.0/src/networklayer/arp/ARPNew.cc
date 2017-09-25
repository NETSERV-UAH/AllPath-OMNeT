/*
 * Copyright (C) 2004 Andras Varga
 * Copyright (C) 2008 Alfonso Ariza Quintana (global arp)
 * Copyright (C) 2010 Diego Rivera (parameter to show number of ARP recieved added)
 * Copyright (C) 2012 Elisa Rojas (parameters, structures, functions and 'friendship' with UDPFlowGenerator to be able to save routes created with ARPPacketRoute, and updated to inet2.0.0, which already has the parameters for counting ARP messages)
 * Copyright (C) 2014 Isaias Martinez Yelmo (Inet 2.2.0 and Inet 2.3.0 adaptation)
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


#include "ARPNew.h"

#include "Ieee802Ctrl_m.h"
#include "IPv4ControlInfo.h"
#include "IPv4Datagram.h"
#include "IPv4InterfaceData.h"
#include "IRoutingTable.h"
#include "RoutingTableAccess.h"
#include "ARPPacketRoute_m.h" //EXTRA
#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"

static std::ostream& operator<<(std::ostream& out, const ARPNew::ARPCacheEntry& e) // EXTRA-IMY
{
    if (e.pending)
        out << "pending (" << e.numRetries << " retries)";
    else
        out << "MAC:" << e.macAddress << "  age:" << floor(simTime()-e.lastUpdate) << "s";
    return out;
}

Define_Module(ARPNew);

void ARPNew::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    if (stage == 0)
    {
        retryTimeout = par("retryTimeout");
        retryCount = par("retryCount");
        cacheTimeout = par("cacheTimeout");
        respondToProxyARP = par("respondToProxyARP");
        globalARP = par("globalARP");

        netwOutGate = gate("netwOut");

        // init statistics
        saveRouteStatistics = false; //EXTRA: Not active until the generator tells the module to save them
        numRequestsSent = numRepliesSent = 0;
        numResolutions = numFailedResolutions = 0;
        WATCH(numRequestsSent);
        WATCH(numRepliesSent);
        WATCH(numResolutions);
        WATCH(numFailedResolutions);

        WATCH_PTRMAP(arpCache);
        WATCH_PTRMAP(globalArpCache);
    }
    else if (stage == 4)  // IP addresses should be available
    {
        ift = InterfaceTableAccess().get();
        rt = check_and_cast<IRoutingTable *>(getModuleByPath(par("routingTableModule")));

        isUp = isNodeUp();

        // register our addresses in the global cache (even if globalARP is locally turned off)
        for (int i=0; i<ift->getNumInterfaces(); i++)
        {
            InterfaceEntry *ie = ift->getInterface(i);
            if (ie->isLoopback())
                continue;
            if (!ie->ipv4Data())
                continue;
            IPv4Address nextHopAddr = ie->ipv4Data()->getIPAddress();
            if (nextHopAddr.isUnspecified())
                continue; // if the address is not defined it isn't included in the global cache
            // check if the entry exist
            ARPCache::iterator it = globalArpCache.find(nextHopAddr);
            if (it!=globalArpCache.end())
                continue;
            ARPCacheEntry *entry = new ARPCacheEntry();
            entry->owner = this;
            entry->ie = ie;
            entry->pending = false;
            entry->timer = NULL;
            entry->numRetries = 0;
            entry->macAddress = ie->getMacAddress();

            ARPCache::iterator where = globalArpCache.insert(globalArpCache.begin(), std::make_pair(nextHopAddr, entry));
            ASSERT(where->second == entry);
            entry->myIter = where; // note: "inserting a new element into a map does not invalidate iterators that point to existing elements"
        }
        NotificationBoard *nb = NotificationBoardAccess().getIfExists();
        if (nb != NULL)
            nb->subscribe(this, NF_INTERFACE_IPv4CONFIG_CHANGED);
    }
}

void ARPNew::handleMessage(cMessage *msg)
{
    if (!isUp)
    {
        handleMessageWhenDown(msg);
        return;
    }

    if (msg->isSelfMessage())
    {
        requestTimedOut(msg);
    }
    // EXTRA-IMY Maybe "if (dynamic_cast<ARPPacketRoute *>(msg)) //EXTRA" " could be removed
    else if (dynamic_cast<ARPPacketRoute *>(msg)) //EXTRA
    {
        ARPPacketRoute *arp = check_and_cast<ARPPacketRoute *>(msg); //EXTRA
        processARPPacket(arp);
    }

    if (ev.isGUI())
        updateDisplayString();
}


void ARPNew::sendARPRequest(InterfaceEntry *ie, IPv4Address ipAddress)
{
    // find our own IPv4 address and MAC address on the given interface
    MACAddress myMACAddress = ie->getMacAddress();
    IPv4Address myIPAddress = ie->ipv4Data()->getIPAddress();

    // both must be set
    ASSERT(!myMACAddress.isUnspecified());
    ASSERT(!myIPAddress.isUnspecified());

    // fill out everything in ARP Request packet except dest MAC address
    ARPPacketRoute *arp = new ARPPacketRoute("arpREQ"); //EXTRA
    arp->setByteLength(ARP_HEADER_BYTES);
    arp->setOpcode(ARP_REQUEST);
    arp->setSrcMACAddress(myMACAddress);
    arp->setSrcIPAddress(myIPAddress);
    arp->setDestIPAddress(ipAddress);
    arp->setRoute(this->getParentModule()->getParentModule()->getFullName()); //EXTRA: Name of the host that emits the ARP Request (parent is NetworkLayerNew and parent again is StandardHostNew)

    static MACAddress broadcastAddress("ff:ff:ff:ff:ff:ff");
    sendPacketToNIC(arp, ie, broadcastAddress, ETHERTYPE_ARP);
    numRequestsSent++;
    emit(sentReqSignal, 1L);
}

void ARPNew::dumpARPPacket(ARPPacketRoute *arp) // EXTRA-IMY
{
    EV << (arp->getOpcode()==ARP_REQUEST ? "ARP_REQ" : arp->getOpcode()==ARP_REPLY ? "ARP_REPLY" : "unknown type")
       << "  src=" << arp->getSrcIPAddress() << " / " << arp->getSrcMACAddress()
       << "  dest=" << arp->getDestIPAddress() << " / " << arp->getDestMACAddress() << "\n";
}

void ARPNew::processARPPacket(ARPPacketRoute *arp) //EXTRA-IMY
{
    EV << "ARP packet " << arp << " arrived:\n";
    dumpARPPacket(arp);

    // extract input port
    IPv4RoutingDecision *controlInfo = check_and_cast<IPv4RoutingDecision*>(arp->removeControlInfo());
    InterfaceEntry *ie = ift->getInterfaceById(controlInfo->getInterfaceId());
    delete controlInfo;

    //
    // Recipe a'la RFC 826:
    //
    // ?Do I have the hardware type in ar$hrd?
    // Yes: (almost definitely)
    //   [optionally check the hardware length ar$hln]
    //   ?Do I speak the protocol in ar$pro?
    //   Yes:
    //     [optionally check the protocol length ar$pln]
    //     Merge_flag := false
    //     If the pair <protocol type, sender protocol address> is
    //         already in my translation table, update the sender
    //         hardware address field of the entry with the new
    //         information in the packet and set Merge_flag to true.
    //     ?Am I the target protocol address?
    //     Yes:
    //       If Merge_flag is false, add the triplet <protocol type,
    //           sender protocol address, sender hardware address> to
    //           the translation table.
    //       ?Is the opcode ares_op$REQUEST?  (NOW look at the opcode!!)
    //       Yes:
    //         Swap hardware and protocol fields, putting the local
    //             hardware and protocol addresses in the sender fields.
    //         Set the ar$op field to ares_op$REPLY
    //         Send the packet to the (new) target hardware address on
    //             the same hardware on which the request was received.
    //

    MACAddress srcMACAddress = arp->getSrcMACAddress();
    IPv4Address srcIPAddress = arp->getSrcIPAddress();

    if (srcMACAddress.isUnspecified())
        error("wrong ARP packet: source MAC address is empty");
    if (srcIPAddress.isUnspecified())
        error("wrong ARP packet: source IPv4 address is empty");

    bool mergeFlag = false;
    // "If ... sender protocol address is already in my translation table"
    ARPCache::iterator it = arpCache.find(srcIPAddress);
    if (it!=arpCache.end())
    {
        // "update the sender hardware address field"
        ARPCacheEntry *entry = (*it).second;
        updateARPCache(entry, srcMACAddress);
        mergeFlag = true;
    }

    // "?Am I the target protocol address?"
    // if Proxy ARP is enabled, we also have to reply if we're a router to the dest IPv4 address
    if (addressRecognized(arp->getDestIPAddress(), ie))
    {
        // "If Merge_flag is false, add the triplet protocol type, sender
        // protocol address, sender hardware address to the translation table"
        if (!mergeFlag)
        {
            ARPCacheEntry *entry;
            if (it!=arpCache.end())
            {
                entry = (*it).second;
            }
            else
            {
                entry = new ARPCacheEntry();
                ARPCache::iterator where = arpCache.insert(arpCache.begin(), std::make_pair(srcIPAddress, entry));
                entry->myIter = where;
                entry->ie = ie;

                entry->pending = false;
                entry->timer = NULL;
                entry->numRetries = 0;
            }
            updateARPCache(entry, srcMACAddress);
        }

        // "?Is the opcode ares_op$REQUEST?  (NOW look at the opcode!!)"
        switch (arp->getOpcode())
        {
            case ARP_REQUEST:
            {
                EV << "Packet was ARP REQUEST, sending REPLY\n";
                /*EXTRA*/ //Save ARP Request route if enabled
                if(saveRouteStatistics)
                    saveRoute(arp->getRoute());
                /*EXTRA*/

                // find our own IPv4 address and MAC address on the given interface
                MACAddress myMACAddress = ie->getMacAddress();
                IPv4Address myIPAddress = ie->ipv4Data()->getIPAddress();

                // "Swap hardware and protocol fields", etc.
                arp->setName("arpREPLY");
                IPv4Address origDestAddress = arp->getDestIPAddress();
                arp->setDestIPAddress(srcIPAddress);
                arp->setDestMACAddress(srcMACAddress);
                arp->setSrcIPAddress(origDestAddress);
                arp->setSrcMACAddress(myMACAddress);
                arp->setOpcode(ARP_REPLY);
                /*EXTRA*/
                std::string route = arp->getRoute();
                route = route + '.' + this->getParentModule()->getParentModule()->getFullName(); //Added name of the host that emits the ARP Reply (parent is NetworkLayerNew and parent again is StandardHostNew) and '.' indicates the change from Request to Reply
                arp->setRoute(route.c_str());
                /*EXTRA*/
                delete arp->removeControlInfo();
                sendPacketToNIC(arp, ie, srcMACAddress, ETHERTYPE_ARP);
                numRepliesSent++;
                emit(sentReplySignal, 1L);
                break;
            }
            case ARP_REPLY:
            {
                /*EXTRA*/ //Save ARP Reply route if enabled
                if(saveRouteStatistics)
                {
                    std::string route = arp->getRoute();
                    unsigned int pos = route.find("."); //Get the first '.' position, which indicates the change from Request to Reply and the second route followed
                    saveRoute(route.substr(pos+1)); //Get the route only for the ARP Reply (getRoute() includes everything)
                }
                /*EXTRA*/
                EV << "Discarding packet\n";
                delete arp;
                break;
            }
            case ARP_RARP_REQUEST: throw cRuntimeError("RARP request received: RARP is not supported");
            case ARP_RARP_REPLY: throw cRuntimeError("RARP reply received: RARP is not supported");
            default: throw cRuntimeError("Unsupported opcode %d in received ARP packet", arp->getOpcode());
        }
    }
    else
    {
        // address not recognized
        EV << "IPv4 address " << arp->getDestIPAddress() << " not recognized, dropping ARP packet\n";
        delete arp;
    }
}


/*EXTRA*/
void ARPNew::activateRouteStatistics(std::vector<std::string> sources)
{
    EV << "->ARPnew::activateRouteStatistics()" << endl;

    EV << "  Activating route statistics for destination host " << this->getParentModule()->getParentModule()->getFullName();
    saveRouteStatistics = true; //Activate route statistics
    routeSrcHosts = sources; //Save statistics about this source hosts (vector of strings with the fullname of each source host)

    EV << "<-ARPnew::activateRouteStatistics()" << endl;
}

void ARPNew::saveRoute(std::string route)
{
    EV << "->ARPnew::saveRoute()" << endl;

    EV << "  Saving route: " << route << " at host " << this->getParentModule()->getParentModule()->getFullName() << endl;
    unsigned int pos = route.find("-"); //Get the first '-' position
    std::string srcHost = route.substr(0,pos); //Get the source host
    std::string routeName = route.substr(pos+1); //Get the rest of the route (will be saved as 'name')
    unsigned int i,j,k;

    //If source host route should be saved, save it
    for(i=0; i<routeSrcHosts.size(); i++)
    {
        if(routeSrcHosts[i] == srcHost)
        {
            //Check if the src host was already saved and then check the route
            bool hostExists = false;
            bool routeExists = false;
            for(j=0; j<savedHostRoutes.size(); j++)
            {
                if(savedHostRoutes[j].srcHost == srcHost)
                {
                    hostExists = true;
                    //Check if the route was already saved and increment the counter if so
                    for(k=0; k<savedHostRoutes[j].savedRoutes.size(); k++)
                    {
                        if(savedHostRoutes[j].savedRoutes[k].name == routeName)
                        {
                            routeExists = true;
                            savedHostRoutes[j].savedRoutes[k].counter++;
                            break;
                        }
                    }
                    break;
                }
            }

            //If the route or the source had not been registered before...
            if(!routeExists)
            {
                //If it didn't exist, add it to the vector
                RouteEntry newRoute;
                newRoute.name = routeName;
                newRoute.counter = 1;

                if(!hostExists)
                {
                    HostRouteEntry newHostRoute;
                    newHostRoute.srcHost = srcHost;
                    newHostRoute.savedRoutes.push_back(newRoute);
                    savedHostRoutes.push_back(newHostRoute);
                }
                else
                    savedHostRoutes[j].savedRoutes.push_back(newRoute);
            }
        }
    }

    //Print saved routes right now
    EV << "  Printing 'savedHostRoutes'..." << endl;
    for(i=0; i<savedHostRoutes.size(); i++)
    {
        EV << "    Source host: " << savedHostRoutes[i].srcHost << endl;
        for(j=0; j<savedHostRoutes[i].savedRoutes.size(); j++)
            EV << "      Route: " << savedHostRoutes[i].savedRoutes[j].name << " (" << savedHostRoutes[i].savedRoutes[j].counter << ")" << endl;
    }

    EV << "<-ARPnew::saveRoute()" << endl;
}

void ARPNew::printRouteStatistics()
{
    EV << "->ARPnew::printRouteStatistics()" << endl;

    EV << "  ROUTES for destination HOST " << this->getParentModule()->getParentModule()->getFullName() << endl;
    for(unsigned int i=0; i<savedHostRoutes.size(); i++)
    {
        EV << "  Source HOST " << savedHostRoutes[i].srcHost << endl;
        for(unsigned int j=0; j<savedHostRoutes[i].savedRoutes.size(); j++)
            EV << "    " << savedHostRoutes[i].savedRoutes[j].name << " (" << savedHostRoutes[i].savedRoutes[j].counter << ")" << endl;
    }

    EV << "<-ARPnew::printRouteStatistics()" << endl;
}
/*EXTRA*/


