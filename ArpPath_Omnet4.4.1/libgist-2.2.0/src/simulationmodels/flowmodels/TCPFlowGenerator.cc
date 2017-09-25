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

#include <stdio.h>
#include "TCPFlowGenerator.h"

Define_Module(TCPFlowGenerator);

void TCPFlowGenerator::startRandomFlow()
{
    FlowGeneratorBase::startRandomFlow();

    EV << "->TCPFlowGenerator::startRandomFlow()" << endl;
/*
    std::string flowInfo; //String to be saved for final statistics about the flow generator
    std::stringstream ss1, ss2, ss3;

    //We randomly choose a source and a destination for the new flow
    int iSource=0, iDestination=0;
    getRandomSrcDstIndex(iSource, iDestination);
    hostInfo[iSource].nFlowSource++;
    hostInfo[iDestination].nFlowDestination++;
    flowInfo = hostInfo[iSource].fullName + "->" + hostInfo[iDestination].fullName;

    //Now a transfer rate and flow size, randomly as well - http://www.omnetpp.org/doc/omnetpp41/api/group__RandomNumbersCont.html

    unsigned int transferRate = intrand(10); // [0,10)
    if (transferRate < 3) transferRate = 500; //30%
    else if (transferRate < 9) transferRate = 1000; //60%
    else transferRate = 10000; //10%
    ss1 << transferRate;
    flowInfo = flowInfo + "; " + ss1.str() + " Kbps";


    unsigned long long flowSize = pareto_shifted(1.3,8000,0); //Mínimo 8000, alpha = 1.3
    if (flowSize > 8000000) flowSize = 8000000; //Truncada si supera 8000000 - ###ERS: Puede suceder que el overflow dé un nuevo valor positivo pequeño y en este caso no se trunca, pero es un caso muy muy muy muy raro (de momento ignorado y no problemático ###)
    hostInfo[iSource].averageSizeSource = (hostInfo[iSource].averageSizeSource*(hostInfo[iSource].nFlowSource-1)+flowSize)/hostInfo[iSource].nFlowSource;
    hostInfo[iDestination].averageSizeDestination = (hostInfo[iDestination].averageSizeDestination*(hostInfo[iDestination].nFlowDestination-1)+flowSize)/hostInfo[iDestination].nFlowDestination;
    ss2 << flowSize;
    flowInfo = flowInfo + "; " + ss2.str() + " KB";

    //Finally, we decide the frames size //TODO: ###ERS### En el modelo de JAC no es necesario, pero aquí sí, de momento está fijo a 500B (una opción sería hacerlo random o meterlo como parámetro)
    unsigned int frameSize = 1500; //Modificado a 1500 - 05/06/12

    EV << "  Flow info created!" << endl;
    EV << "    [" << hostInfo[iSource].fullName << " (" << hostInfo[iSource].ipAddress << ")" << " -> " << hostInfo[iDestination].fullName << " (" << hostInfo[iDestination].ipAddress << ")]" << endl;
    EV << "    Transfer Rate: " << transferRate << " (Kbps); Flow Size: " << flowSize << " (KB); Frame Size: " << frameSize << " (B)" << endl;

    //The generator indicates the parameters to the source and it's this host that will start and stop the flow
    hostInfo[iSource].pUdpFlowHost->startFlow(transferRate, flowSize*1000, frameSize, hostInfo[iDestination].ipAddress); //Kbps, B(KB*1000), B, address
    numSent++;

    int n = numSent; ss3 << n;
    flowInfo = ss3.str() + " - " + flowInfo;
    //generatedFlows.push_back(flowInfo);

     */

    EV << "<-TCPFlowGenerator::startRandomFlow()" << endl;
}

