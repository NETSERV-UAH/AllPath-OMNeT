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

#include "FlowGeneratorBase.h"
#include "IInterfaceTable.h"
#include "IPvXAddressResolver.h"
#include "IPv4InterfaceData.h"

void FlowGeneratorBase::initialize(int stage)
{
    EV << "->FlowGeneratorBase::initialize()" << endl;
    //We wait until stage 3, because we need FlatNetworkConfigurator to assign the addresses, and it does so at stage 2
    //ERS: More info about 'stage': http://www.omnetpp.org/doc/omnetpp/api/classcComponent.html
    if(stage == 3) //Only if generator is active! (true)
    {
        //Get parameters and initialize variables
        dataCenterTraffic = par("dataCenterTraffic");
        stopTime = par("stopTime");
        numSent = 0;
        numReceived = 0;
        statsPeriod = par("statsPeriod");
        lastStatsTime = simTime();

        //Excluded addresses (from the traffic generation)
        const char *excludedAddrs = par("excludedAddresses");
        cStringTokenizer tokenizer(excludedAddrs);
        const char *token;
        while ((token = tokenizer.nextToken())!=NULL)
            excludedAddresses.push_back(IPvXAddressResolver().resolve(token).get4());

        //Host weights (for host whose weight is not 1)
        const char *hWeights = par("hostsWeights");
        tokenizer = cStringTokenizer(hWeights);
        while ((token = tokenizer.nextToken())!=NULL)
        {
            HostWeight newWeight;
            newWeight.ipAddress = IPvXAddressResolver().resolve(token).get4();
            if((token = tokenizer.nextToken())==NULL)
                error("'hostsWeights' parameter format is not correct!");
            newWeight.weight = atoi(token);
            hostsWeights.push_back(newWeight);
        }

        //Extract topology into the cTopology object, then fill in the vectors: NodeInfoVector and HostInfoVector
        extractTopology();

        //After extracting the topology, activate route statistics (for host/switches) if the params tell so
        initializeRouteStatistics();

        //The generator will start generating traffic at 'startTime' (parameter)
        cMessage *timer = new cMessage("Gen-NewFlow!");
        scheduleAt((double)par("startTime"), timer);
    }
    EV << "<-FlowGeneratorBase::initialize()" << endl;
}

