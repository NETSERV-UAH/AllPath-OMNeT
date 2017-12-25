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
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.world.radio.ChannelControl;
import libgist.src.nodes.inet.WirelessHostAPB;


//
// TODO auto-generated type
//
network Prueba_Wireless
{
    @display("bgb=671,345");
    submodules:
        AP: APSwitchAPB {
            @display("p=325,147");
            //wlan[*].mgmtType = "Ieee80211MgmtSTASimplified";
        }
        host2: WirelessHostAPB {
            @display("p=512,258");
            //wlan[*].mgmtType = "Ieee80211MgmtSTASimplified";
        }
        host1: WirelessHostAPB {
            @display("p=130,261");
            //wlan[*].mgmtType = "Ieee80211MgmtSTASimplified";
        }
        channelControl: ChannelControl {
            @display("p=76,45");
        }
        configurator: IPv4NetworkConfigurator {
            config=xml("<config><interface hosts='*' address='145.236.x.x' netmask='255.255.0.0'/></config>");
            @display("p=140,50");
        }
}
