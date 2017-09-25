// Copyright (C) 2014 Isaias Martinez Yelmo (Inet 2.2.0 and Inet 2.3.0 adaptation)
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

#include "MACRelayUnitRoute.h"

Define_Module(MACRelayUnitRoute);

void MACRelayUnitRoute::initialize()
{
	saveRouteStatistics = false;
}

void MACRelayUnitRoute::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
}

void MACRelayUnitRoute::finish()
{
	printRouteStatistics();
}

void MACRelayUnitRoute::updateAndSaveRoute(EtherFrame* frame)
{
	EV << "->MACRelayUnitRoute::updateAndSaveRoute()" << endl;

	//UPDATE ROUTE (always)
	cPacket *packet = frame->decapsulate(); //getEncapsulatedPacket();
	ARPPacketRoute* arpPacket = check_and_cast<ARPPacketRoute*>(packet);
	std::string route = arpPacket->getRoute();
	route = route + '-' + this->getParentModule()->getFullName(); //Name of the switch
	arpPacket->setRoute(route.c_str()); //Update route in the ARP packet message
	frame->encapsulate(arpPacket);
	EV << "      Route: " << route << endl;

	//SAVE ROUTE (only if applicable)
	if(saveRouteStatistics)
	{
		EV << "  Route: " << route << " at switch " << this->getParentModule()->getFullName() << " is applicable?" << endl;
		size_t pos = route.find("."); //Get the first '.' position
		if(pos != std::string::npos)
		{
			std::string requestRoute = route.substr(0,pos); //Get the subroute from the ARP-Request to the ARP-Reply
			std::string replyRoute = route.substr(pos+1); //Get the rest of the subroute since the ARP-Reply

			pos = requestRoute.find("-");
			std::string srcHost = requestRoute.substr(0,pos); //Get the source host
			std::string srcRoute = requestRoute.substr(pos+1); //Get the rest of the route

			pos = replyRoute.find("-");
			std::string dstHost = replyRoute.substr(0,pos); //Get the destination host
			std::string dstRoute = replyRoute.substr(pos+1); //Get the rest of the route

			bool servingSrc = isServingSrc(srcRoute, dstRoute);
			bool servingDst = isServingDst(srcRoute, dstRoute);
			if(servingSrc || servingDst)
			{
				EV << "    Yes! Is serving source = " << servingSrc << "; is serving destination = " << servingDst << endl;
				EV << "      srcRoute = " << srcRoute << "; dstRoute = " << dstRoute << endl;
				if(servingSrc)
					saveRoute(dstRoute);
				else //servingDst
					saveRoute(srcRoute);
			}
			else
				EV << "    No. Switch is not serving source or destination!" << endl;
		}
		else
			EV << "    No. Still not ARP Reply!" << endl;
	}

	EV << "<-MACRelayUnitRoute::updateAndSaveRoute()" << endl;
}