void FlowGeneratorBase::extractTopology()
{
    EV << "->FlowGeneratorBase::extractTopology()" << endl;

    // extract topology
    cTopology topo("topo");
    topo.extractByProperty("node");
    EV << "  cTopology found " << topo.getNumNodes() << " nodes" << endl;

    // fill in NodeInfoVector (isHost and names) and HostInfoVector (IP and MAC addresses)
    nodeInfo.resize(topo.getNumNodes());
    unsigned int nHosts = 0;
    for (unsigned int i=0; i<topo.getNumNodes(); i++)
    {
        EV << "    Node #" << i << ":" <<endl;
        cModule *mod = topo.getNode(i)->getModule();
        nodeInfo[i].nedTypeName = std::string(mod->getNedTypeName()); //returns the ned type name
        nodeInfo[i].fullName = std::string(mod->getFullName()); //getFullName() or getName() returns the name assigned in the topology (such as host1, switch1, etc...)
        EV << "      Ned type: " << nodeInfo[i].nedTypeName << "; Name: " << nodeInfo[i].fullName <<endl;

        //isHost
        if (nodeInfo[i].nedTypeName.find("Host") != std::string::npos) //such as: EtherHost, EtherHostNew,...
        {
            nodeInfo[i].isHost = true;
            //Add element to hostInfo vector
            HostInfo newHost;
            IInterfaceTable *ift = IPvXAddressResolver().interfaceTableOf(mod);
            int nInterfaces = ift->getNumInterfaces();
            if(nInterfaces > 2) //If host has more than 2 interfaces...
                error("The host has more than 2 interfaces (one connected and loopback) and that's still not implemented!");
            for (unsigned int k=0; k<nInterfaces; k++)
            {
                InterfaceEntry *ie = ift->getInterface(k);
                //We add only the info about the entry which is not the loopback
                if (!ie->isLoopback())
                {
                    newHost.fullName = nodeInfo[i].fullName;
                    newHost.ipAddress = ie->ipv4Data()->getIPAddress();
                    newHost.macAddress = ie->getMacAddress();
                    newHost.pUdpFlowHost = check_and_cast<UDPFlowHost *>(mod->getSubmodule("udpGen")); //newHost.pUdpFlowHost = (UDPFlowHost *)mod->getSubmodule("udpGen");
                    newHost.pARPnew = check_and_cast<ARPNew *>(mod->getSubmodule("networkLayer")->getSubmodule("arp"));
                    newHost.weight = getHostWeight(newHost.ipAddress);
                    newHost.nFlowSource = 0;
                    newHost.nFlowDestination = 0;
                    newHost.averageSizeSource = 0;
                    newHost.averageSizeDestination = 0;
                    EV << "        " << newHost.fullName << "-> IP: " << newHost.ipAddress << "; MAC: " << newHost.macAddress << "; weight: " << newHost.weight <<endl;
                    nHosts++;
                }
            }

            //Before adding it to the hostInfo vector, exclude it if it is in the excludedAddresses vector
            bool isExcluded = false;
            for (unsigned int k=0; k<excludedAddresses.size(); k++)
            {
                if (excludedAddresses[k].equals(newHost.ipAddress))
                {
                    isExcluded = true;
                    EV << "          ...not included!" <<endl;
                    break;
                }
            }
            if(!isExcluded)
                hostInfo.push_back(newHost);
        }
        else if (nodeInfo[i].nedTypeName.find("Switch") != std::string::npos) //such as: EtherSwitch, EtherSwitchAPB,...
        {
            nodeInfo[i].isSwitch = true;
            //Add element to switchInfo vector
            SwitchInfo newSwitch;
            newSwitch.fullName = nodeInfo[i].fullName;
            newSwitch.pMACRelayUnitRoute = check_and_cast<MACRelayUnitRoute *>(mod->getSubmodule("relayUnitRoute"));
            switchInfo.push_back(newSwitch);
        }
    }

    unsigned int n = hostInfo.size();
    EV << "  and " << n << "(active)/" << nHosts << "(total) of those nodes are hosts" << endl;
    EV << "  and " << switchInfo.size() << " of those nodes are switches" << endl;
    //Finally, we update the number of hosts value in the UDPFlowHost module at each host
    for(unsigned int i=0; i<n; i++)
        hostInfo[i].pUdpFlowHost->updateHostsInfo(n, stopTime);

    EV << "<-FlowGeneratorBase::extractTopology()" << endl;
}

