//
// Copyright (C) 2006 Levente Meszaros
// Copyright (C) 2013 Elisa Rojas (same changes than EtherMACNew named with EXTRA)
//
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

#ifndef __INET_ETHER_DUPLEX_MAC_H
#define __INET_ETHER_DUPLEX_MAC_H

#include "INETDefs.h"

#include "EtherMACBase.h"

/**
 * A simplified version of EtherMAC. Since modern Ethernets typically
 * operate over duplex links where's no contention, the original CSMA/CD
 * algorithm is no longer needed. This simplified implementation doesn't
 * contain CSMA/CD, frames are just simply queued up and sent out one by one.
 */
class INET_API EtherMACFullDuplexNew : public EtherMACBase
{
  public:
    EtherMACFullDuplexNew();

  protected:
    virtual void initialize();
    virtual void initializeStatistics();
    virtual void initializeFlags();
    virtual void handleMessage(cMessage *msg);

    // finish
    virtual void finish();

    // event handlers
    virtual void handleEndIFGPeriod();
    virtual void handleEndTxPeriod();
    virtual void handleEndPausePeriod();
    virtual void handleSelfMessage(cMessage *msg);

    // helpers
    virtual void startFrameTransmission();
    virtual void processFrameFromUpperLayer(EtherFrame *frame);
    virtual void processMsgFromNetwork(EtherTraffic *msg);
    virtual void processReceivedDataFrame(EtherFrame *frame);
    virtual void processPauseCommand(int pauseUnits);
    virtual void scheduleEndIFGPeriod();
    virtual void scheduleEndPausePeriod(int pauseUnits);
    virtual void beginSendFrames();


    // statistics
    simtime_t totalSuccessfulRxTime; // total duration of successful transmissions on channel

    /*EXTRA*/
    int txQueueLimit;                   // max queue length (for not causing a runtime error as in inet, but causing frames to be dropped)
    cOutVector queueLength;                 // queue length vector
    unsigned long queueLengthInterval;      // queue length interval of recording (for queueLenght)
    unsigned long queueLengthIntervalIter;  // queue length interval of recording iterator
    unsigned long queueDroppedFrames;       // number of dropped frames
    cOutVector queueDroppedFramesVector;    // vector containing

    //Variables to measure latencies between hosts
    cOutVector latencyTimes;
    simtime_t averageLatency;
    simtime_t minLatency;
    simtime_t maxLatency;
    unsigned long nLatency;

    //Variables to measure switch processing times (queue + processing ¿+ fwd?)
    cOutVector processingTimes;
    simtime_t averageProcTime;
    simtime_t minProcTime;
    simtime_t maxProcTime;
    unsigned long nProcTime;

    //Variable that indicates the time in which the latencies and processing times start to be measured
    simtime_t statsTime;
    simtime_t debugTime; //Debug

    int ARPReqRcvd;
    /*EXTRA*/
};

#endif

