//
// Copyright (C) 2014 Isaias Martinez Yelmo (Inet 2.2.0 and Inet 2.3.0 adaptation)
// Copyright (C) 2004 Andras Varga
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


#include <stdlib.h>
#include <string.h>

#include "IPv4New.h"

#include "ARPPacketRoute_m.h" //EXTRA-IMY
#include "IARPCache.h"
#include "ICMPMessage_m.h"
#include "Ieee802Ctrl_m.h"
#include "IRoutingTable.h"
#include "InterfaceTableAccess.h"
#include "IPSocket.h"
#include "IPv4ControlInfo.h"
#include "IPv4Datagram.h"
#include "IPv4InterfaceData.h"
#include "NodeOperations.h"
#include "NodeStatus.h"
#include "NotificationBoard.h"


Define_Module(IPv4New);

void IPv4New::endService(cPacket *packet)
{
    if (!isUp) {
        EV << "IPv4 is down -- discarding message\n";
        delete packet;
        return;
    }
    if (packet->getArrivalGate()->isName("transportIn")) //TODO packet->getArrivalGate()->getBaseId() == transportInGateBaseId
    {
        handlePacketFromHL(packet);
    }
    else if (packet->getArrivalGate() == arpInGate)
    {
        handlePacketFromARP(packet);
    }
    else // from network
    {
        const InterfaceEntry *fromIE = getSourceInterfaceFrom(packet);
        if (dynamic_cast<ARPPacketRoute *>(packet)) //EXTRA
            handleIncomingARPPacket((ARPPacketRoute *)packet, fromIE); //EXTRA
        else if (dynamic_cast<IPv4Datagram *>(packet))
            handleIncomingDatagram((IPv4Datagram *)packet, fromIE);
        else
            throw cRuntimeError(packet, "Unexpected packet type");
    }

    if (ev.isGUI())
        updateDisplayString();
}

void IPv4New::handleIncomingARPPacket(ARPPacketRoute *packet, const InterfaceEntry *fromIE) // EXTRA-IMY
{
    // give it to the ARP module
    Ieee802Ctrl *ctrl = check_and_cast<Ieee802Ctrl*>(packet->getControlInfo());
    ctrl->setInterfaceId(fromIE->getInterfaceId());
    send(packet, arpOutGate);
}
