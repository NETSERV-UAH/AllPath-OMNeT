/*
 * Copyright (C) 2004 Andras Varga
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

#ifndef __INET_ARPNEW_H
#define __INET_ARPNEW_H

//#include <stdio.h>
//#include <string.h>
//#include <vector>
#include <map>

#include "INETDefs.h"
#include "ARP.h" //EXTRA-IMY

#include "MACAddress.h"
#include "ModuleAccess.h"
#include "IPv4Address.h"

// Forward declarations:
class ARPPacketRoute; //EXTRA


class INET_API ARPNew : public ARP
{
    friend class FlowGeneratorBase; //EXTRA

    /* EXTRA-IMY functions modified by libgist */

  protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

    virtual void sendARPRequest(InterfaceEntry *ie, IPv4Address ipAddress);
    virtual void processARPPacket(ARPPacketRoute *arp); // EXTRA
    virtual void dumpARPPacket(ARPPacketRoute *arp); // EXTRA

    /* EXTRA-IMY */

  public:

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

    /*EXTRA*/
    bool saveRouteStatistics; //Indicates wheter this module has to save route statistics or not
    std::vector<std::string> routeSrcHosts; //Source hosts for which this module will save the route (for statistics)
    std::vector<HostRouteEntry> savedHostRoutes; //Vector that has the saved routes for the source hosts required by the generator
    /*EXTRA*/

  protected:

    /*EXTRA*/
    virtual void activateRouteStatistics(std::vector<std::string> sources);
    virtual void saveRoute(std::string route);
    virtual void printRouteStatistics();
    /*EXTRA*/

};

/* EXTRA-IMY access to ARPNew as module. It seems to be only needed in Manet Routing. To be considered in wireless scenarios. Thinking on an overrid class method if necessary */

class INET_API ArpNewAccess : public ModuleAccess<ARPNew>
{
  public:
    ArpNewAccess() : ModuleAccess<ARPNew>("arpNew") {}
};

/* EXTRA-IMY */

#endif