void MACRelayUnitRoute::saveRoute(std::string route)
{
	EV << "->MACRelayUnitRoute::saveRoute()" << endl;

	EV << "  Saving route: " << route << " at switch " << this->getParentModule()->getFullName() << endl;
	unsigned int pos = route.find("-"); //Get the first '-' position
	std::string srcSwitch = route.substr(0,pos); //Get the source switch
	unsigned int i,j,k;

	//If source switch route should be saved, save it
	for(i=0; i<routeSrcSwitches.size(); i++)
	{
		if(routeSrcSwitches[i] == srcSwitch)
		{
			//Check if the src switch was already saved and then check the route
			bool switchExists = false;
			bool routeExists = false;
			for(j=0; j<savedSwitchRoutes.size(); j++)
			{
				if(savedSwitchRoutes[j].srcSwitch == srcSwitch)
				{
					switchExists = true;
					//Check if the route was already saved and increment the counter if so
					for(k=0; k<savedSwitchRoutes[j].savedRoutes.size(); k++)
					{
						if(savedSwitchRoutes[j].savedRoutes[k].name == route)
						{
							routeExists = true;
							savedSwitchRoutes[j].savedRoutes[k].counter++;
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
				newRoute.name = route;
				newRoute.counter = 1;

				if(!switchExists)
				{
					SwitchRouteEntry newSwitchRoute;
					newSwitchRoute.srcSwitch = srcSwitch;
					newSwitchRoute.savedRoutes.push_back(newRoute);
					savedSwitchRoutes.push_back(newSwitchRoute);
				}
				else
					savedSwitchRoutes[j].savedRoutes.push_back(newRoute);
			}
		}
	}

	//Print saved routes right now
	EV << "  Printing 'savedSwitchRoutes'..." << endl;
	for(i=0; i<savedSwitchRoutes.size(); i++)
	{
		EV << "    Source switch: " << savedSwitchRoutes[i].srcSwitch << endl;
		for(j=0; j<savedSwitchRoutes[i].savedRoutes.size(); j++)
			EV << "      Route: " << savedSwitchRoutes[i].savedRoutes[j].name << " (" << savedSwitchRoutes[i].savedRoutes[j].counter << ")" << endl;
	}

	EV << "<-MACRelayUnitRoute::saveRoute()" << endl;
}

bool MACRelayUnitRoute::isServingSrc(std::string srcRoute, std::string dstRoute)
{
	bool servingSrc = false;

	/*The condition is that the both routes are the same, but one is the reverse of the other*/
	size_t posSrc = srcRoute.find("-");
	if(posSrc != std::string::npos)
	{
		//Let's check both routes, by saving their switches
		std::vector<std::string> route1, route2;
		size_t posDst = dstRoute.find("-");
		while(posDst!=std::string::npos && posSrc!=std::string::npos)
		{
			//Get next switches for both routes and add them to the vector
			std::string switch1, switch2;
			switch1 = srcRoute.substr(0,posSrc);
			switch2 = dstRoute.substr(0,posDst);
			route1.push_back(switch1);
			route2.push_back(switch2);

			//Update route strings to the next hop
			srcRoute = srcRoute.substr(posSrc+1);
			dstRoute = dstRoute.substr(posDst+1);
			posSrc = srcRoute.find("-");
			posDst = dstRoute.find("-");
		}

		//The number of switches should be the same in each route (so, the pos variables should be both npos, stopped at the same time)
		if((posDst==std::string::npos && posSrc==std::string::npos) && (route1.size() == route2.size())) //It is not necessary to check: (route1.size() == route2.size()) if the first part is true
			servingSrc = true; //In general, not necessary to check if they're the same, they SHOULD BE (because of the protocol), but if a second ARP Request changed the route before the ARP Reply... //TODO: Case ignored, very rare to happen (specially because we are always working with edge switches... and because path would not be saved that way...)
	}
	else //Switch is unique in the route
		if(srcRoute == dstRoute) //If serves src and dst at the same time, the route should be the same <- In general, not necessary to check if they're the same, they SHOULD BE (because of the protocol), but if a second ARP Request changed the route before the ARP Reply...
			servingSrc = true;

	return servingSrc;
}

bool MACRelayUnitRoute::isServingDst(std::string srcRoute, std::string dstRoute)
{
	bool servingDst = false;

	/*The condition is that the route does not have an '-' because is the first switch after the ARP Reply*/
	if(dstRoute.find("-") == std::string::npos) //If serving to dst, the character '-' will not be found (this switch was the first added to the route)
		servingDst = true;

	return servingDst;
}

void MACRelayUnitRoute::activateRouteStatistics(std::vector<std::string> sources)
{
	EV << "->MACRelayUnitRoute::activateRouteStatistics()" << endl;

	EV << "  Activating route statistics for destination switch " << this->getParentModule()->getFullName();
	saveRouteStatistics = true; //Activate route statistics
	routeSrcSwitches = sources; //Save statistics about this source hosts (vector of strings with the fullname of each source host)

	EV << "<-MACRelayUnitRoute::activateRouteStatistics()" << endl;
}

void MACRelayUnitRoute::printRouteStatistics()
{
	EV << "->MACRelayUnitRoute::printRouteStatistics()" << endl;

	EV << "  ROUTES for destination SWITCH " << this->getParentModule()->getFullName() << endl;
	for(unsigned int i=0; i<savedSwitchRoutes.size(); i++)
	{
		EV << "  Source SWITCH " << savedSwitchRoutes[i].srcSwitch << endl;
		for(unsigned int j=0; j<savedSwitchRoutes[i].savedRoutes.size(); j++)
			EV << "    " << savedSwitchRoutes[i].savedRoutes[j].name << " " << savedSwitchRoutes[i].savedRoutes[j].counter << " " << endl;
	}

	EV << "<-MACRelayUnitRoute::printRouteStatistics()" << endl;
}


