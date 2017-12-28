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

package allpath.examples.Report1;

import inet.common.scenario.ScenarioManager;
//import inet.common.lifecycle.LifecycleController;
import inet.linklayer.configurator.L2NetworkConfigurator;
//import inet.linklayer.ieee8021d.tester.STPTester;
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.StandardHost;
//import inet.node.ethernet.EtherHost;
import ned.DatarateChannel;
import inet.common.misc.ThruputMeteringChannel;

network test2
{
    @display("bgb=890.525,473.23126");
   // parameters:
     //   double edgeDatarate @unit(bps);
       // double coreDatarate @unit(bps);
    types:
        channel C extends ThruputMeteringChannel
        {
            delay = 2ms;
            datarate = 10 Mbps; //edgeDatarate;
            thruputDisplayFormat = "b B U";
        }
    submodules:
        client3: StandardHost {
            @display("p=65.932495,69.284996;i=device/pc");
            gates:
                ethg[1];
        }
        client2: StandardHost {
            @display("p=55.875,177.6825;i=device/pc");
            gates:

                ethg[1];

        }
        client5: StandardHost {
            @display("p=267.0825,31.289999;i=device/pc");
            gates:
                ethg[1];


        }
        client7: StandardHost {
            @display("p=373.245,355.365;i=device/pc");
            gates:
                ethg[1];


        }

        server1: StandardHost {
            @display("p=719.67,189.97499;i=device/server_l");
            gates:

                ethg[1];


        }
        switchA: EtherSwitch {
            parameters:
                @display("p=150.8625,176.565");
            gates:
                ethg[5];
        }
        switchB: EtherSwitch {
            parameters:
                @display("p=268.19998,129.62999");
            gates:
                ethg[5];
        }
        switchC: EtherSwitch {
            parameters:
                @display("p=260.3775,230.205");
            gates:
                ethg[2];
        }
        switchD: EtherSwitch {
            parameters:
                @display("p=392.2425,188.8575");
            gates:
                ethg[7];
        }
        switchE: EtherSwitch {
            parameters:
                @display("p=590.04,188.8575");
            gates:
                ethg[2];
        }

        configurator: IPv4NetworkConfigurator {
            @display("p=13.425,447.5");
        }
        l2NetworkConfigurator: L2NetworkConfigurator {
            @display("p=835.70624,415.05624");
        }
        //  lifecycleController: LifecycleController {
        //    @display("p=130.89375,402.75");
        //}
        //       stpTester: STPTester {
        //         @display("p=848.0125,328.9125");
        //   }
        scenarioManager: ScenarioManager {
            @display("p=708.16876,431.8375");
        }
        server2: StandardHost {
            @display("p=553.1625,49.17;i=device/server_l");
            gates:

                ethg[1];


        }
        client1: StandardHost {
            @display("p=67.049995,303.96;i=device/laptop");
            gates:

                ethg[1];

        }
        client6: StandardHost {
            @display("p=373.245,31.289999;i=device/pc");
            gates:

                ethg[1];

        }
        client4: StandardHost {
            @display("p=150.8625,31.289999;i=device/pc");
            gates:

                ethg[1];

        }
        client8: StandardHost {
            @display("p=509.58,355.365;i=device/pc");
            gates:

                ethg[1];

        }
        client9: StandardHost {
            @display("p=654.855,355.365;i=device/pc");
            gates:

                ethg[1];

        }
    connections:
        switchA.ethg++ <--> C <--> switchB.ethg++;
        switchC.ethg++ <--> C <--> switchD.ethg++;
        switchE.ethg++ <--> C <--> server1.ethg++;
        switchC.ethg++ <--> C <--> switchA.ethg++;
        switchB.ethg++ <--> C <--> switchD.ethg++;
        switchD.ethg++ <--> C <--> switchE.ethg++;
        client3.ethg++ <--> C <--> switchA.ethg++;
        client2.ethg++ <--> C <--> switchA.ethg++;
        client1.ethg++ <--> C <--> switchA.ethg++;
        client4.ethg++ <--> C <--> switchB.ethg++;
        client5.ethg++ <--> C <--> switchB.ethg++;
        client6.ethg++ <--> C <--> switchB.ethg++;
        client7.ethg++ <--> C <--> switchD.ethg++;
        client8.ethg++ <--> C <--> switchD.ethg++;
        client9.ethg++ <--> C <--> switchD.ethg++;
        server2.ethg++ <--> C <--> switchD.ethg++;
}


