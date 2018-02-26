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

void UDPFlowGenerator::initialize(int stage)
{
    EV << "->FlowGenerator::initialize()" << endl;
    FlowGeneratorBase::initialize(stage);

    trafficType = par("trafficType");
    EV << "<-FlowGenerator::initialize()" << endl;

}

void UDPFlowGenerator::startRandomFlow()
{
    FlowGeneratorBase::startRandomFlow();

    EV << "->UDPFlowGenerator::startRandomFlow()" << endl;

    std::string flowInfo, smodel; //String to be saved for final statistics about the flow generator
    std::stringstream ss1, ss2, ss3, ss4;

    //We randomly choose a source and a destination for the new flow
    int iSource=0, iDestination=0;
    getRandomSrcDstIndex(iSource, iDestination);
    adhocInfo[iSource].nFlowSource++;
    adhocInfo[iDestination].nFlowDestination++;
    flowInfo = adhocInfo[iSource].fullName + "->" + adhocInfo[iDestination].fullName;

    //Now a transfer rate and flow size,

    double flowSize, transferRate;
    unsigned int frameSize = 0;
    unsigned long long numPackets = 0;
    int sessionInterval = (int)par("sessionInterval");
    if(strcmp(trafficType, "S_DATA") == 0)  //S_DATA traffic
    {
        /*
         S_DATA : packet size is 64 Bytes, Inter-arrival time of data packets is 20 ms
         then 25.6 kbps
         */
        transferRate = 25.6;
        ss1 << transferRate;
        flowInfo = flowInfo + "; " + ss1.str() + " Kbps";

        /*S_DATA: packet size is 64 Bytes, Inter-arrival time of data packets is 20 ms
         * then, 50 packets per secend
         * */
        smodel = "(Ad-hoc -> S_DATA!)";
        flowSize = sessionInterval * 50 * 64; //Bytes
        //Finally, we decide the frames size
        frameSize = 64; //Bytes
        numPackets = flowSize / frameSize;
    }else if(strcmp(trafficType, "VOICE") == 0) //VOICE traffic
    {
        /*
         * VOICE : sampling frequency is 8KHz, then 8000 samples (1 Byte * 8000) in each second, 64 kbps
         *
         */
        transferRate = 64;
        ss1 << transferRate;
        flowInfo = flowInfo + "; " + ss1.str() + " Kbps";

        /*VOICE : one packet is produced every 20 ms
         * then, 50 packets per secend
         * */
        smodel = "(Ad-hoc -> VOICE!)";
        flowSize = sessionInterval * 50 * 160; //Bytes

        // 8 KB is produced every 1 second, then packet size (in 20 ms) = 160 Bytes
        frameSize = 160; //Bytes
        numPackets = flowSize / frameSize;
    }else if(strcmp(trafficType, "CUSTOMIZED") == 0)  //CUSTOMIZED traffic
    {
        /*
         S_DATA : packet size is 64 Bytes, Inter-arrival time of data packets is 20 ms
         then 25.6 kbps
         */
        transferRate = 25.6;
        ss1 << transferRate;
        flowInfo = flowInfo + "; " + ss1.str() + " Kbps";

        /*S_DATA: packet size is 64 Bytes, Inter-arrival time of data packets is 20 ms
         * then, 50 packets per secend
         * */
        smodel = "(Ad-hoc -> CUSTOMIZED!)";
        flowSize = sessionInterval * 50 * 64; //Bytes
        //Finally, we decide the frames size
        frameSize = par("packetsize"); //256;//512; //1500; //64 //Modificado a 1500 - 05/06/12
        numPackets = flowSize / frameSize;
    } else
        throw cRuntimeError("Type of traffic is undefined");

    adhocInfo[iSource].averageSizeSource = (adhocInfo[iSource].averageSizeSource*(adhocInfo[iSource].nFlowSource-1)+flowSize)/adhocInfo[iSource].nFlowSource;
    adhocInfo[iDestination].averageSizeDestination = (adhocInfo[iDestination].averageSizeDestination*(adhocInfo[iDestination].nFlowDestination-1)+flowSize)/adhocInfo[iDestination].nFlowDestination;
    ss2 << flowSize;
    flowInfo = flowInfo + "; " + ss2.str() + " B";

    EV << "  Flow info created! " << smodel << endl;
    EV << "    [" << adhocInfo[iSource].fullName << " (" << adhocInfo[iSource].ipAddress << ")" << " -> " << adhocInfo[iDestination].fullName << " (" << adhocInfo[iDestination].ipAddress << ")]" << endl;
    EV << "    Transfer Rate: " << transferRate << " (Kbps); Flow Size: " << flowSize << " (B); Frame Size: " << frameSize << " (B)" << endl;

    //The generator indicates the parameters to the source and it's this host that will start and stop the flow
    //adhocInfo[iSource].pUdpFlowHost->startFlow(transferRate, flowSize*1000, frameSize, adhocInfo[iDestination].ipAddress); //Kbps, B(KB*1000), B, address
    adhocInfo[iSource].pUdpFlowHost->startFlow(transferRate, flowSize, frameSize, adhocInfo[iDestination].ipAddress, adhocInfo[iSource].ipAddress); //Kbps, B, B, dst address, local address
    numSent++;

    int n = numSent; ss3 << n;
    simtime_t genTime = simTime();
    ss4 << sessionInterval + genTime;
    flowInfo = ss3.str() + " - " + flowInfo + "; start at t = " + genTime.str() + " s; end at t = " + ss4.str();
    generatedFlows.push_back(flowInfo);

    EV << "<-UDPFlowGenerator::startRandomFlow()" << endl;
}

} //namespace wapb
