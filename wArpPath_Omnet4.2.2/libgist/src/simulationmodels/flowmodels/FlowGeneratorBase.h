//
// Copyright (C) 05/2013 Elisa Rojas
//      Implements the base class for UDPFlowGenerator and TCPFlowGenerator, which share functionality
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

#ifndef FLOWGENERATORBASE_H_
#define FLOWGENERATORBASE_H_

#include <omnetpp.h>
#include "IPv4Address.h"
#include "MACAddress.h"
#include <string>
#include "UDPFlowHost.h"
#include "ARPNew.h"
#include "MACRelayUnitRoute.h"

class FlowGeneratorBase : public cSimpleModule
{
   protected:
      bool dataCenterTraffic; //Data center traffic? (default = false)

      //To save info about all the nodes in the network
      struct NodeInfo {
          NodeInfo() {isHost=false; isSwitch=false;}
          std::string fullName;
          std::string nedTypeName;
          bool isHost;
          bool isSwitch;
      };
      typedef std::vector<NodeInfo> NodeInfoVector;

      //To save info only about the host nodes in the network
      struct HostInfo {
          std::string fullName;
          IPv4Address ipAddress;
          MACAddress macAddress;
          UDPFlowHost * pUdpFlowHost; //Module in the host that generates the traffic
          ARPNew * pARPnew; //Module in the host that handles the ARP (route) messages
          unsigned int weight;
          unsigned int nFlowSource; //# of flows whose source was this host
          unsigned int nFlowDestination; //# of flows whose destination was this host
          unsigned int averageSizeSource; //Average flow size as source
          unsigned int averageSizeDestination; //Average flow size as destination
      };
      typedef std::vector<HostInfo> HostInfoVector;

      //To save info only about the switch nodes in the network
      struct SwitchInfo {
          std::string fullName;
          MACRelayUnitRoute * pMACRelayUnitRoute; //Module in the switch that will handle the ARP (route) messages
      };
      typedef std::vector<SwitchInfo> SwitchInfoVector;

      //To save info only about the host with weight != 1 in the network
      struct HostWeight {
          IPv4Address ipAddress;
          unsigned int weight;
      };
      typedef std::vector<HostWeight> HostWeightVector;

      //To save info about src and dst for route statistics
      struct DstSrcs {
          std::string dst;
          std::vector<std::string> srcs;
      };
      typedef std::vector<DstSrcs> DstSrcsVector;

      std::vector<IPvXAddress> excludedAddresses;
      HostWeightVector hostsWeights;
      simtime_t statsPeriod;
      simtime_t lastStatsTime;
      simtime_t stopTime;

      int numSent;
      int numReceived;

      NodeInfoVector nodeInfo; //Vector that contains the topology, it will be of size topo.nodes[]
      HostInfoVector hostInfo; //Vector that contains only the hosts in the topology and their IP and MAC addresses
      SwitchInfoVector switchInfo; //Vector that contains only the switches in the topology

      //std::vector<std::string> generatedFlows; //Vector that contains the strings of the generated flows

   protected:
      virtual int numInitStages() const  {return 4;} //At least 3 (=4-1) because we need FlatNetworkConfigurator to be initialized (and it does at stage 2)
      virtual void initialize(int stage);
      virtual void extractTopology();
      virtual void initializeRouteStatistics();
      virtual unsigned int getHostWeight(IPv4Address host);
      virtual void getRandomSrcDstIndex(int& iSource, int& iDestination);
      virtual void startRandomFlow();
      virtual void handleMessage(cMessage *msg);
      virtual void processPacket(cPacket *msg);
      virtual void finish();
};

#endif /* FLOWGENERATORBASE_H_ */
