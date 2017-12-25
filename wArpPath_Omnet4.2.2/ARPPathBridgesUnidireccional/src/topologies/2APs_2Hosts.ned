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

import arppathbridgesunidireccional.src.nodes.wireless.APSwitchAPB;
import inet.world.radio.ChannelControl;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import libgist.src.nodes.inet.WirelessHostAPB;
import ned.DatarateChannel;

//
// TODO auto-generated type
//
network _2APs_2Hosts
{
    parameters:
        @display("bgb=637,274");
        bool useEthernetSwitching = default(true);
        bool useRouting = default(false);
    submodules:
        channelControl: ChannelControl {
            @display("p=53,32");
        }
        accessPoint1: APSwitchAPB {
            @display("p=231,116");
            numRadios = 2;
        }
        accessPoint2: APSwitchAPB {
            @display("p=394,116");
            numRadios = 2;
        }
        configurator: IPv4NetworkConfigurator {
            parameters:
                @display("p=187,34");
        }
        host2: WirelessHostAPB {
            @display("p=543,117");
        }
        host1: WirelessHostAPB {
            @display("p=76,117");
        }
  //  connections:        
     //   accessPoint1.ethg++ <--> DatarateChannel {  datarate = 100Mbps; } <--> accessPoint2.ethg++ if useEthernetSwitching;
}
