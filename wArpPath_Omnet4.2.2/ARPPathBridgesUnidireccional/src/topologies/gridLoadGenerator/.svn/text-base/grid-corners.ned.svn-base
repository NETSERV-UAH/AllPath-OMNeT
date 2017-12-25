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
// 2012 (Elisa)

package arppathbridgesunidireccional.src.topologies.gridLoadGenerator;

import libgist.src.simulationmodels.flowmodels.UDPFlowGenerator;
import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import arppathbridgesunidireccional.src.nodes.ethernet.EtherSwitchAPB;
import libgist.src.nodes.inet.StandardHostNew;
import ned.DelayChannel;

//
// A generated network with grid topology.
//
network gridcorners
{
    parameters:
        int columns = default(3);
        int rows = default(3);
        int hostNumber = default(25);
        @display("bgb=614,552");
    submodules:
        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=228,30");
        }
        switch[columns*rows]: EtherSwitchAPB {
            @display("p=187,91,matrix,$columns");

        }
        hostI[hostNumber]: StandardHostNew {
            @display("p=68,92,column,55");
        }
        hostD[hostNumber]: StandardHostNew {
            @display("p=487,92,column,55");
        }

        udpFlowGenerator: UDPFlowGenerator {
            @display("p=379,30");
        }
    connections allowunconnected:
        for i=0..rows-1, for j=0..columns-1 {
            switch[i*columns+j].ethg++ <--> DelayChannel <--> switch[(i+1)*columns+j].ethg++ if i!=rows-1;
            switch[i*columns+j].ethg++ <--> DelayChannel <--> switch[i*columns+j+1].ethg++ if j!=columns-1;
        }
        for i=0..hostNumber-1 {
            hostI[i].ethg++ <--> DelayChannel <--> switch[0*columns].ethg++;
            hostD[i].ethg++ <--> DelayChannel <--> switch[(rows-1)*columns+(columns-1)].ethg++;
        }

}
