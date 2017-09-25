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

package arppathbridgesunidireccional.src.topologies.dataCenterPortland;
import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import inet.nodes.inet.StandardHost;
import libgist.src.nodes.inet.StandardHostNew;
import arppathbridgesunidireccional.src.nodes.ethernet.EtherSwitchAPB;
import arppathbridgesunidireccional.src.linklayer.ARPPathSwitch.MACRelayUnitAPB;
import ned.DatarateChannel;
import libgist.src.simulationmodels.flowmodels.UDPFlowGenerator;

//
// TODO documentation
//
// @author elisa
//
network dataCenterPortlandGenerator
{
    @display("bgb=976,477");
    types:
        channel ethline extends DatarateChannel
        {
            delay = 50ns;
            datarate = 100Mbps;
        }
    submodules:
        //Root switches
        switch1: EtherSwitchAPB 
        {
            parameters:
            	@display("p=275,60");
            gates:
                ethg[5];
        }
        switch2: EtherSwitchAPB 
        {
            parameters:
            	@display("p=425,60");
            gates:
                ethg[5];
        }
        switch3: EtherSwitchAPB 
        {
            parameters:
            	@display("p=575,60");
            gates:
                ethg[5];
        }
        switch4: EtherSwitchAPB 
        {
            parameters:
            	@display("p=725,60");
            gates:
                ethg[5];
        }
        //Pod 1
        switch11: EtherSwitchAPB 
        {
            parameters:
            	@display("p=150,225");
            gates:
                ethg[5];
        }
        switch12: EtherSwitchAPB 
        {
            parameters:
            	@display("p=250,225");
            gates:
                ethg[5];
        }
        switch13: EtherSwitchAPB 
        {
            parameters:
            	@display("p=150,325");
            gates:
                ethg[5];
        }
        switch14: EtherSwitchAPB 
        {
            parameters:
            	@display("p=250,325");
            gates:
                ethg[5];
        }
        //Pod 2
        switch21: EtherSwitchAPB 
        {
            parameters:
            	@display("p=350,225");
            gates:
                ethg[5];
        }
        switch22: EtherSwitchAPB 
        {
            parameters:
            	@display("p=450,225");
            gates:
                ethg[5];
        }
        switch23: EtherSwitchAPB 
        {
            parameters:
            	@display("p=350,325");
            gates:
                ethg[5];
        }
        switch24: EtherSwitchAPB 
        {
            parameters:
            	@display("p=450,325");
            gates:
                ethg[5];
        }
        //Pod 3
        switch31: EtherSwitchAPB 
        {
            parameters:
            	@display("p=550,225");
            gates:
                ethg[5];
        }
        switch32: EtherSwitchAPB  
        {
            parameters:
            	@display("p=650,225");
            gates:
                ethg[5];
        }
        switch33: EtherSwitchAPB  
        {
            parameters:
            	@display("p=550,325");
            gates:
                ethg[5];
        }
        switch34: EtherSwitchAPB  
        {
            parameters:
            	@display("p=650,325");
            gates:
                ethg[5];
        }
        //Pod 2
        switch41: EtherSwitchAPB  
        {
            parameters:
            	@display("p=750,225");
            gates:
                ethg[5];
        }
        switch42: EtherSwitchAPB  
        {
            parameters:
            	@display("p=850,225");
            gates:
                ethg[5];
        }
        switch43: EtherSwitchAPB  
        {
            parameters:
            	@display("p=750,325");
            gates:
                ethg[5];
        }
        switch44: EtherSwitchAPB  
        {
            parameters:
            	@display("p=850,325");
            gates:
                ethg[5];
        }

        //Hosts
        host1A: StandardHostNew {
            @display("p=125,410");
        }
        host1B: StandardHostNew {
            @display("p=175,410");
        }
        host1C: StandardHostNew {
            @display("p=225,410");
        }
        host1D: StandardHostNew {
            @display("p=275,410");
        }
        host2A: StandardHostNew {
            @display("p=325,410");
        }
        host2B: StandardHostNew {
            @display("p=375,410");
        }
        host2C: StandardHostNew {
            @display("p=425,410");
        }
        host2D: StandardHostNew {
            @display("p=475,410");
        }
        host3A: StandardHostNew {
            @display("p=525,410");
        }
        host3B: StandardHostNew {
            @display("p=575,410");
        }
        host3C: StandardHostNew {
            @display("p=625,410");
        }
        host3D: StandardHostNew {
            @display("p=675,410");
        }
        host4A: StandardHostNew {
            @display("p=725,410");
        }
        host4B: StandardHostNew {
            @display("p=775,410");
        }
        host4C: StandardHostNew {
            @display("p=825,410");
        }
        host4D: StandardHostNew {
            @display("p=875,410");
        }

        //Extra config
        configurator: FlatNetworkConfigurator {
            @display("p=101,36");
        }
        generator: UDPFlowGenerator {
            @display("p=181,36");
        }
    connections allowunconnected:
        //From the root switches to the pods
        switch1.ethg[1] <--> ethline <--> switch11.ethg[3]; //ethg[0] or ethg++
        switch1.ethg[2] <--> ethline <--> switch22.ethg[3];
        switch1.ethg[3] <--> ethline <--> switch32.ethg[3];
        switch1.ethg[4] <--> ethline <--> switch41.ethg[3];
        switch2.ethg[1] <--> ethline <--> switch11.ethg[4];
        switch2.ethg[2] <--> ethline <--> switch21.ethg[4];
        switch2.ethg[3] <--> ethline <--> switch32.ethg[4];
        switch2.ethg[4] <--> ethline <--> switch42.ethg[3];
        switch3.ethg[1] <--> ethline <--> switch12.ethg[3];
        switch3.ethg[2] <--> ethline <--> switch21.ethg[3];
        switch3.ethg[3] <--> ethline <--> switch31.ethg[3];
        switch3.ethg[4] <--> ethline <--> switch42.ethg[4];
        switch4.ethg[1] <--> ethline <--> switch12.ethg[4];
        switch4.ethg[2] <--> ethline <--> switch22.ethg[4];
        switch4.ethg[3] <--> ethline <--> switch31.ethg[4];
        switch4.ethg[4] <--> ethline <--> switch41.ethg[4];

        //In the pods
        switch11.ethg[1] <--> ethline <--> switch13.ethg[3];
        switch11.ethg[2] <--> ethline <--> switch14.ethg[3];
        switch12.ethg[2] <--> ethline <--> switch14.ethg[4];
        switch12.ethg[1] <--> ethline <--> switch13.ethg[4];
        switch21.ethg[1] <--> ethline <--> switch23.ethg[3];
        switch21.ethg[2] <--> ethline <--> switch24.ethg[3];
        switch22.ethg[2] <--> ethline <--> switch24.ethg[4];
        switch22.ethg[1] <--> ethline <--> switch23.ethg[4];
        switch31.ethg[1] <--> ethline <--> switch33.ethg[3];
        switch31.ethg[2] <--> ethline <--> switch34.ethg[3];
        switch32.ethg[2] <--> ethline <--> switch34.ethg[4];
        switch32.ethg[1] <--> ethline <--> switch33.ethg[4];
        switch41.ethg[1] <--> ethline <--> switch43.ethg[3];
        switch41.ethg[2] <--> ethline <--> switch44.ethg[3];
        switch42.ethg[2] <--> ethline <--> switch44.ethg[4];
        switch42.ethg[1] <--> ethline <--> switch43.ethg[4];

        //From the pods to the hosts
        switch13.ethg[1] <--> ethline <--> host1A.ethg++;
        switch13.ethg[2] <--> ethline <--> host1B.ethg++;
        switch14.ethg[1] <--> ethline <--> host1C.ethg++;
        switch14.ethg[2] <--> ethline <--> host1D.ethg++;
        switch23.ethg[1] <--> ethline <--> host2A.ethg++;
        switch23.ethg[2] <--> ethline <--> host2B.ethg++;
        switch24.ethg[1] <--> ethline <--> host2C.ethg++;
        switch24.ethg[2] <--> ethline <--> host2D.ethg++;
        switch33.ethg[1] <--> ethline <--> host3A.ethg++;
        switch33.ethg[2] <--> ethline <--> host3B.ethg++;
        switch34.ethg[1] <--> ethline <--> host3C.ethg++;
        switch34.ethg[2] <--> ethline <--> host3D.ethg++;
        switch43.ethg[1] <--> ethline <--> host4A.ethg++;
        switch43.ethg[2] <--> ethline <--> host4B.ethg++;
        switch44.ethg[1] <--> ethline <--> host4C.ethg++;
        switch44.ethg[2] <--> ethline <--> host4D.ethg++;
}



