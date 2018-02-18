// Copyright (C) 05/2013 Elisa Rojas
//      Implements the base class for UDPFlowGenerator and TCPFlowGenerator, which share functionality
/*
 * Copyright (C) 2018 Elisa Rojas(1), Hedayat Hosseini(2);
 *                    (1) GIST, University of Alcala, Spain.
 *                    (2) CEIT, Amirkabir University of Technology (Tehran Polytechnic), Iran.
 *                    INET 3.6.3 adaptation
*/
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

#include "src/simulationmodels/flowmodels/FlowGeneratorBase.h"

#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/ipv4/IPv4InterfaceData.h"

namespace wapb {
using namespace inet;

void FlowGeneratorBase::initialize(int stage)
{
    EV << "->FlowGeneratorBase::initialize()" << endl;
    //We wait until stage 3, because we need FlatNetworkConfigurator to assign the addresses, and it does so at stage 2
    //ERS: More info about 'stage': http://www.omnetpp.org/doc/omnetpp/api/classcComponent.html
    //if(stage == 3) //Only if generator is active! (true)
    if(stage == INITSTAGE_NETWORK_LAYER_3) // for preparing IP addresses of interface table
    {
        //Get parameters and initialize variables
        stopTime = par("stopTime");
        numSessions = par("numSessions");
        numSent = 0;
        numReceived = 0;
        numSentInbyte = 0;
        numSentInPacket = 0;
        numReceivedInbyte = 0;
        numReceivedInPacket = 0;
        goodputRatio = 0;
        averageendToEndDelay = 0;
        maxInterval = 1;
        intvlStartTime = intvlLastPkTime = 0;
        intvlDelay = 0;
        intvlNumPackets = 0;

        WATCH(numSent);
        WATCH(numReceived);
        WATCH(numSentInbyte);
        WATCH(numReceivedInbyte);
        WATCH(numSentInPacket);
        WATCH(numReceivedInPacket);
        WATCH(goodputRatio);
        WATCH(averageendToEndDelay);

        averageEndToEndDelayVector.setName("averageEndToEnd (sec)");
        averageEndToEndDelayIntervalVector.setName("averageEndToEndInIntrvl (sec)");
        goodputRatioVector.setName("goodputRatio (%)");


        //Excluded addresses (from the traffic generation)
        const char *excludedAddrs = par("excludedAddresses");
        cStringTokenizer tokenizer(excludedAddrs);
        const char *token;
        while ((token = tokenizer.nextToken())!=NULL)
            //excludedAddresses.push_back(IPvXAddressResolver().resolve(token).get4());
            excludedAddresses.push_back(L3AddressResolver().resolve(token).toIPv4());

        //Host weights (for host whose weight is not 1)
        const char *hWeights = par("hostsWeights");
        tokenizer = cStringTokenizer(hWeights);
        while ((token = tokenizer.nextToken())!=NULL)
        {
            HostWeight newWeight;
            //newWeight.ipAddress = IPvXAddressResolver().resolve(token).get4();
            newWeight.ipAddress = L3AddressResolver().resolve(token).toIPv4();
            if((token = tokenizer.nextToken())==NULL)
                error("'hostsWeights' parameter format is not correct!");
            newWeight.weight = atoi(token);
            hostsWeights.push_back(newWeight);
        }

        //Extract topology into the cTopology object, then fill in the vectors: NodeInfoVector and HostInfoVector
        extractTopology();

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
    topo.extractByProperty("networkNode"); // topo.extractByProperty("node");
    cModule *mod = topo.getNode(0)->getModule();


    // fill in NodeInfoVector (isHost and names) and HostInfoVector (IP and MAC addresses)
    nodeInfo.resize(topo.getNumNodes());
    unsigned int nAdhoc = 0;

    for (unsigned int i=0; i<topo.getNumNodes(); i++)
    {
        EV << "    Node #" << i << ":" <<endl;
        cModule *mod = topo.getNode(i)->getModule();
        nodeInfo[i].nedTypeName = std::string(mod->getNedTypeName()); //returns the ned type name
        nodeInfo[i].fullName = std::string(mod->getFullName()); //getFullName() or getName() returns the name assigned in the topology (such as host1, switch1, etc...)
        EV << "      Ned type: " << nodeInfo[i].nedTypeName << "; Name: " << nodeInfo[i].fullName <<endl;

        //isAdhoc
        if (nodeInfo[i].nedTypeName.find("Adhoc") != std::string::npos) //such as: AdhocHost, AdhocHostAPB,...
        {
            nodeInfo[i].isAdhoc = true;
            //Add element to adhocInfo vector
            AdhocInfo newAdhoc;
            //IInterfaceTable *ift = IPvXAddressResolver().interfaceTableOf(mod);
            IInterfaceTable *ift = L3AddressResolver().interfaceTableOf(mod);

            int nInterfaces = ift->getNumInterfaces();
            if(nInterfaces > 2) //If host has more than 2 interfaces...
                error("The host has more than 2 interfaces (one connected and loopback) and that's still not implemented!");
            for (unsigned int k=0; k<nInterfaces; k++)
            {
                InterfaceEntry *ie = ift->getInterface(k);
                //We add only the info about the entry which is not the loopback
                if (!ie->isLoopback())
                {
                    newAdhoc.fullName = nodeInfo[i].fullName;
                    newAdhoc.ipAddress = ie->ipv4Data()->getIPAddress();
                    newAdhoc.macAddress = ie->getMacAddress();
                    //If all Adhoc host have not udpGen, an error will occur in next line
                    newAdhoc.pUdpFlowHost = check_and_cast<UDPFlowHost *>(mod->getSubmodule("udpGen")); //newAdhoc.pUdpFlowHost = (UDPFlowHost *)mod->getSubmodule("udpGen");
                    //newAdhoc.pARPnew = check_and_cast<ARPNew *>(mod->getSubmodule("networkLayer")->getSubmodule("arp"));
                    newAdhoc.weight = getHostWeight(newAdhoc.ipAddress);
                    newAdhoc.nFlowSource = 0;
                    newAdhoc.nFlowDestination = 0;
                    newAdhoc.averageSizeSource = 0;
                    newAdhoc.averageSizeDestination = 0;
                    EV << "        " << newAdhoc.fullName << "-> IP: " << newAdhoc.ipAddress << "; MAC: " << newAdhoc.macAddress << "; weight: " << newAdhoc.weight <<endl;
                    nAdhoc++;
                }
            }

            //Before adding it to the hostInfo vector, exclude it if it is in the excludedAddresses vector
            bool isExcluded = false;
            for (unsigned int k=0; k<excludedAddresses.size(); k++)
            {
                // if (excludedAddresses[k].equals(newHost.ipAddress))
                if (excludedAddresses[k]==newAdhoc.ipAddress)
                {
                    isExcluded = true;
                    EV << "          ...not included!" <<endl;
                    break;
                }
            }
            if(!isExcluded)
                adhocInfo.push_back(newAdhoc);

        }
    }

    unsigned int n = adhocInfo.size();
    EV << "  and " << n << "(active)/" << nAdhoc << "(total) of those nodes are Adhoc node" << endl;
    //Finally, we update the number of hosts value in the UDPFlowHost module at each host
    for(unsigned int i=0; i<n; i++)
        adhocInfo[i].pUdpFlowHost->updateHostsInfo(n, stopTime);

    EV << "<-FlowGeneratorBase::extractTopology()" << endl;
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
    unsigned int nAdhoc = adhocInfo.size();
    std::vector<unsigned int> hostIndexes;
    for (unsigned int i=0; i<nAdhoc; i++)
        for (unsigned int j=0; j<adhocInfo[i].weight; j++)
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
            if((simTime()+(double)par("sessionGap") <= stopTime) && (numSent < numSessions))
            {
                scheduleAt(simTime()+(double)par("sessionGap"), msg); //Next generation
            }

            else
                delete msg; //Simulation ended -> delete message associated to flow generation
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

    //if (ev.isGUI())
    if (hasGUI())
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

void FlowGeneratorBase::accumulateReceivedData(simtime_t now, simtime_t endToEndDelay, unsigned long long numReceivedInPacket, unsigned long long numReceivedInbyte)
{
    Enter_Method("FlowGeneratorBase::accumulateReceivedData()");

    if (this->numSentInbyte != 0)
    {
        goodputRatio = (double) this->numReceivedInbyte / this->numSentInbyte * 100;
        goodputRatioVector.record(goodputRatio);
    }
    //this->endToEndDelay = this->endToEndDelay + sumDelay / numReceivedInPacket;
    this->averageendToEndDelay = (this->averageendToEndDelay * this->numReceivedInPacket + endToEndDelay.dbl() * numReceivedInPacket)/(this->numReceivedInPacket + numReceivedInPacket);

    this->numReceivedInbyte += numReceivedInbyte;
    this->numReceivedInPacket += numReceivedInPacket;

    averageEndToEndDelayVector.record(this->averageendToEndDelay);

    //statistics of interval

    if (now - intvlStartTime >= maxInterval)
        beginNewInterval(now, endToEndDelay, numReceivedInPacket);

    intvlNumPackets += numReceivedInPacket;
    intvlLastPkTime = now;
    intvlDelay = endToEndDelay.dbl(); // note that endToEndDelay is average delay of numReceivedPacket
}

void FlowGeneratorBase::beginNewInterval(simtime_t now, simtime_t endToEndDelay, unsigned long long numReceivedPacket)
{
    simtime_t duration = now - intvlStartTime;

    // record measurements

    if (intvlNumPackets !=0)
        intvlDelay = (intvlDelay * intvlNumPackets + endToEndDelay.dbl() * numReceivedPacket) / (intvlNumPackets + numReceivedPacket);
    else
        intvlDelay += endToEndDelay.dbl(); // note that endToEndDelay is average delay of numReceivedPacket

        averageEndToEndDelayIntervalVector.recordWithTimestamp(intvlStartTime, intvlDelay);

    // restart counters
    intvlStartTime = now;    // FIXME this should be *beginning* of tx of this packet, not end!
    intvlNumPackets = 0;
}

void FlowGeneratorBase::accumulateSentData(unsigned long long numSentInPacket, unsigned long long numSentInbyte)
{
    Enter_Method("FlowGeneratorBase::accumulateSentData()");
    this->numSentInbyte += numSentInbyte;
    this->numSentInPacket += numSentInPacket;

    if (this->numSentInbyte != 0)
    {
        goodputRatio = (double) this->numReceivedInbyte / this->numSentInbyte * 100;
        goodputRatioVector.record(goodputRatio);
    }

}

void FlowGeneratorBase::finish()
{
    EV << "->FlowGeneratorBase::finish()" << endl;

    recordScalar("total generated sessions", numSent);
    recordScalar("total sent bytes", numSentInbyte);
    recordScalar("total sent packets", numSentInPacket);
    recordScalar("total received bytes", numReceivedInbyte);
    recordScalar("total sent packets", numReceivedInPacket);
    recordScalar("goodput ratio", goodputRatio);
    recordScalar("average end to end delay", averageendToEndDelay);


    //Print statistics...
    EV << "  Printing some statistics..." << endl;
    EV << "    #Sent = " << numSent << " ; #Received = " << numReceived << endl;
    for(unsigned int i=0; i<adhocInfo.size(); i++)
    {
        EV << "      Adhoc Host '" << adhocInfo[i].fullName << "' -> " << adhocInfo[i].macAddress << " [" << adhocInfo[i].ipAddress << "]" << endl;
        EV << "        # Flow source = " << adhocInfo[i].nFlowSource << " [Average size = " << adhocInfo[i].averageSizeSource << "(KB)]" << endl;
        EV << "        # Flow destination = " << adhocInfo[i].nFlowDestination << " [Average size = " << adhocInfo[i].averageSizeDestination << "(KB)]" << endl;
        //EV << "        Average traffic offered to the network (as source) = " << (hostInfo[i].nFlowSource*hostInfo[i].averageSizeSource*8)/stopTime.dbl() << "(Kbps)" << endl; //###Esta estadística considera que se ha enviado todo el tráfico, pero probablemente no, porque en simTime (aprox) se para todo
    }
    /*EV << "    Generated flows..." << endl;
    for(unsigned int i=0; i<generatedFlows.size(); i++)
    {
        EV << "      " << generatedFlows[i] << endl;
    }*/

}


} // nemespace wapb
