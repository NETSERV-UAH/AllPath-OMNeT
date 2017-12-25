package arppathbridgesunidireccional.src.topologies;
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

import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import arppathbridgesunidireccional.src.nodes.ethernet.EtherSwitchAPB;
import libgist.src.nodes.inet.StandardHostNew;
import libgist.src.simulationmodels.flowmodels.UDPFlowGenerator;
import ned.DatarateChannel;

network triangle_pruebaGenerator
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
            	@display("p=169,148");
            gates:
                ethg[3];
        }
        switchB: EtherSwitchAPB {
            parameters:
            	@display("p=389,148");
            gates:
                ethg[3];
        }
        switchC: EtherSwitchAPB {
            parameters:
            	@display("p=290,335");
            gates:
                ethg[3];
        }
        host1: StandardHostNew {
            @display("p=52,149");
        }
        host2: StandardHostNew {
            @display("p=512,149");
        }
        host3: StandardHostNew {
            @display("p=290,411");
        }
        generator: UDPFlowGenerator {
            @display("p=169,41");
        }
    connections:

        switchA.ethg[1] <--> ethline <--> switchB.ethg[1];
        switchC.ethg[1] <--> ethline <--> switchA.ethg[2];
        switchC.ethg[2] <--> ethline <--> switchB.ethg[2];
        host1.ethg++ <--> ethline <--> switchA.ethg[0];
        host2.ethg++ <--> ethline <--> switchB.ethg[0]; 
        host3.ethg++ <--> ethline <--> switchC.ethg[0];
}
