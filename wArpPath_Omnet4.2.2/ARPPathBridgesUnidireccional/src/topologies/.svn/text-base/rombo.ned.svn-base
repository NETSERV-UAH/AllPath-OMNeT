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

network rombo
{
    @display("bgb=601,449");
    types:
        channel fiberline extends DatarateChannel
        {
            delay = 1us;
            datarate = 512Mbps;
        }
        channel ethline extends DatarateChannel
        {
            delay = 50ns;
            datarate = 100Mbps;
        }
    submodules:
        configurator: FlatNetworkConfigurator {
            @display("p=60,41");
        }
        switchA: EtherSwitchAPB {
            parameters:
            	@display("p=150,200");
            gates:
                ethg[3];
        }
        switchB: EtherSwitchAPB {
            parameters:
            	@display("p=350,200");
            gates:
                ethg[4];
        }
        switchC: EtherSwitchAPB {
            parameters:
            	@display("p=250,100");
            gates:
                ethg[2];
        }
        switchD: EtherSwitchAPB {
            parameters:
            	@display("p=250,300");
            gates:
                ethg[2];
        }
        host1: StandardHostNew {
            @display("p=50,200");
        }
        host2: StandardHostNew {
            @display("p=500,150");
        }
        host3: StandardHostNew {
            @display("p=500,250");
        }
    connections:

        switchA.ethg[0] <--> ethline <--> switchC.ethg[0];
        switchA.ethg[1] <--> ethline <--> switchD.ethg[0];
        switchB.ethg[0] <--> ethline <--> switchC.ethg[1];
        switchB.ethg[1] <--> ethline <--> switchD.ethg[1];
        host1.ethg++ <--> ethline <--> switchA.ethg[2];
        host2.ethg++ <--> ethline <--> switchB.ethg[2]; 
        host3.ethg++ <--> ethline <--> switchB.ethg[3];
}
