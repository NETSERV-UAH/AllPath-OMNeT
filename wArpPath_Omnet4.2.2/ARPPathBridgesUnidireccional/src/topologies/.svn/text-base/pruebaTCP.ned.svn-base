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

package arppathbridgesunidireccional.src.topologies;

import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import arppathbridgesunidireccional.src.nodes.ethernet.EtherSwitchAPB;
import libgist.src.nodes.inet.StandardHostNew;
import ned.DatarateChannel;


network pruebaTCP //cogido del examples/inet/tcpsack
{
    parameters:
        @display("bgb=400,200");
    submodules:
        switch: EtherSwitchAPB {
            @display("p=200,100");
        }
        client: StandardHostNew {
            @display("p=50,100");
        }
        server: StandardHostNew {
            @display("p=350,100;i=device/server");
        }
        configurator: FlatNetworkConfigurator {
            @display("p=81,27");
        }
    connections:
        client.ethg++ <--> NormalPath <--> switch.ethg++;
        switch.ethg++ <--> NormalPath <--> server.ethg++;
}

channel NormalPath extends DatarateChannel
{
    parameters:
        datarate = 100Mbps;
        delay = 0.0565us; // 10m
        per = 0;
        ber = 0;
}

