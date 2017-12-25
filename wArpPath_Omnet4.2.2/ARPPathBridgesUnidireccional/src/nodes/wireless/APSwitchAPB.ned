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

package arppathbridgesunidireccional.src.nodes.wireless;

import inet.mobility.IMobility;
import inet.linklayer.IWirelessNic;
import inet.linklayer.IMACRelayUnit;
import inet.linklayer.IEtherMAC;
import inet.base.NotificationBoard;
import inet.networklayer.common.InterfaceTable;
import libgist.src.linklayer.ARPPathSwitch.MACRelayUnitRoute;

module APSwitchAPB
{
    parameters:
        @node();
        @labels(node,ethernet-node,wireless-node);
        @display("i=device/accesspoint");
        string mobilityType = default("StationaryMobility"); // how the node moves around. see: inet.mobility
        string relayUnitType =  default("MACRelayUnitAPB");
        bool csmacdSupport = default(true);  // by default use CSMA/CD 
        string macType = default(csmacdSupport ? "EtherMACNew" : "EtherMACFullDuplex"); // ~EtherMAC or ~EtherMACFullDuplex (EtherMACNew esta adaptado en libgist, pero la version reducida (FullDuplex) no)
        int numRadios = default(1);               // the number of radios in the access point
        bool withInterfaceTable = default(false);    // module has interfaceTable if enabled
    gates:
        input radioIn[numRadios] @directIn;
        inout ethg[] @labels(EtherFrame-conn);
    submodules:
        interfaceTable: InterfaceTable if withInterfaceTable {
            @display("p=59,121");
        }
        relayUnit: <relayUnitType> like IMACRelayUnit{
            parameters:
                @display("p=151,121");
            gates:
                lowerLayerIn[(sizeof(ethg))+numRadios];
                lowerLayerOut[(sizeof(ethg))+numRadios];
        }
        wlan[numRadios]: <default("Ieee80211Nic")> like IWirelessNic {
            parameters:
                @display("p=89,225;q=queue");
        }
        mac[sizeof(ethg)]:  <macType> like IEtherMAC {
            parameters:
                promiscuous = true;
                txQueueLimit = 1000; // increase if needed
                queueModule = "";
                @display("p=208,225,row;q=txQueue");
        }
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=59,43");
        }
        mobility: <mobilityType> like IMobility {
            parameters:
                @display("p=169,43");
        }
        relayUnitRoute: MACRelayUnitRoute {
            parameters:
                @display("p=219,111");
        }
    connections allowunconnected:
        for i=0..sizeof(ethg)-1 {
            mac[i].phys <--> ethg[i];
            mac[i].upperLayerIn <-- relayUnit.lowerLayerOut[i];
            mac[i].upperLayerOut --> relayUnit.lowerLayerIn[i];
        }
        for i=0..numRadios-1 {
            radioIn[i] --> wlan[i].radioIn;
            wlan[i].upperLayerOut --> relayUnit.lowerLayerIn[(sizeof(ethg))+i];
            wlan[i].upperLayerIn <-- relayUnit.lowerLayerOut[(sizeof(ethg))+i];
        }
}