void FlowGeneratorBase::initializeRouteStatistics()
{
    EV << "->FlowGeneratorBase::routeStatistics()" << endl;

    unsigned int i, j, k;

    //HOST ROUTES
    EV << "  Host routes..." << endl;
    DstSrcsVector routeDsts;
    const char *rHosts = par("routeHosts");
    EV << "    'routeHosts' = " << rHosts << endl;
    cStringTokenizer tokenizer(rHosts);
    const char *token;

    //Get info from the parameter
    while ((token = tokenizer.nextToken())!=NULL)
    {
        //Get source and destination of the route
        std::string src = std::string(token);
        if((token = tokenizer.nextToken())==NULL)
            error("'routeHosts' parameter format is not correct!");
        std::string dst = std::string(token);
        EV << "      Adding src = " << src << " and dst = " << dst << endl;

        //Add source to that destination (first check if it's in the vector)
        for(i=0; i<routeDsts.size(); i++)
        {
            if(routeDsts[i].dst == dst)
            {
                //If destination is in the vector, check if source is as well
                for(j=0; j<routeDsts[i].srcs.size(); j++)
                    if(routeDsts[i].srcs[j] == src)
                        break;
                break;
            }
        }

        if(i==routeDsts.size()) //destination not in the vector yet
        {
            DstSrcs newDst;
            newDst.dst = dst;
            newDst.srcs.push_back(src);
            routeDsts.push_back(newDst);
        }
        else if(i!=routeDsts.size() && j==routeDsts[i].srcs.size()) //destination already in the vector, but not source
            routeDsts[i].srcs.push_back(src);
    }

    //Finally activate the route statistics at every source host
    EV << "    Activating hosts..." << endl;
    for(i=0; i<routeDsts.size(); i++)
    {
        for(j=0; j<hostInfo.size(); j++)
        {
            if(routeDsts[i].dst == hostInfo[j].fullName)
            {
                EV << "      Host " << hostInfo[j].fullName << " (destination) with ";
                for(k=0; k<routeDsts[i].srcs.size(); k++)
                    EV << routeDsts[i].srcs[k] << " ";
                EV << "(sources)" << endl;
                hostInfo[j].pARPnew->activateRouteStatistics(routeDsts[i].srcs);
            }
        }
    }

    //SWITCH ROUTES
    EV << "  -------------------" << endl;
    EV << "  Switch routes..." << endl;
    routeDsts.clear();
    const char *rSwitches = par("routeSwitches");
    EV << "    'routeSwitches' = " << rSwitches << endl;
    tokenizer = cStringTokenizer (rSwitches);

    //Get info from the parameter
    while ((token = tokenizer.nextToken())!=NULL)
    {
        //Get source and destination of the route
        std::string src = std::string(token);
        if((token = tokenizer.nextToken())==NULL)
            error("'routeSwitches' parameter format is not correct!");
        std::string dst = std::string(token);
        EV << "      Adding src = " << src << " and dst = " << dst << endl;

        //Add source to that destination (first check if it's in the vector)
        for(i=0; i<routeDsts.size(); i++)
        {
            if(routeDsts[i].dst == dst)
            {
                //If destination is in the vector, check if source is as well
                for(j=0; j<routeDsts[i].srcs.size(); j++)
                    if(routeDsts[i].srcs[j] == src)
                        break;
                break;
            }
        }

        if(i==routeDsts.size()) //destination not in the vector yet
        {
            DstSrcs newDst;
            newDst.dst = dst;
            newDst.srcs.push_back(src);
            routeDsts.push_back(newDst);
        }
        else if(i!=routeDsts.size() && j==routeDsts[i].srcs.size()) //destination already in the vector, but not source
            routeDsts[i].srcs.push_back(src);
    }

    //Finally activate the route statistics at every source switch
    EV << "    Activating switches..." << endl;
    for(i=0; i<routeDsts.size(); i++)
    {
        for(j=0; j<switchInfo.size(); j++)
        {
            if(routeDsts[i].dst == switchInfo[j].fullName)
            {
                EV << "      Switch " << switchInfo[j].fullName << " (destination) with ";
                for(k=0; k<routeDsts[i].srcs.size(); k++)
                    EV << routeDsts[i].srcs[k] << " ";
                EV << "(sources)" << endl;
                switchInfo[j].pMACRelayUnitRoute->activateRouteStatistics(routeDsts[i].srcs);
            }
        }
    }

    EV << "<-FlowGeneratorBase::routeStatistics()" << endl;
}

unsigned int FlowGeneratorBase::getHostWeight(IPv4Address host)
{
    unsigned int weight = 1; //Default weight is 1
    for(unsigned int i=0; i<hostsWeights.size(); i++)
        if(host == hostsWeights[i].ipAddress)
            weight = hostsWeights[i].weight;

    return weight;
}

