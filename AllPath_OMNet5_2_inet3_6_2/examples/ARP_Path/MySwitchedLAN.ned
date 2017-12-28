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

package allpath.examples.ARP_Path;

import inet.common.scenario.ScenarioManager;
import inet.examples.httptools.socket.tenserverssocket.ethernetline;
//import inet.common.lifecycle.LifecycleController;
//import inet.linklayer.configurator.L2NetworkConfigurator;
//import inet.linklayer.ieee8021d.tester.STPTester;
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import allpath.src.node.ethernet.EtherSwitchAPB;
import inet.node.inet.StandardHost;
//import inet.node.ethernet.EtherHost;

import ned.DatarateChannel;
import inet.common.misc.ThruputMeteringChannel;

network MySwitchedLAN
{
    @display("bgb=693,352");
    types:
        channel C extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 100Mbps;
        }
    submodules:
        client: StandardHost {
            @display("p=158,310;i=device/laptop_l");
            gates:
                ethg[1];
        }
        host1: StandardHost {
            @display("p=53,303;i=device/pc");
            gates:

                ethg[1];

        }
        host2: StandardHost {
            @display("p=102,310;i=device/pc");
            gates:
                ethg[1];


        }

        switchA: EtherSwitchAPB {
            parameters:
                @display("p=89,229");
            gates:
                ethg[5];
        }
        switchB: EtherSwitchAPB {
            parameters:
                @display("p=44,137");
            gates:
                ethg[4];
        }
        switchC: EtherSwitchAPB {
            parameters:
                @display("p=148,137");
            gates:
                ethg[4];
        }
        switchD: EtherSwitchAPB {
            parameters:
                @display("p=102,23");
            gates:
                ethg[4];
        }
        switchE: EtherSwitchAPB {
            parameters:
                @display("p=347,23");
            gates:
                ethg[4];
        }
        switchF: EtherSwitchAPB {
            parameters:
                @display("p=551,23");
            gates:
                ethg[4];
        }
        switchG: EtherSwitchAPB {
            parameters:
                @display("p=495,137");
            gates:
                ethg[4];
        }
        switchH: EtherSwitchAPB {
            parameters:
                @display("p=607,137");
            gates:
                ethg[4];
        }
        switchI: EtherSwitchAPB {
            parameters:
                @display("p=562,215");
            gates:
                ethg[3];
        }
        server: StandardHost {
            @display("p=562,303;i=device/server_l");
            gates:

                ethg[1];


        }
        configurator: IPv4NetworkConfigurator {
            @display("p=638,69");
        }
        //  l2NetworkConfigurator: L2NetworkConfigurator {
        //    @display("p=799.90625,377.01877");
        //}
        //  lifecycleController: LifecycleController {
        //    @display("p=130.89375,402.75");
        //}
        //       stpTester: STPTester {
        //         @display("p=848.0125,328.9125");
        //   }
        scenarioManager: ScenarioManager {
            @display("p=638,14");
        }


    connections:
        host1.ethg[0] <--> C <--> switchA.ethg[0];
        host2.ethg[0] <--> C <--> switchA.ethg[1];
        client.ethg[0] <--> C <--> switchA.ethg[2];

        switchA.ethg[3] <--> C <--> switchB.ethg[0];
        switchA.ethg[4] <--> C <--> switchC.ethg[0];

        server.ethg[0] <--> C <--> switchI.ethg[0];

        switchI.ethg[1] <--> C <--> switchH.ethg[0];
        switchI.ethg[2] <--> C <--> switchG.ethg[0];

        switchB.ethg[1] <--> C <--> switchD.ethg[0];
        switchB.ethg[2] <--> C <--> switchE.ethg[0];
        switchB.ethg[3] <--> C <--> switchF.ethg[0];

        switchC.ethg[1] <--> C <--> switchD.ethg[1];
        switchC.ethg[2] <--> C <--> switchE.ethg[1];
        switchC.ethg[3] <--> C <--> switchF.ethg[1];

        switchG.ethg[1] <--> C <--> switchD.ethg[2];
        switchG.ethg[2] <--> C <--> switchE.ethg[2];
        switchG.ethg[3] <--> C <--> switchF.ethg[2];

        switchH.ethg[1] <--> C <--> switchD.ethg[3];
        switchH.ethg[2] <--> C <--> switchE.ethg[3];
        switchH.ethg[3] <--> C <--> switchF.ethg[3];
}


