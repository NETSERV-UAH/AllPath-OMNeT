/*
 * Copyright (C) 2003 Andras Varga; CTIE, Monash University, Australia
 * Copyright (C) 2010 Diego Rivera (from EtherMACBaseNew)
 * Copyright (C) 2012 Elisa Rojas
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

#include "EtherMACBase.h"


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
class INET_API EtherMACNew : public EtherMACBase
{
  public:
    EtherMACNew();
    virtual ~EtherMACNew();

  protected:
    virtual void initialize();
    virtual void initializeFlags();
    virtual void initializeStatistics();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

  protected:
    // states
    int numConcurrentTransmissions;    // number of colliding frames -- we must receive this many jams (caches endRxTimeList.size())
    int  backoffs;                     // value of backoff for exponential back-off algorithm
    long currentSendPkTreeID;

    // other variables
    EtherTraffic *frameBeingReceived;
    cMessage *endRxMsg, *endBackoffMsg, *endJammingMsg;

    // list of receptions during reconnect state; an additional special entry (with packetTreeId=-1)
    // stores the end time of the reconnect state
    struct PkIdRxTime
    {
        long packetTreeId;             // >=0: tree ID of packet being received; -1: this is a special entry that stores the end time of the reconnect state
        simtime_t endTime;             // end of reception
        PkIdRxTime(long id, simtime_t time) {packetTreeId=id; endTime = time;}
    };
    typedef std::list<PkIdRxTime> EndRxTimeList;
    EndRxTimeList endRxTimeList;       // list of incoming packets, ordered by endTime

    // statistics
    simtime_t totalCollisionTime;      // total duration of collisions on channel
    simtime_t totalSuccessfulRxTxTime; // total duration of successful transmissions on channel
    simtime_t channelBusySince;        // needed for computing totalCollisionTime/totalSuccessfulRxTxTime
    unsigned long numCollisions;       // collisions (NOT number of collided frames!) sensed
    unsigned long numBackoffs;         // number of retransmissions
    unsigned int  framesSentInBurst;   // Number of frames send out in current frame burst
    long bytesSentInBurst;             // Number of bytes transmitted in current frame burst

    static simsignal_t collisionSignal;
    static simsignal_t backoffSignal;

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

  protected:
    // event handlers
    virtual void handleSelfMessage(cMessage *msg);
    virtual void handleEndIFGPeriod();
    virtual void handleEndPausePeriod();
    virtual void handleEndTxPeriod();
    virtual void handleEndRxPeriod();
    virtual void handleEndBackoffPeriod();
    virtual void handleEndJammingPeriod();
    virtual void handleRetransmission();

    // helpers
    virtual void readChannelParameters(bool errorWhenAsymmetric);
    virtual void processFrameFromUpperLayer(EtherFrame *msg);
    virtual void processMsgFromNetwork(EtherTraffic *msg);
    virtual void scheduleEndIFGPeriod();
    virtual void scheduleEndTxPeriod(EtherFrame *);
    virtual void scheduleEndRxPeriod(EtherTraffic *);
    virtual void scheduleEndPausePeriod(int pauseUnits);
    virtual void beginSendFrames();
    virtual void sendJamSignal();
    virtual void startFrameTransmission();
    virtual void frameReceptionComplete();
    virtual void processReceivedDataFrame(EtherFrame *frame);
    virtual void processReceivedJam(EtherJam *jam);
    virtual void processReceivedPauseFrame(EtherPauseFrame *frame);
    virtual void processConnectDisconnect();
    virtual void addReception(simtime_t endRxTime);
    virtual void addReceptionInReconnectState(long id, simtime_t endRxTime);
    virtual void processDetectedCollision();

    virtual void printState();
};

#endif


