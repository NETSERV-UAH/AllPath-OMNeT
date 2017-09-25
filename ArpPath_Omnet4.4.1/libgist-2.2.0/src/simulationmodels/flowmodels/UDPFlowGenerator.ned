//
// Copyright (C) 03/2012 Elisa Rojas
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

package libgist.src.simulationmodels.flowmodels;

// This module is an UDP flow traffic generator. You can configure a generation frequency (lambda) and 
// this will determine every time a new flow is created by the generator. The generator operates
// with the UDPFlowHost modules at every host (StandardHostNew) in the network. Every time a new 
// flow is created, the generator indicates the source host to start the flow (and its parameters)

simple UDPFlowGenerator
{
    parameters:
        double startTime @unit("s") = default(0.2s); // time first generation begins		
        double stopTime @unit("s") = default(10000s); //time to stop generating traffic 
        double statsPeriod @unit("s") = default(0s); //period of time to print periodic statistics
        volatile double generatorFreq @unit("s"); // should usually be a random value, e.g. exponential(1)
        string excludedAddresses = default(""); // list of \IP addresses, separated by spaces, to be excluded from the traffic generation (both as source and destination)
        string hostsWeights = default(""); // list of \IP addresses and their weights (!=1 which is the default value), all separated by spaces
        string routeHosts = default(""); // list of couples of \IP addresses (src/dst), separated by spaces, to have their routes registered by the ARP
        string routeSwitches = default(""); //list of couples of switches (src/dst), separated by spaces, to have their routes (of the hosts they are serving) registered by the ARP
        bool dataCenterTraffic = default(false); // data center traffic (mice/elephant)
        @display("i=block/cogwheel_s");
        @labels(node);
}
