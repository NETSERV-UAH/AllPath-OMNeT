//
// Copyright (C) 2014 Isaias Martinez Yelmo (Inet 2.2.0 and Inet 2.3.0 adaptation)
// Copyright (C) 2012 Elisa Rojas (updated to inet2.0.0, although the only difference is the use of ARPPacketRoute instead of ARPPacket)
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

#ifndef __INET_IPNEW_H
#define __INET_IPNEW_H

#include "INETDefs.h"

#include "IPv4.h"

#ifdef WITH_MANET
#include "ControlManetRouting_m.h"
#endif

class ARPPacketRoute; //EXTRA

class INET_API IPv4New: public IPv4 {

/* EXTRA-IMY functions modified by libgist */

 protected:

    virtual void handleIncomingARPPacket(ARPPacketRoute *packet, const InterfaceEntry *fromIE);


 protected:
    virtual void endService(cPacket *msg);

 /* EXTRA-IMY */

};

#endif

