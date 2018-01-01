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

package wAPB.src.node.inet;


import inet.node.inet.WirelessHost;
import inet.linklayer.contract.IMACRelayUnit;
import inet.linklayer.contract.IMACAddressTable;

import wAPB.src.linklayer.ethernet.ARPPathSwitch.MACRelayUnitWAPB;
import wAPB.src.linklayer.ethernet.ARPPathSwitch.BlockingTableWAPB;
import wAPB.src.linklayer.ethernet.ARPPathSwitch.LearningTableWAPB;
import wAPB.src.linklayer.ieee80211.mgmt.Ieee80211MgmtAdhocAPB;
import wAPB.src.linklayer.ieee80211.Ieee80211NicAPB;

module AdhocHostAPB extends WirelessHostAPBW
{
    parameters:
        @display("i=device/cellphone");
        wlan[*].mgmtType = default("Ieee80211MgmtAdhocAPB");  // use adhoc management wARPPath
        forwarding = default(true);

        // EXTRA
        //string relayUnitType = default("MACRelayUnitWAPB");
        //string learningTableType = default("LearningTableWAPB"); 
        //string blockingTableType = default("BlockingTableWAPB"); 


    submodules:
        BT: BlockingTableWAPB  {  //<macTableType> like IMACAddressTable {
            @display("p=180,250;is=s");
        }
        LT: LearningTableWAPB { //<macTableType> like IMACAddressTable {
            @display("p=125,250;is=s");
        }
        relayUnit: MACRelayUnitWAPB{ //<relayUnitType> like IMACRelayUnit {
            parameters:
                @display("p=149,300");
            gates:
                ifIn[numRadios];
                ifOut[numRadios];
        }
    connections:
        for i=0..numRadios-1 {
            wlan[i].upperLayerRelayOut --> relayUnit.ifIn++ ;//if wlan[i] ==  "Ieee80211NicAPB"; //if numRadios>1;
            wlan[i].upperLayerRelayIn <-- relayUnit.ifOut++; //if numRadios>1;
        }
        //End EXTRA      
}

