package arppathbridgesunidireccional.src.nodes.ethernet;

//EtherSwitchAPB is a switch that uses MACRelayUnitAPB implementation of ARP Path Bridges protocol.

import inet.linklayer.IMACRelayUnit;
import inet.linklayer.IEtherMAC;
import libgist.src.linklayer.ARPPathSwitch.MACRelayUnitRoute;

module EtherSwitchAPB
{
    parameters:    
        @node();
        @labels(node,ethernet-node);
        @display("i=device/switch");
        bool csmacdSupport = default(true);  // by default use CSMA/CD 
        string macType = default(csmacdSupport ? "EtherMACNew" : "EtherMACFullDuplex"); // ~EtherMAC or ~EtherMACFullDuplex (EtherMACNew esta adaptado en libgist, pero la version reducida (FullDuplex) no)
        string relayUnitType = default("MACRelayUnitAPB"); // type of the ~IMACRelayUnit; currently possible
                                                          // values (in inet) are ~MACRelayUnitNP and ~MACRelayUnitPP
    gates:
        inout ethg[] @labels(EtherFrame-conn);
    submodules:
        relayUnit: <relayUnitType> like IMACRelayUnit {
            parameters:
                @display("p=200,50");
            gates:
                lowerLayerIn[sizeof(ethg)];
                lowerLayerOut[sizeof(ethg)];
        }
        mac[sizeof(ethg)]: <macType> like IEtherMAC {
            parameters:
                promiscuous = true;
                queueModule = "";
                @display("p=70,150,row;q=txQueue");
        }
        relayUnitRoute: MACRelayUnitRoute {
            parameters:
                @display("p=219,111");
        }
    connections:
        for i=0..sizeof(ethg)-1 {
            mac[i].upperLayerIn <-- relayUnit.lowerLayerOut[i];
            mac[i].upperLayerOut --> relayUnit.lowerLayerIn[i];
            mac[i].phys <--> ethg[i];
        }
}
