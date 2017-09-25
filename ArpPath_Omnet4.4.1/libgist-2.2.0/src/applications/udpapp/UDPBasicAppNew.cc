//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2004,2011 Andras Varga
// Copyright (C) 2010 Diego Rivera (added startTime, stopTime, numberOfMessages)
// Copyright (C) 2012 Elisa Rojas (updated to inet2.0.0, which already has startTime, stopTime implemented... messageFreq is now sendInterval)
// Copyright (C) 2014 Isaias Martinez Yelmo (Inet 2.2.0 and Inet 2.3.0 adaptation)
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


void UDPBasicAppNew::initialize(int stage)
{
    ApplicationBase::initialize(stage); //EXTRA-IMY

    // because of IPvXAddressResolver, we need to wait until interfaces are registered,
    // address auto-assignment takes place etc.
    if (stage == 0)
    {
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);
        sentPkSignal = registerSignal("sentPk");
        rcvdPkSignal = registerSignal("rcvdPk");

        localPort = par("localPort");
        destPort = par("destPort");
        startTime = par("startTime").doubleValue();
        stopTime = par("stopTime").doubleValue();
        if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
            error("Invalid startTime/stopTime parameters");
        selfMsg = new cMessage("sendTimer");
        numberOfMessages = par("numberOfMessages"); // EXTRA-IMY
    }
}


void UDPBasicAppNew::processSend()
{
    sendPacket();
    simtime_t d = simTime() + par("sendInterval").doubleValue();
    if ((stopTime < SIMTIME_ZERO || d < stopTime) && (numSent <= numberOfMessages)) // EXTRA-IMY
    {
        selfMsg->setKind(SEND);
        scheduleAt(d, selfMsg);
    }
    else
    {
        selfMsg->setKind(STOP);
        scheduleAt(stopTime, selfMsg);
    }
}
