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

#ifndef __LIBGIST_MACRELAYUNITROUTE_H_
#define __LIBGIST_MACRELAYUNITROUTE_H_

#include <omnetpp.h>
#include <string.h>
#include <vector>
#include "EtherFrame_m.h"
#include "ARPPacketRoute_m.h"

class MACRelayUnitRoute : public cSimpleModule
{
	friend class FlowGeneratorBase;
	friend class MACRelayUnitAPB;

  public:
    struct RouteEntry
    {
    	std::string name;
    	unsigned int counter;
    };

    struct SwitchRouteEntry
    {
    	std::string srcSwitch;
    	std::vector<RouteEntry> savedRoutes;
    };

  protected:
    bool saveRouteStatistics; //Indicates wheter this module has to save route statistics or not
    std::vector<std::string> routeSrcSwitches; //Source switches for which this module will save the route (for statistics)
    std::vector<SwitchRouteEntry> savedSwitchRoutes; //Vector that has the saved routes for the source switches required by the generator

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

	virtual void updateAndSaveRoute(EtherFrame* frame);
	virtual void saveRoute(std::string route);
	virtual bool isServingSrc(std::string srcRoute, std::string dstRoute);
	virtual bool isServingDst(std::string srcRoute, std::string dstRoute);
    virtual void activateRouteStatistics(std::vector<std::string> sources);
    virtual void printRouteStatistics();
};

#endif
