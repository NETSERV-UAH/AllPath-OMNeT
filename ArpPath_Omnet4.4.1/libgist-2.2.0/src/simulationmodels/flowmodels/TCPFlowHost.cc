//
// Copyright (C) 05/2013 Elisa Rojas
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

#include "TCPFlowHost.h"

Define_Module(TCPFlowHost);

int TCPFlowHost::counter;
simsignal_t TCPFlowHost::sentPkSignal = SIMSIGNAL_NULL;
simsignal_t TCPFlowHost::rcvdPkSignal = SIMSIGNAL_NULL;

TCPFlowHost::TCPFlowHost()
{

}

TCPFlowHost::~TCPFlowHost()
{
    for(unsigned int i=0; i<flowMessages.size(); i++)
        cancelAndDelete(flowMessages[i]);
}

void TCPFlowHost::initialize(int stage)
{
    //Initializes at the same stage that the UDPFlowGenerator module
    if(stage == 3)
    {
        counter = 0;
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);
        sentPkSignal = registerSignal("sentPk");
        rcvdPkSignal = registerSignal("rcvdPk");
    }
}

void TCPFlowHost::startFlow(unsigned int transferRate, unsigned long long flowSize, unsigned int frameSize, const IPvXAddress& destAddr) //Kbps, B(KB*1000), B, address
{
    /*Since this module function is called from another - UDPFlowGenerator - we need to indicate it via Enter_Method
     * or Enter_Method_Silent - with no animation -
     * More about this at: http://www.omnetpp.org/doc/omnetpp/manual/usman.html#sec182 (4.12 Direct Method Calls Between Modules at the Omnet++ manual) */
    Enter_Method("TCPFlowHost::startFlow()");

    EV << "->TCPFlowHost::startFlow()" << endl;
    EV << "  Flow info received at host " << info() << "!" << endl;
    EV << "    [" << info() << " -> " << destAddr << "]" << endl;
    EV << "    Transfer Rate: " << transferRate << " (Kbps); Flow Size: " << flowSize/1000 << " (KB); Frame Size: " << frameSize << " (B)" << endl;

    //Now we check in there's already a flow started for that destination and add only the size (not transfer rate or frame size)
    //or not, or if we need to start a new flow because that destination is still not registered at flowInfo
    int destIndex=-1, nextFreeIndex=-1, i=0;
    for(i=0; i<flowInfo.size(); i++)
    {
        //If we find the destination address, we save its index
        if(flowInfo[i].destAddress == destAddr)
            destIndex = i;

        //Meanwhile, we save the next free entry
        if(nextFreeIndex== -1 && flowInfo[i].localPort == 0) //The entry is free if localPort (and destPort) == 0
            nextFreeIndex = i;
    }

    //If we have already registered the destination address..
    i=0;
    if (destIndex!=-1)
    {
        EV << "  The destination was already registered!" << endl;
        i = destIndex;
        if(flowInfo[i].flowSize == 0) //If flow is inactive, we active it
        {
            //Register the flow
            flowInfo[i].transferRate = transferRate;
            flowInfo[i].flowSize = flowSize;
            flowInfo[i].frameSize = frameSize;

            //Start the handler to generate the flow traffic
            if(frameSize > flowSize) frameSize = flowSize; //Should never happen (with JAC's model), but just in case it happens //###
            double startTime = double(frameSize*8)/(transferRate*1000); //(B*8)/(Kbps*1000)
            char indexName[6]; indexName[0] = 'G'; indexName[1] = 'e'; indexName[2] = 'n'; indexName[3] = '-';
            indexName[4] = i; indexName[5] = 0; //#i \x0i\x00 (cMessage copia la cadena y la hace 'const')
            cMessage *timer = new cMessage(indexName); //To distinguish events, we pass the index as the name of the timer
            if(simTime()+startTime <= stopTime)
            {
                scheduleAt(simTime()+startTime, timer);
                EV << "    A new flow starts at T=" << simTime()+startTime << " (now T=" << simTime() << ")" << endl;
                flowMessages.push_back(timer);
            }
            else
                EV << "    A new flow will not start at T=" << simTime()+startTime << " > stopTime = " << stopTime << endl;
        }
        else //Otherwise, we add more traffic to it
        {
            //Add more traffic to the existing flow, but the frame size or transfer rate will not change
            flowInfo[i].flowSize = flowInfo[i].flowSize + flowSize;
            //The handler is already active... no need to scheduleAt...
            EV << "    The flow was already running, we just added " << flowSize << "(KB)"<< endl;
        }
    }
    //If not, we register it
    else
    {
        EV << "  The destination was not registered at this source!" << endl;
        i = nextFreeIndex;

        //Assign port numbers
        flowInfo[i].localPort = 1000+nextFreeIndex; //From 1000 to 1999
        flowInfo[i].destPort = 2000+nextFreeIndex; //From 2000 to 2999

        //Create socket and bind to localPort
//        flowInfo[i].socket.setOutputGate(gate("udpOut"));
//        flowInfo[i].socket.bind(flowInfo[i].localPort);
//        setSocketOptions(flowInfo[i].socket);

        //Register the flow
        flowInfo[i].destAddress = destAddr;
        flowInfo[i].transferRate = transferRate;
        flowInfo[i].flowSize = flowSize;
        flowInfo[i].frameSize = frameSize;

        //Start the handler to generate the flow traffic
        if(frameSize > flowSize) frameSize = flowSize; //Should never happen, but just in case it happens //###
        double startTime = double(frameSize*8)/(transferRate*1000); //(B*8)/(Kbps*1000)
        char indexName[6]; indexName[0] = 'G'; indexName[1] = 'e'; indexName[2] = 'n'; indexName[3] = '-';
        indexName[4] = i; indexName[5] = 0; //#i \x0i\x00 (cMessage copia la cadena y la hace 'const')
        cMessage *timer = new cMessage(indexName); //To distinguish events, we pass the index as the name of the timer
        if(simTime()+startTime <= stopTime)
        {
            scheduleAt(simTime()+startTime, timer);
            EV << "    A new flow starts at T=" << simTime()+startTime << " (now T=" << simTime() << ")" << endl;
//TODO:            EV << "      #" << i << "-> TCPClientId: " << flowInfo[i].client.getId() << "; Local port: " << flowInfo[i].localPort << "; Destination port: " << flowInfo[i].destPort << endl;
            flowMessages.push_back(timer);
        }
        else
            EV << "    A new flow will not start at " << simTime()+startTime << " > stopTime = " << stopTime << endl;
    }

    EV << "<-TCPFlowHost::startFlow()" << endl;
}

