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

package arppathbridgesunidireccional.src.topologies.dataCenter2L4CGenerator;

import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import arppathbridgesunidireccional.src.nodes.ethernet.EtherSwitchAPB;
import libgist.src.nodes.inet.StandardHostNew;
import libgist.src.simulationmodels.flowmodels.UDPFlowGenerator;
import ned.DatarateChannel;

network dataCenter2L4Cgenerator //Data Center de 2 niveles (L) y 4 cores (C) valido para el generador
{
    parameters:
        int groupNumber = default(5); //nº de grupos a cada lado del data center (5*2=10 grupos en total si 5 es el valor, 5 izquierda y 5 derecha)
        int hostNumber = default(25);
    	@display("bgb=595,510");

	types:
        channel ethline extends DatarateChannel
        {
            delay = 50ns;
            datarate = 100Mbps;
        }
    submodules:
        configurator: FlatNetworkConfigurator {
            @display("p=60,41");
        }
        generator: UDPFlowGenerator {
            @display("p=169,41");
        }
        s1: EtherSwitchAPB {
            @display("p=200,200");
        }
        s2: EtherSwitchAPB {
            @display("p=200,280");
        }
        s3: EtherSwitchAPB {
            @display("p=300,200");
        }
        s4: EtherSwitchAPB {
            @display("p=300,280");
        }
        switch[groupNumber*2]: EtherSwitchAPB {
            @display("p=140,120,column,30"); //http://www.omnetpp.org/doc/omnetpp/manual/usman.html#sec537
        }
        host[groupNumber*2*hostNumber]: StandardHostNew {
            @display("p=30,90,matrix,5,10,20");
        }

    connections allowunconnected:
        s3.ethg++ <--> ethline <--> s1.ethg++;
        s1.ethg++ <--> ethline <--> s2.ethg++;
        s2.ethg++ <--> ethline <--> s4.ethg++;
        s4.ethg++ <--> ethline <--> s3.ethg++;
        s4.ethg++ <--> ethline <--> s1.ethg++;
        s3.ethg++ <--> ethline <--> s2.ethg++;
        
        for i=0..groupNumber-1 {
            switch[i].ethg++ <--> ethline <--> s1.ethg++;
            switch[i].ethg++ <--> ethline <--> s2.ethg++;
        }
        
        for i=groupNumber..2*groupNumber-1 {
            switch[i].ethg++ <--> ethline <--> s3.ethg++;
            switch[i].ethg++ <--> ethline <--> s4.ethg++;
        }
        
        for i=0..2*groupNumber-1, for j=0..hostNumber-1 {
        	switch[i].ethg++ <--> ethline <--> host[i*hostNumber+j].ethg++;
        }
}
