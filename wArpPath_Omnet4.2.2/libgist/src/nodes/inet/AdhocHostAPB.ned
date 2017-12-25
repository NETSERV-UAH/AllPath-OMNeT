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

package libgist.src.nodes.inet;

import inet.networklayer.IManetRouting;
import inet.linklayer.IMACRelayUnit;

module AdhocHostAPB extends WirelessHostAPB
{
   parameters:
        @display("i=device/cellphone");
        wlan[*].mgmtType = default("Ieee80211MgmtAdhocAPB");  // use adhoc management ARPPath
        string routingProtocol @enum("AODVUU","DYMOUM","DYMO","DSRUU","OLSR","OLSR_ETX","DSDV_2","Batman") = default("");  // used mobile routing protocol. see: inet.networklayer.manetrouting
        IPForward = default(true);
		string relayUnitType = default("MACRelayUnitAPBw");
		
    submodules:
        manetrouting: <routingProtocol> like IManetRouting if routingProtocol != "" {
            @display("p=522,307");
        }
        relayUnit: <relayUnitType> like IMACRelayUnit{
            parameters:
                @display("p=127,320");
           gates:
               lowerLayerIn[numRadios];
               lowerLayerOut[numRadios];
        }
    connections:
        networkLayer.manetOut --> manetrouting.from_ip if routingProtocol != "";
        networkLayer.manetIn <-- manetrouting.to_ip if routingProtocol != "";
        
       for i=0..numRadios-1 {
           wlan[i].upperLayerOut --> relayUnit.lowerLayerIn[numRadios] if numRadios>1;
           wlan[i].upperLayerIn <-- relayUnit.lowerLayerOut[numRadios] if numRadios>1;
       } 
}