/*cPacket *TCPFlowHost::createPacket(unsigned int frameSize)
{
    char msgName[32];
    sprintf(msgName,"TCPFlowHost-%d", counter++);

    cPacket *payload = new cPacket(msgName);
    payload->setByteLength(frameSize);
    return payload;
}

void TCPFlowHost::sendPacket(unsigned int frameSize, unsigned int nFlow)
{
    //Generate payload and send it via UDP
    cPacket *payload = createPacket(frameSize);

    emit(sentPkSignal, payload);
    flowInfo[nFlow].socket.sendTo(payload, flowInfo[nFlow].destAddress, flowInfo[nFlow].destPort);
    numSent++;
}*/

void TCPFlowHost::handleMessage(cMessage *msg)
{
    EV << "->TCPFlowHost::handleMessage()" << endl;
    if (msg->isSelfMessage())
    {
        deleteMessageFromVector(msg); //Delete it from the pending messages vector

        // send, then reschedule next sending
        if (simTime()<=stopTime)
        {
            std::string indexName = msg->getName(); //From const char * to string (not possible to char *)
            unsigned char c = indexName[4]; //The char #5 (index 4) of the chain indicates the flowInfo index
            unsigned int i = c; //First from char to unsigned char and then to unsigned int (otherwise, the 'i' could get a wrong value)
            EV << "  Generating a new UDP packet for flow #" << i+1 << endl;

            int transferRate = flowInfo[i].transferRate;
            long long flowSize = flowInfo[i].flowSize;
            int frameSize = flowInfo[i].frameSize;

            //Send next frameSize(B) UDP packet and substract it from the flow size (total)
            if(frameSize > flowSize) frameSize = flowSize; //Unlikely, but just in case it happens
//            sendPacket(frameSize, i); //Send packet of size 'frameSize' and flow parameters at vector position 'i'
            flowInfo[i].flowSize = flowSize - frameSize;
            EV << "    " << frameSize << "/" << flowSize << "(B) at rate " << transferRate << ")" << endl;

            //If flow has not finished, reschedule
            if(flowInfo[i].flowSize > 0)
            {
                double nextTime = double(frameSize*8)/(transferRate*1000); //(B*8)/(Kbps*1000)
                if(simTime()+nextTime <= stopTime)
                {
                    scheduleAt(simTime()+nextTime, msg);
                    EV << "    Next UDP packet at T=" << simTime()+nextTime << " (now T=" << simTime() << ")" << endl;
                    flowMessages.push_back(msg);
                }
                else
                    EV << "    No next UDP packet at T=" << simTime()+nextTime << " > stopTime = " << stopTime << endl;
            }
            else
            {
                EV << "    Flow ended at T=" << simTime() << "!" << endl;
            }
        }
        else
        {
            EV << "  Host stopped because traffic generator ended! At " << simTime() << " with stop time T=" << stopTime << endl;
        }
    } //TODO: AÑADIR LOS 'delete' de UDP... y otras posibles actualizaciones.......
/*    else if (msg->getKind() == UDP_I_DATA)
    {
        // process incoming packet
//        processPacket(PK(msg));
    }
    else if (msg->getKind() == UDP_I_ERROR)
    {
        EV << "Ignoring UDP error report\n";
        delete msg;
    }*/
    else
    {
        error("Unrecognized message (%s)%s", msg->getClassName(), msg->getName());
    }

    if (ev.isGUI())
    {
        char buf[40];
        sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
        getDisplayString().setTagArg("t", 0, buf);
    }
    EV << "<-TCPFlowHost::handleMessage()" << endl;
}

