//
//
// Copyright (C) 2014 Isaias Martinez Yelmo (Inet 2.2.0 and Inet 2.3.0 adaptation)
// Copyright (C) 2012 Elisa Rojas (module 'udpGen' added, UDPFlowHost type (for the UDPFlowGenerator), to be used by the generator, and updated to inet2.0.0 with NodeBaseNew)
// Copyright (C) 2004 Andras Varga
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

package libgist.src.nodes.inet; //EXTRA-IMY

import inet.applications.IPingApp;
import inet.applications.ISCTPApp;
import inet.applications.ITCPApp;
import inet.applications.IUDPApp;
import inet.transport.ISCTP;
import inet.transport.ITCP;
import inet.transport.IUDP;


//
// IPv4 host with SCTP, TCP, UDP layers and applications.
// IP forwarding is disabled by default (see IPForward).
// - Can be connected via ethernet interface to other nodes using
//   the ethg gate. By default full-duplex connections are supported
//   only (twisted pair). Set **.eth[*].typename="EthernetInterface" for
//   a full/half-duplex CSMA/CD implementation (coaxial cable)
// - By default contains no wireless cards, however it can be configured
//   by the numRadios parameter. Wirless card type is configured by the
//   **.wlan[*].typename parameter. see:  inet.linklayer.ieee80211 or other
//   modules implementing ~IWirelessNic
// - Also external interfaces can be configured for HW in the loop simulation
//   using the numExtInterfaces parameter and setting the type using
//   **.ext[*].typename in the INI file. see: ~ExtInterface and ~IExternalNic
// - If wireless card is present, node mobility can be set using **.mobilityType
//   see: inet.mobility and ~IMobility
//
module StandardHostNew extends NodeBaseNew //EXTRA-IMY
{
    parameters:
        @display("i=device/pc2");
        int numTcpApps = default(0);  // no of TCP apps. Specify the app types in INI file with tcpApp[0..1].typename="TCPEchoApp" syntax
        int numUdpApps = default(0);  // no of UDP apps. Specify the app types in INI file with udpApp[0..1].typename="UDPVideoStreamCli" syntax
        int numSctpApps = default(0); // no of SCTP apps. Specify the app types in INI file with sctpApp[0..1].typename="SCTPServer" syntax
        int numPingApps = default(0);  // no of PING apps. Specify the app types in INI file with pingApp[0..1].typename="PingApp" syntax
        bool hasTcp = default(numTcpApps>0);
        bool hasUdp = default(numUdpApps>0);
        bool hasSctp = default(numSctpApps>0);
        string tcpType = default(firstAvailable("TCP", "TCP_lwIP", "TCP_NSC", "TCP_None"));  // tcp implementation (e.g. ~TCP, ~TCP_lwIP, ~TCP_NSC) or ~TCPSpoof
        string udpType = default(firstAvailable("UDP","UDP_None"));
        string udpGenType = default("UDPFlowHost"); //EXTRA: There is always one, so we cannot omit it, other option would be adding a parameter that should be one or zero (host gen or not, but I prefer it this way)
        string sctpType = default(firstAvailable("SCTP","SCTP_None"));
        IPForward = default(false);  // disable routing by default
        networkLayer.proxyARP = default(false);
    submodules:
        tcpApp[numTcpApps]: <> like ITCPApp {
            parameters:
                @display("p=147,54,row,60");
        }
        tcp: <tcpType> like ITCP if hasTcp {
            parameters:
                @display("p=147,141");
        }
        udpApp[numUdpApps]: <> like IUDPApp {
            parameters:
                @display("p=329,54,row,60");
        }
        
        // EXTRA-IMY BEGIN        
        udpGen: <udpGenType> like IUDPApp {
            parameters:
                @display("p=407,129");
        }    
        // EXTRA-IMY END
                
        udp: <udpType> like IUDP if hasUdp {
            parameters:
                @display("p=329,141");
         }
        
		
		 // EXTRA-IMY BEGIN 
		 // Now we have definition above
		//
        // udp: <udpType> like IUDP {//if numUdpApps>0 { //EXTRA
        //    parameters:
        //        @display("p=336,141");
        // }
        // EXTRA-IMY END
        
        sctpApp[numSctpApps]: <> like ISCTPApp {
            parameters:
                @display("p=527,54,row,60");
        }
        sctp: <sctpType> like ISCTP if hasSctp {
            @display("p=527,141");
        }
        pingApp[numPingApps]: <default("PingApp")> like IPingApp {
            parameters:
                @display("p=635,141,row,60");
        }
    connections allowunconnected:
        for i=0..numTcpApps-1 {
            tcpApp[i].tcpOut --> tcp.appIn++;
            tcpApp[i].tcpIn <-- tcp.appOut++;
        }

        tcp.ipOut --> networkLayer.transportIn++ if hasTcp;
        tcp.ipIn <-- networkLayer.transportOut++ if hasTcp;

        for i=0..numUdpApps-1 {
            udpApp[i].udpOut --> udp.appIn++;
            udpApp[i].udpIn <-- udp.appOut++;
        }

		udpGen.udpOut --> udp.appIn++; //EXTRA
        udpGen.udpIn <-- udp.appOut++; //EXTRA

        // udp.ipOut --> networkLayer.udpIn; //if numUdpApps>0; //EXTRA
        // udp.ipIn <-- networkLayer.udpOut; //if numUdpApps>0; //EXTRA
        // EXTRA-IMT Now exits lines below

        udp.ipOut --> networkLayer.transportIn++ if hasUdp;
        udp.ipIn <-- networkLayer.transportOut++ if hasUdp;

        for i=0..numSctpApps-1 {
            sctpApp[i].sctpOut --> sctp.from_appl++;
            sctp.to_appl++ --> sctpApp[i].sctpIn;
        }
        sctp.to_ip --> networkLayer.transportIn++ if hasSctp;
        networkLayer.transportOut++ --> sctp.from_ip if hasSctp;

        for i=0..numPingApps-1 {
            networkLayer.pingOut++ --> pingApp[i].pingIn;
            networkLayer.pingIn++ <-- pingApp[i].pingOut;
        }
}
