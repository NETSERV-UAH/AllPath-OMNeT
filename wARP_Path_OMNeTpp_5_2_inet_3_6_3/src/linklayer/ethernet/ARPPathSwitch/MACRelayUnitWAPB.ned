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

package wAPB.src.linklayer.ethernet.ARPPathSwitch;

import inet.linklayer.contract.IMACRelayUnit;

simple MACRelayUnitWAPB like IMACRelayUnit
{
    parameters:
        @class(wapb::MACRelayUnitWAPB);        
        string learningTablePath = default("^.LT"); // The path to the LT MACAddressTable module
        string blockingTablePath = default("^.BT"); // The path to the BT MACAddressTable module        
        @display("i=block/switch");
        @signal[packetSentToLower](type=cPacket);
        @signal[packetReceivedFromLower](type=cPacket);

        //EXTRA
        string implementation = default("new");
        string functionality = default("wARP-Path");
		int protocolVersion = default(2); //Two versions of ARP-Path are implemented (2 -unidirectional- and 3 -conditional-, not 1 because it didn't work properly)
		double helloTime @unit("s") = default(10s);		//Hello freq time (and HeT timer = helloTime + blockingTime (not used))
		double repairingTime @unit("s") = default(1s);	//RT timer
		int repairType = default(0);  //default(1); for ARP-Path. //Three types of repair: 1-PathFail+PathReq+PathReply; 2-PathFail+PathReq; 3-PathFail+PathReply -- 0-NoRepair (saves events at executing if we will not use any repair method)
		
        string address = default("00-00-00-00-00-00"); //Set to 00-... by default, if MACRelayUnit watchs the default address (all zeroes), it will assign an automatic MAC address per module ID
		string portLinkDown = default(""); //Ports which links are down and times (port number ':' init time '-' end time, and separated by ';')
		string switchDown = default(""); //Times at which the switch will be completely down (init time '-' end time, and separated by ';') - This parameter deletes all tables and, combined with the portLinkDown, it can simulate a switch that failed (and later was restarted)
						
		int broadcastSeed = default(0);
		bool multicastActive = default(true); //Indicates whether multicast optimization is active or not (active by default)
		//end EXTRA
		
		        
    gates:
        input ifIn[] @labels(EtherFrame);
        output ifOut[] @labels(EtherFrame);
        input stpIn @loose;
        output stpOut @loose;
}