/*void TCPFlowHost::processPacket(cPacket *pk)
{
    EV << "->TCPFlowHost::processPacket()" << endl;

    emit(rcvdPkSignal, pk);
    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
    numReceived++;
}

void TCPFlowHost::setSocketOptions(UDPSocket socket)
{
    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
        socket.setTimeToLive(timeToLive);

    int typeOfService = par("typeOfService");
    if (typeOfService != -1)
        socket.setTypeOfService(typeOfService);

    const char *multicastInterface = par("multicastInterface");
    if (multicastInterface[0])
    {
        IInterfaceTable *ift = InterfaceTableAccess().get(this);
        InterfaceEntry *ie = ift->getInterfaceByName(multicastInterface);
        if (!ie)
            throw cRuntimeError("Wrong multicastInterface setting: no interface named \"%s\"", multicastInterface);
        socket.setMulticastOutputInterface(ie->getInterfaceId());
    }

    bool receiveBroadcast = par("receiveBroadcast");
    if (receiveBroadcast)
        socket.setBroadcast(true);

    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");
    if (joinLocalMulticastGroups)
        socket.joinLocalMulticastGroups();
}*/

void TCPFlowHost::deleteMessageFromVector(cMessage *msg)
{
    unsigned int i=0;
    for(i=0; i<flowMessages.size(); i++)
        if(flowMessages[i] == msg)
            break;
    if(i<flowMessages.size())
        flowMessages.erase(flowMessages.begin()+i);
}

void TCPFlowHost::updateHostsInfo(unsigned int n, simtime_t stop)
{
    EV << "->TCPFlowHost::updateHostsInfo()" << endl;
    EV << "  Number of active hosts in the topology = " << n << " and stop time T=" << stop << endl;

    //Updates the number of hosts
    //TODO: This method considers it is called just once (at the beginning of the simulation), since the topology cannot change in real time (right now March'12)
    nHosts = n; //Update number of hosts
    flowInfo.resize(n-1); //n hosts, so n-1 possible destinations

    //Update vector values
    for(unsigned int i=0; i<n-1; i++)
    {
//TODO:        flowInfo[i].client = TCPBasicClientApp();
        flowInfo[i].localPort = 0; //From 1000 to 1999 - initialized when the destination address is assigned
        flowInfo[i].destPort = 0;  //From 2000 to 2999 - initialized when the destination address is assigned
        flowInfo[i].transferRate = 0;
        flowInfo[i].flowSize = 0;
        flowInfo[i].frameSize = 0;
    }

    //Update the stop time (it is defined by the generator (global for the whole topology))
    stopTime = stop;
    EV << "<-TCPFlowHost::updateHostsInfo()" << endl;
}

void TCPFlowHost::finish()
{
    EV << "->TCPFlowHost::finish()" << endl;

    recordScalar("packets sent", numSent);
    recordScalar("packets received", numReceived);

    //Print statistics...
    EV << "  Printing some statistics..." << endl;
    EV << "    Counter = " << counter << "; #Sent = " << numSent << " ; #Received = " << numReceived << endl;
    EV << "    Traffic offered to the network (as source) = " << (numSent*500*8)/stopTime.dbl()/1000 << "(Kbps)" << endl; //###Está puesto fijo con tramas de 500B (cambiar cuando el tamaño de trama sea variable)
    EV << "    Flow Info Vector:" << endl;
    for(unsigned int i=0; i<flowInfo.size(); i++)
    {
        if(!flowInfo[i].destAddress.isUnspecified()) //If destination address is not unspecified, do not print
        {
            EV << "      Flow #" << i+1 << ":" << endl;
            EV << "        Local port: " << flowInfo[i].localPort << "; Destination port: " << flowInfo[i].destPort << endl;
            EV << "        Destination address: " << flowInfo[i].destAddress << endl;
        }
    }

    //Print pending scheduled messages (if any)
    EV << "Pending messages: " << flowMessages.size() << endl;
    for(unsigned int i=0; i<flowMessages.size(); i++)
        EV << "  " << flowMessages[i] << endl;
}

