//
// Copyright (C) 03/2012 Elisa Rojas
//
/*
 * Copyright (C) 2018 Elisa Rojas(1), Hedayat Hosseini(2);
 *                    (1) GIST, University of Alcala, Spain.
 *                    (2) CEIT, Amirkabir University of Technology (Tehran Polytechnic), Iran.
 *                    INET 3.6.3 adaptation
*/
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "src/simulationmodels/flowmodels/UDPFlowGenerator.h"

#include <stdio.h>

namespace wapb {
using namespace inet;

Define_Module(UDPFlowGenerator);

void UDPFlowGenerator::startRandomFlow()
{
    FlowGeneratorBase::startRandomFlow();

    EV << "->UDPFlowGenerator::startRandomFlow()" << endl;

    std::string flowInfo, smodel; //String to be saved for final statistics about the flow generator
    std::stringstream ss1, ss2, ss3;

    //We randomly choose a source and a destination for the new flow
    int iSource=0, iDestination=0;
    getRandomSrcDstIndex(iSource, iDestination);
    adhocInfo[iSource].nFlowSource++;
    adhocInfo[iDestination].nFlowDestination++;
    flowInfo = adhocInfo[iSource].fullName + "->" + adhocInfo[iDestination].fullName;

    //Now a transfer rate and flow size, randomly as well - http://www.omnetpp.org/doc/omnetpp41/api/group__RandomNumbersCont.html
    /*Tasa de transferencia:
        30% - 0,5 Mbps
        60% - 1 Mbps
        10% - 10 Mbps */
    unsigned int transferRate = intrand(10); // [0,10)
    if (transferRate < 3) transferRate = 500; //30%
    else if (transferRate < 9) transferRate = 1000; //60%
    else transferRate = 10000; //10%
    ss1 << transferRate;
    flowInfo = flowInfo + "; " + ss1.str() + " Kbps";

    unsigned long long flowSize;
    if(dataCenterTraffic)
    {
        /*VL2: 99% are mice < 100MB (around some KB); elephants between 100MB~1GB, but 100MB are common chunks because...
            Nuestro modelo:
            95% mice -> 10KB
            5% elephants -> 100MB
            Distribución normal: en el intervalo [μ -3σ, μ + 3σ] se encuentra comprendida, aproximadamente, el 99,74% de la distribución */
        unsigned int percentage = uniform(0,100); //Mice or elephant? (returns a value in the range [0,100)
        if(percentage < 95) //mice
        {
            smodel = "(Data Center -> mouse!)";
            flowSize = normal(10,3); //Media 10KB, desviación típica 3KB
            if (flowSize <= 1) flowSize = 1; //Mínimo 1KB
        }
        else //elephant
        {
            smodel = "(Data Center -> elephant!)";
            flowSize = normal(100000,10000); //Media 100MB, desviación típica 10MB
            if (flowSize <= 10000) flowSize = 10000; //Mínimo 10MB
        }
    }
    else
    {
        /*Tamaño: distribución Pareto (alfa=1,3) truncada.
            Mínimo: 8 MB
            Máximo (truncado): 8 GB
            Media: 34,7 MB
          Omnet++: http://www.omnetpp.org/doc/omnetpp/api/group__RandomNumbersCont.html#gcd24b1b115e588575bd4da594b8581fe
          Omnet++:http://www.omnetpp.org/listarchive/msg03242.php */
        smodel = "(JAC)";
        flowSize = pareto_shifted(1.3,8000,0); //Mínimo 8000, alpha = 1.3
        if (flowSize > 8000000) flowSize = 8000000; //Truncada si supera 8000000 - ###ERS: Puede suceder que el overflow dé un nuevo valor positivo pequeño y en este caso no se trunca, pero es un caso muy muy muy muy raro (de momento ignorado y no problemático ###)
    }
    adhocInfo[iSource].averageSizeSource = (adhocInfo[iSource].averageSizeSource*(adhocInfo[iSource].nFlowSource-1)+flowSize)/adhocInfo[iSource].nFlowSource;
    adhocInfo[iDestination].averageSizeDestination = (adhocInfo[iDestination].averageSizeDestination*(adhocInfo[iDestination].nFlowDestination-1)+flowSize)/adhocInfo[iDestination].nFlowDestination;
    ss2 << flowSize;
    flowInfo = flowInfo + "; " + ss2.str() + " KB";

    //Finally, we decide the frames size //TODO: ###ERS### En el modelo de JAC no es necesario, pero aquí sí, de momento está fijo a 500B (una opción sería hacerlo random o meterlo como parámetro)
    unsigned int frameSize = 1500; //Modificado a 1500 - 05/06/12

    EV << "  Flow info created! " << smodel << endl;
    EV << "    [" << adhocInfo[iSource].fullName << " (" << adhocInfo[iSource].ipAddress << ")" << " -> " << adhocInfo[iDestination].fullName << " (" << adhocInfo[iDestination].ipAddress << ")]" << endl;
    EV << "    Transfer Rate: " << transferRate << " (Kbps); Flow Size: " << flowSize << " (KB); Frame Size: " << frameSize << " (B)" << endl;

    //The generator indicates the parameters to the source and it's this host that will start and stop the flow
    //adhocInfo[iSource].pUdpFlowHost->startFlow(transferRate, flowSize*1000, frameSize, adhocInfo[iDestination].ipAddress); //Kbps, B(KB*1000), B, address
    adhocInfo[iSource].pUdpFlowHost->startFlow(transferRate, flowSize*1000, frameSize, adhocInfo[iDestination].ipAddress, adhocInfo[iSource].ipAddress); //Kbps, B(KB*1000), B, dst address, local address
    numSent++;

    int n = numSent; ss3 << n;
    flowInfo = ss3.str() + " - " + flowInfo;
    //generatedFlows.push_back(flowInfo);

    EV << "<-UDPFlowGenerator::startRandomFlow()" << endl;
}

} //namespace wapb
