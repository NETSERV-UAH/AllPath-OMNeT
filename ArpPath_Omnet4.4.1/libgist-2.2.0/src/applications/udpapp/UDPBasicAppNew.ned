// Copyright (C) 2014 Isaias Martinez Yelmo (Inet 2.2.0 and Inet 2.3.0 adaptation)
// Copyright (C) 2012 Elisa Rojas (updated to inet2.0.0, which already has startTime, stopTime implemented... messageFreq is now sendInterval)
// Copyright (C) 2010 Diego Rivera (added startTime, stopTime, numberOfMessages)
// Copyright (C) 2004 Andras Varga
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
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

package libgist.src.applications.udpapp;

import inet.applications.udpapp.UDPBasicApp;


//
// Sends UDP packets to the given IP address at the given interval.
// Compatible with both ~IPv4 and ~IPv6.
//
// The sending interval can be a constant or a random value (e.g. exponential(1)).
// If the destAddresses parameter contains more than one address, one
// of them is randomly chosen for each packet. An address may be given in the
// dotted decimal notation, or with the module name. (The IPvXAddressResolver
// class is used to resolve the address.) To disable the model, set
// destAddresses to "".
//
// Received packets are discarded.
//
// The peer can be a ~UDPSink, another ~UDPBasicApp (it handles received packets
// like ~UDPSink), or a ~UDPEchoApp. When used with ~UDPEchoApp, the rcvdPkLifetime
// statistic will contain the round-trip times.
//
simple UDPBasicAppNew extends UDPBasicApp
{
    parameters:

		@class(UDPBasicAppNew);
        int numberOfMessages = default(10000000); //total traffic to send by the application //D. Rivera. // EXTRA-IMY

}



