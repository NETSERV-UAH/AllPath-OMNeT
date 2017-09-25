/*
 * Copyright (C) 2003 Andras Varga; CTIE, Monash University, Australia
 * Copyright (C) 2010 Diego Rivera (from EtherMACBaseNew)
 * Copyright (C) 2012 Elisa Rojas
 * Copyright (C) 2014 Isaias Martinez Yelmo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __INET_ETHERMAC_NEW_H
#define __INET_ETHERMAC_NEW_H

#include "INETDefs.h"

#include "EtherMAC.h" //EXTRA-IMY


class EtherJam;
class EtherPauseFrame;
class IPassiveQueue;

/**
 * Ethernet MAC module which supports both half-duplex (CSMA/CD) and full-duplex
 * operation. (See also EtherMACFullDuplex which has a considerably smaller
 * code with all the CSMA/CD complexity removed.)
 *
 * See NED file for more details.
 */
class INET_API EtherMACNew : public EtherMAC
{

  /* EXTRA-IMY functions modified by libgist */
  protected:
    virtual void initialize(int stage);
    virtual void initializeStatistics();
    virtual void finish();

    // event handlers
    virtual void handleEndTxPeriod();
    // helpers
    virtual void processFrameFromUpperLayer(EtherFrame *msg);
    virtual void processReceivedDataFrame(EtherFrame *frame);
    /* EXTRA-IMY */


  protected:

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


