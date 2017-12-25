//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2004,2011 Andras Varga
// Copyright (C) 2010 Diego Rivera (added startTime, stopTime, numberOfMessages)
// Copyright (C) 2012 Elisa Rojas (updated to inet2.0.0, which already has startTime, stopTime implemented... messageFreq is now sendInterval)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "UDPBasicAppNew.h"
#include "UDPControlInfo_m.h"
#include "IPvXAddressResolver.h"
#include "InterfaceTableAccess.h"


Define_Module(UDPBasicAppNew);

int UDPBasicAppNew::counter;
simsignal_t UDPBasicAppNew::sentPkSignal = SIMSIGNAL_NULL;
simsignal_t UDPBasicAppNew::rcvdPkSignal = SIMSIGNAL_NULL;

void UDPBasicAppNew::initialize(int stage)
{
    // because of IPvXAddressResolver, we need to wait until interfaces are registered,
    // address auto-assignment takes place etc.
    if (stage != 3)
        return;

    counter = 0;
    numSent = 0;
    numReceived = 0;
    WATCH(numSent);
    WATCH(numReceived);
    sentPkSignal = registerSignal("sentPk");
    rcvdPkSignal = registerSignal("rcvdPk");

    localPort = par("localPort");
    destPort = par("destPort");

    const char *destAddrs = par("destAddresses");
    cStringTokenizer tokenizer(destAddrs);
    const char *token;

    while ((token = tokenizer.nextToken()) != NULL)
        destAddresses.push_back(IPvXAddressResolver().resolve(token));

    socket.setOutputGate(gate("udpOut"));
    socket.bind(localPort);
    setSocketOptions();

    if (destAddresses.empty())
        return;

    stopTime = par("stopTime").doubleValue();
    simtime_t startTime = par("startTime").doubleValue();
    if (stopTime != 0 && stopTime <= startTime)
        error("Invalid startTime/stopTime parameters");

    cMessage *timerMsg = new cMessage("sendTimer");
    scheduleAt(startTime, timerMsg);

    numberOfMessages = par("numberOfMessages");
}

void UDPBasicAppNew::finish()
{
    recordScalar("packets sent", numSent);
    recordScalar("packets received", numReceived);
}

void UDPBasicAppNew::setSocketOptions()
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
}

IPvXAddress UDPBasicAppNew::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    return destAddresses[k];
}

cPacket *UDPBasicAppNew::createPacket()
{
    char msgName[32];
    sprintf(msgName, "UDPBasicAppData-%d", counter++);

    cPacket *payload = new cPacket(msgName);
    payload->setByteLength(par("messageLength").longValue());
    return payload;
}

void UDPBasicAppNew::sendPacket()
{
    cPacket *payload = createPacket();
    IPvXAddress destAddr = chooseDestAddr();

    emit(sentPkSignal, payload);
    EV << "payload->getBitLength(): " << payload->getBitLength() << endl; //Debug ###ERS###
    socket.sendTo(payload, destAddr, destPort);
    numSent++;
}

void UDPBasicAppNew::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        // send, then reschedule next sending
        sendPacket();
        simtime_t d = simTime() + par("sendInterval").doubleValue();
        if ((stopTime == 0 || d < stopTime) && (numSent <= numberOfMessages))
            scheduleAt(d, msg);
        else
            delete msg;
    }
    else if (msg->getKind() == UDP_I_DATA)
    {
        // process incoming packet
        processPacket(PK(msg));
    }
    else if (msg->getKind() == UDP_I_ERROR)
    {
        EV << "Ignoring UDP error report\n";
        delete msg;
    }
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
}

void UDPBasicAppNew::processPacket(cPacket *pk)
{
    emit(rcvdPkSignal, pk);
    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
    numReceived++;
}

