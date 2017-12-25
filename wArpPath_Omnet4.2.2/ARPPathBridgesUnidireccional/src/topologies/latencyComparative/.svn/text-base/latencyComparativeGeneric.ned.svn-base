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
// 2013 (Elisa)

package arppathbridgesunidireccional.src.topologies.latencyComparative;

import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import arppathbridgesunidireccional.src.nodes.ethernet.EtherSwitchAPB;
import libgist.src.nodes.inet.StandardHostNew;
import ned.DatarateChannel;

//
// A generated network with grid topology.
//
network latencyComparativeGeneric
{
    parameters:
        int paths = default(3);
        int size = default(2); //Size of the first path
        int sizeDiff = default(1); //First path is set to size = 'size' and next will be = 'size'+'sizeDiff', next 'size'+2*'sizeDiff' and so on
        int hostNumber = default(25);
        @display("bgb=900,500");
    types:
        channel ethline extends DatarateChannel
        {
            delay = 50ns;
            datarate = 100Mbps; //1Gbps; 
        }
    submodules:
        flatNetworkConfigurator: FlatNetworkConfigurator 
        {
            @display("p=276,29");
        } 
        //Number of switches = 2 (edge) + paths*size + sizeDiff*summation from 0 to paths-1 (extra switches), and summation from 0 to paths-1 = paths*(paths-1)/2
        //switch[2 + paths*size + sizeDiff*(paths*(paths-1))/2]: EtherSwitchAPB 
        edgeSwitchLeft: EtherSwitchAPB
        {
            @display("p=150,200");
        }
        edgeSwitchRight: EtherSwitchAPB
        {
            @display("p=750,200");
        }
        pathSwitch[paths*size]: EtherSwitchAPB 
        {
            @display("p=300,200,matrix,$size,70,50");
        }
        pathExtraSwitch[sizeDiff*(paths*(paths-1))/2]: EtherSwitchAPB 
        {
            @display("p=600,250,column,50");
        }
        hostLeft[hostNumber]: StandardHostNew 
        {
            @display("p=50,100,column,25");
        }
        hostRight[hostNumber]: StandardHostNew 
        {
            @display("p=850,100,column,25");
        }
        //Manual (display string): http://www.omnetpp.org/doc/omnetpp/manual/usman.html#sec537

    connections allowunconnected:
        for i=0..hostNumber-1 //Connect host with edge bridges
        {
            edgeSwitchLeft.ethg++ <--> ethline <--> hostLeft[i].ethg++;
            edgeSwitchRight.ethg++ <--> ethline <--> hostRight[i].ethg++;
        }
        
        for i=0..paths-1, for j=0..size-1 //Connect left edge bridge and join path bridges together  
        {
            edgeSwitchLeft.ethg++ <--> ethline <--> pathSwitch[i*size+j].ethg++ if j==0;
            pathSwitch[i*size+j].ethg++ <--> ethline <--> pathSwitch[i*size+j+1].ethg++ if j!=size-1;
            
            //Connect also right edge brige to the first path, and to all the others if sizeDiff=0
            edgeSwitchRight.ethg++ <--> ethline <--> pathSwitch[i*size+j].ethg++ if i==0 && j==size-1;
            edgeSwitchRight.ethg++ <--> ethline <--> pathSwitch[i*size+j].ethg++ if i!=0 && sizeDiff==0 && j==size-1;
        }
    
        for i=1..paths-1, for j=0..sizeDiff*i-1 //Connect right edge bridge and join extra path bridges together, also with the previous ones 
        {
            pathSwitch[(i+1)*size-1].ethg++ <--> ethline <--> pathExtraSwitch[(i*(i-1))/2*sizeDiff+j].ethg++ if j==0;
            pathExtraSwitch[(i*(i-1))/2*sizeDiff+j].ethg++ <--> ethline <--> pathExtraSwitch[(i*(i-1))/2*sizeDiff+j+1].ethg++ if j!=sizeDiff*i-1;
            pathExtraSwitch[(i*(i-1))/2*sizeDiff+j].ethg++ <--> ethline <--> edgeSwitchRight.ethg++ if j==sizeDiff*i-1;
        }
}