void FlowGeneratorBase::getRandomSrcDstIndex(int& iSource, int& iDestination)
{
    //Generate the vector of host indexes, considering their weights
    unsigned int nHosts = hostInfo.size();
    std::vector<unsigned int> hostIndexes;
    for (unsigned int i=0; i<nHosts; i++)
        for (unsigned int j=0; j<hostInfo[i].weight; j++)
            hostIndexes.push_back(i);

    //Assign an uniform random index from that host indexes vector
    unsigned int nIndexes = hostIndexes.size();
    iSource = hostIndexes[intrand(nIndexes)];
    iDestination = hostIndexes[intrand(nIndexes)];
    while(iDestination == iSource) //While source and destination index are the same, choose a new destination index
        iDestination = hostIndexes[intrand(nIndexes)];
}

void FlowGeneratorBase::startRandomFlow()
{
    EV << "->FlowGeneratorBase::startRandomFlow()" << endl;

    EV << "<-FlowGeneratorBase::startRandomFlow()" << endl;
}

void FlowGeneratorBase::handleMessage(cMessage *msg)
{
    EV << "->FlowGeneratorBase::handleMessage()" << endl;
    if (msg->isSelfMessage())
    {
        // send, then reschedule next sending
        if (simTime()<=stopTime)
        {
            EV << "  Generating a new flow..." << endl;
            startRandomFlow(); //Generate flow (source, destination, rate, size) + send that info to the source host
            if(simTime()+(double)par("generatorFreq") <= stopTime)
                scheduleAt(simTime()+(double)par("generatorFreq"), msg); //Next generation
            else
                delete msg; //Simulation ended -> delete message associated to flow generation

            if(statsPeriod!=0 && statsPeriod < simTime()-lastStatsTime)
            {
                EV << "  Printing periodic statistics..." << endl;
                for (unsigned int i=0; i<switchInfo.size(); i++)
                    switchInfo[i].pMACRelayUnitRoute->printRouteStatistics();
                lastStatsTime = simTime();
            }
        }
        else
        {
            EV << "  Traffic generator ended! At " << simTime() << " with stop time T=" << stopTime << endl;
            delete msg; //Simulation ended -> delete message associated to flow generation
        }
    }
    else
    {
        // process incoming packet
        processPacket(PK(msg));
    }

    if (ev.isGUI())
    {
        char buf[40];
        sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
        getDisplayString().setTagArg("t",0,buf);
    }
    EV << "<-FlowGeneratorBase::handleMessage()" << endl;
}

void FlowGeneratorBase::processPacket(cPacket *msg)
{
    EV << "->FlowGeneratorBase::processPacket()" << endl;
    EV << "  Received packet: ";
    //printPacket(msg); - Only for UDPAppBase 'children'
    delete msg;
    numReceived++;
}

void FlowGeneratorBase::finish()
{
    EV << "->FlowGeneratorBase::finish()" << endl;

    //Print statistics...
    EV << "  Printing some statistics..." << endl;
    EV << "    #Sent = " << numSent << " ; #Received = " << numReceived << endl;
    for(unsigned int i=0; i<hostInfo.size(); i++)
    {
        EV << "      Host '" << hostInfo[i].fullName << "' -> " << hostInfo[i].macAddress << " [" << hostInfo[i].ipAddress << "]" << endl;
        EV << "        # Flow source = " << hostInfo[i].nFlowSource << " [Average size = " << hostInfo[i].averageSizeSource << "(KB)]" << endl;
        EV << "        # Flow destination = " << hostInfo[i].nFlowDestination << " [Average size = " << hostInfo[i].averageSizeDestination << "(KB)]" << endl;
        //EV << "        Average traffic offered to the network (as source) = " << (hostInfo[i].nFlowSource*hostInfo[i].averageSizeSource*8)/stopTime.dbl() << "(Kbps)" << endl; //###Esta estadística considera que se ha enviado todo el tráfico, pero probablemente no, porque en simTime (aprox) se para todo
    }
    /*EV << "    Generated flows..." << endl;
    for(unsigned int i=0; i<generatedFlows.size(); i++)
    {
        EV << "      " << generatedFlows[i] << endl;
    }*/
}


