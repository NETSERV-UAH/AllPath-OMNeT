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

network test
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
        client1: StandardHost {
            @display("p=53.64,322.9575;i=device/laptop");
            gates:

                ethg[1];

        }
        client2: StandardHost {
            @display("p=53.64,219.03;i=device/pc");
            gates:

                ethg[1];

        }
        client3: StandardHost {
            @display("p=53.64,128.5125;i=device/pc");
            gates:
                ethg[1];
        }
        client4: StandardHost {
            @display("p=164.27249,31.289999;i=device/pc");
            gates:

                ethg[1];

        }
        client5: StandardHost {
            @display("p=328.54498,25.7025;i=device/pc");
            gates:
                ethg[1];


        }
        client6: StandardHost {
            @display("p=472.70248,31.289999;i=device/pc");
            gates:

                ethg[1];

        }
        server1: StandardHost {
            @display("p=745.3725,219.03;i=device/server_l");
            gates:

                ethg[1];


        }
        server2: StandardHost {
            @display("p=628.035,40.23;i=device/server_l");
            gates:

                ethg[1];


        }
        
        switchA: EtherSwitch {
            parameters:
                @display("p=184.3875,217.91249");
            gates:
                ethg[5];
        }
        switchB: EtherSwitch {
            parameters:
                @display("p=328.54498,127.395");
            gates:
                ethg[5];
        }
        switchC: EtherSwitch {
            parameters:
                @display("p=328.54498,302.8425");
            gates:
                ethg[2];
        }
        switchD: EtherSwitch {
            parameters:
                @display("p=472.70248,217.91249");
            gates:
                ethg[4];
        }
        switchE: EtherSwitch {
            parameters:
                @display("p=628.035,217.91249");
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
        server2.ethg++ <--> C <--> switchD.ethg++;
}


