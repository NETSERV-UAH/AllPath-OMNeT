//
// Copyright (C) 2014 Isaias Martinez Yelmo (Inet 2.2.0 and Inet 2.3.0 adaptation)
// Copyright (C) 2012 Elisa Rojas (parameters, structures, functions and 'friendship' with UDPFlowGenerator to be able to save routes created with ARPPacketRoute, and updated to inet2.0.0, which already has the parameters for counting ARP messages)
// Copyright (C) 2010 Diego Rivera (parameter to show number of ARP recieved added)
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

package libgist.src.networklayer.arp;

import inet.networklayer.arp.ARP;

//
// Implements the Address Resolution Protocol for IPv4 and IEEE 802 6-byte
// MAC addresses.
//
// Expects ~IPv4RoutingDecision control info objects to be attached to packets
// coming from higher layers. ARP packets are represented by the ~ARPPacket
// class. Datagrams to be sent and incoming ARP packets are expected to
// on gate ipIn.
//
// ARP resolution is used over <i>broadcast</i> interfaces only,
// that is, over interfaces that have the <tt>isBroadcast()</tt> flag set in
// <tt>InterfaceEntry</tt> (see ~InterfaceTable). Since routing files
// (<tt>.irt</tt> or <tt>.mrt</tt> files, given as parameter to ~RoutingTable)
// may modify the default interface configuration, you must take care that
// these files don't contain the word <tt>BROADCAST</tt> e.g. for PPP
// interfaces.
//
simple ARPNew extends ARP
{
    parameters:
        @class(ARPNew); // EXTRA-IMY
}
