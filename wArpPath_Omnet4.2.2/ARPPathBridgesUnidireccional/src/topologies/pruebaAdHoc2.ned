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

import libgist.src.nodes.inet.AdhocHostAPB;
import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import inet.world.radio.ChannelControl;

//
// TODO auto-generated type
//
network PruebaAdHoc2
{
    @display("bgb=698,440");
    submodules:
        host1: AdhocHostAPB {
            @display("p=64,241");
        }
        host2: AdhocHostAPB {
            @display("p=440,303");
        }
        host3: AdhocHostAPB {
            @display("p=255,264");
        }
        host4: AdhocHostAPB {
            @display("p=372,163");
        }
        channelControl: ChannelControl {
            @display("p=71,56");
        }
        Configurator: FlatNetworkConfigurator {
            @display("p=210,57");
        }
        host5: AdhocHostAPB {
            @display("p=570,202");
        }
}
