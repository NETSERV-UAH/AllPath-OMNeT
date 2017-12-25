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

import arppathbridgesunidireccional.src.nodes.ethernet.EtherSwitchAPB;
import arppathbridgesunidireccional.src.nodes.wireless.APSwitchAPB;
import inet.world.radio.ChannelControl;
import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import libgist.src.nodes.inet.WirelessHostAPB;
import ned.DatarateChannel;

//
// TODO auto-generated type
//
network Wired_wireless_network
{
    @display("bgb=920,501");
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
        channelControl: ChannelControl {
            @display("p=53,32");
        }
        accessPoint1: APSwitchAPB {
            @display("p=159,116");
        }
        accessPoint2: APSwitchAPB {
            @display("p=742,116");
        }
        accessPoint3: APSwitchAPB {
            @display("p=425,228");
        }
        configurator: FlatNetworkConfigurator {
            @display("p=151,33");
        }
        host2: WirelessHostAPB {
            @display("p=892,117");
        }
        host3: WirelessHostAPB {
            @display("p=341,371");
        }
        host1: WirelessHostAPB {
            @display("p=33,117");
        }
        etherSwitchAPB: EtherSwitchAPB {
            @display("p=330,116");
        }
        etherSwitchAPB1: EtherSwitchAPB {
            @display("p=224,228");
        }
        etherSwitchAPB2: EtherSwitchAPB {
            @display("p=636,228");
        }
        etherSwitchAPB3: EtherSwitchAPB {
            @display("p=543,116");
        }
        host4: WirelessHostAPB {
            @display("p=504,371");
        }
    connections:
        accessPoint1.ethg++ <--> ethline <--> etherSwitchAPB.ethg++;
        accessPoint1.ethg++ <--> ethline <--> etherSwitchAPB1.ethg++;
        etherSwitchAPB1.ethg++ <--> ethline <--> etherSwitchAPB.ethg++;
        etherSwitchAPB.ethg++ <--> ethline <--> accessPoint3.ethg++;
        etherSwitchAPB1.ethg++ <--> ethline <--> accessPoint3.ethg++;
        accessPoint3.ethg++ <--> ethline <--> etherSwitchAPB3.ethg++;
        etherSwitchAPB.ethg++ <--> ethline <--> etherSwitchAPB3.ethg++;
        etherSwitchAPB3.ethg++ <--> ethline <--> etherSwitchAPB2.ethg++;
        accessPoint3.ethg++ <--> ethline <--> etherSwitchAPB2.ethg++;
        etherSwitchAPB2.ethg++ <--> ethline <--> accessPoint2.ethg++;
        etherSwitchAPB3.ethg++ <--> ethline <--> accessPoint2.ethg++;
}
