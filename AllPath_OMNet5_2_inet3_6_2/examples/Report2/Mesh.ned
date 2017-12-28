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

package allpath.examples.Report2;

import inet.common.scenario.ScenarioManager;
import inet.examples.httptools.socket.tenserverssocket.ethernetline;
//import inet.common.lifecycle.LifecycleController;
//import inet.linklayer.configurator.L2NetworkConfigurator;
//import inet.linklayer.ieee8021d.tester.STPTester;
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.StandardHost;
//import inet.node.ethernet.EtherHost;

import ned.DatarateChannel;
import inet.common.misc.ThruputMeteringChannel;

network MeshLAN
{
    @display("bgb=693,429");
    types:
        channel C extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 100Mbps;
        }
    submodules:
        host1: StandardHost {
            @display("p=61,133;i=device/pc");
            gates:

                ethg[1];

        }
        host2: StandardHost {
            @display("p=298,38;i=device/pc");
            gates:
                ethg[1];


        }

        server3: StandardHost {
            @display("p=522,133;i=device/pc");
            gates:

                ethg[1];

        }
        host4: StandardHost {
            @display("p=61,216;i=device/pc");
            gates:

                ethg[1];

        }
        server6: StandardHost {
            @display("p=522,216;i=device/pc");
            gates:

                ethg[1];

        }
        host7: StandardHost {
            @display("p=61,304;i=device/pc");
            gates:

                ethg[1];

        }
        host8: StandardHost {
            @display("p=298,392;i=device/pc");
            gates:

                ethg[1];

        }
        server9: StandardHost {
            @display("p=530,304;i=device/pc");
            gates:

                ethg[1];

        }

        switch1: EtherSwitch {
            parameters:
                @display("p=184,132");
            gates:
                ethg[3];
        }
        switch2: EtherSwitch {
            parameters:
                @display("p=298,132");
            gates:
                ethg[4];
        }
        switch3: EtherSwitch {
            parameters:
                @display("p=408,132");
            gates:
                ethg[3];
        }
        switch4: EtherSwitch {
            parameters:
                @display("p=184,215");
            gates:
                ethg[4];
        }
        switch5: EtherSwitch {
            parameters:
                @display("p=298,215");
            gates:
                ethg[4];
        }
        switch6: EtherSwitch {
            parameters:
                @display("p=408,215");
            gates:
                ethg[4];
        }
        switch7: EtherSwitch {
            parameters:
                @display("p=184,303");
            gates:
                ethg[3];
        }
        switch8: EtherSwitch {
            parameters:
                @display("p=298,303");
            gates:
                ethg[4];
        }
        switch9: EtherSwitch {
            parameters:
                @display("p=408,303");
            gates:
                ethg[3];
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
        host1.ethg[0] <--> C <--> switch1.ethg[0];
        host2.ethg[0] <--> C <--> switch2.ethg[0];
        server3.ethg[0] <--> C <--> switch3.ethg[0];
        host4.ethg[0] <--> C <--> switch4.ethg[0];
        server6.ethg[0] <--> C <--> switch6.ethg[0];
        host7.ethg[0] <--> C <--> switch7.ethg[0];
        host8.ethg[0] <--> C <--> switch8.ethg[0];
        server9.ethg[0] <--> C <--> switch9.ethg[0];

        switch1.ethg[1] <--> C <--> switch2.ethg[1];
        switch1.ethg[2] <--> C <--> switch4.ethg[1];
        switch2.ethg[2] <--> C <--> switch3.ethg[1];
        switch2.ethg[3] <--> C <--> switch5.ethg[0];
        switch3.ethg[2] <--> C <--> switch6.ethg[1];
        switch6.ethg[2] <--> C <--> switch5.ethg[1];
        switch6.ethg[3] <--> C <--> switch9.ethg[1];
        switch9.ethg[2] <--> C <--> switch8.ethg[1];
        switch8.ethg[2] <--> C <--> switch7.ethg[1];
        switch7.ethg[2] <--> C <--> switch4.ethg[2];
        switch4.ethg[3] <--> C <--> switch5.ethg[2];
        switch5.ethg[3] <--> C <--> switch8.ethg[3];
}


