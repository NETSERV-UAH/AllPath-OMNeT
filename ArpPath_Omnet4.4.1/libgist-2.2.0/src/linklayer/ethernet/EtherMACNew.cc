/*
 * Copyright (C) 2003 Andras Varga; CTIE, Monash University, Australia
 * Copyright (C) 2011 Zoltan Bojthe
 * Copyright (C) 2010 Diego Rivera (from EtherMACBaseNew)
 * Copyright (C) 2012 Elisa Rojas
 * Copyright (C) 2014 Isaias Martinez Yelmo (Inet 2.2.0 and Inet 2.3.0 adaptation)
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

#include <stdio.h>
#include <string.h>

#include "EtherMACNew.h"

#include "EtherFrame_m.h"
#include "Ethernet.h"
#include "Ieee802Ctrl_m.h"
#include "IPassiveQueue.h"

// TODO: there is some code that is pretty much the same as the one found in EtherMACFullDuplex.cc (e.g. EtherMAC::beginSendFrames)
// TODO: refactor using a statemachine that is present in a single function
// TODO: this helps understanding what interactions are there and how they affect the state


Define_Module(EtherMACNew);

void EtherMACNew::initialize(int stage)
{
    EtherMACBase::initialize(stage);

    if (stage == 0)
    {
        endRxMsg = new cMessage("EndReception", ENDRECEPTION);
        endBackoffMsg = new cMessage("EndBackoff", ENDBACKOFF);
        endJammingMsg = new cMessage("EndJamming", ENDJAMMING);

        // initialize state info
        backoffs = 0;
        numConcurrentTransmissions = 0;
        currentSendPkTreeID = 0;

        WATCH(backoffs);
        WATCH(numConcurrentTransmissions);

        /*EXTRA*/
        txQueueLimit = par("txQueueLimit");
        queueLength.setName("queue length");
        queueLengthInterval = par("queueLengthInterval"); //EXTRA
        queueLengthIntervalIter = queueLengthInterval; //EXTRA
        queueDroppedFrames = 0; //EXTRA
        queueDroppedFramesVector.setName("dropped frames"); //EXTRA
        WATCH(txQueueLimit);

        ARPReqRcvd = 0;
        /*EXTRA*/
    }
}

void EtherMACNew::initializeStatistics()
{
    EtherMACBase::initializeStatistics();

    framesSentInBurst = 0;
    bytesSentInBurst = 0;

    WATCH(framesSentInBurst);
    WATCH(bytesSentInBurst);

    // initialize statistics
    totalCollisionTime = 0.0;
    totalSuccessfulRxTxTime = 0.0;
    numCollisions = numBackoffs = 0;

    WATCH(numCollisions);
    WATCH(numBackoffs);

    collisionSignal = registerSignal("collision");
    backoffSignal = registerSignal("backoff");

    /*EXTRA*/
    latencyTimes.setName("latency times");
    averageLatency = 0.0;
    maxLatency = 0.0;
    minLatency = 100.0;
    WATCH(averageLatency);
    WATCH(maxLatency);
    WATCH(minLatency);
    nLatency = 0;

    processingTimes.setName("processing times");
    averageProcTime = 0.0;
    maxProcTime = 0.0;
    minProcTime = 100.0;
    WATCH(averageProcTime);
    WATCH(maxProcTime);
    WATCH(minProcTime);
    nProcTime = 0;

    statsTime = par("statsTime");
    debugTime = 0.0;
    WATCH(debugTime);
    /*EXTRA*/
}


void EtherMACNew::processFrameFromUpperLayer(EtherFrame *frame)
{
    if (frame->getByteLength() < MIN_ETHERNET_FRAME_BYTES)
        frame->setByteLength(MIN_ETHERNET_FRAME_BYTES);  // "padding"

    frame->setFrameByteLength(frame->getByteLength());

    EV << "Received frame from upper layer: " << frame << endl;

    emit(packetReceivedFromUpperSignal, frame);

    if (frame->getDest().equals(address))
    {
        error("Logic error: frame %s from higher layer has local MAC address as dest (%s)",
                frame->getFullName(), frame->getDest().str().c_str());
    }

    if (frame->getByteLength() > MAX_ETHERNET_FRAME_BYTES)
    {
        error("Packet from higher layer (%d bytes) exceeds maximum Ethernet frame size (%d)",
                (int)(frame->getByteLength()), MAX_ETHERNET_FRAME_BYTES);
    }

    if (!connected || disabled)
    {
        EV << (!connected ? "Interface is not connected" : "MAC is disabled") << " -- dropping packet " << frame << endl;
        emit(dropPkFromHLIfaceDownSignal, frame);
        numDroppedPkFromHLIfaceDown++;
        delete frame;

        requestNextFrameFromExtQueue();
        return;
    }

    // fill in src address if not set
    if (frame->getSrc().isUnspecified())
        frame->setSrc(address);

    bool isPauseFrame = (dynamic_cast<EtherPauseFrame*>(frame) != NULL);

    if (!isPauseFrame)
    {
        numFramesFromHL++;
        emit(rxPkFromHLSignal, frame);
    }

    if (txQueue.extQueue)
    {
        ASSERT(curTxFrame == NULL);
        curTxFrame = frame;
        fillIFGIfInBurst();
    }
    else
    {
        if (txQueue.innerQueue->isFull())
        {
            /*EXTRA
            error("txQueue length exceeds %d -- this is probably due to "
                  "a bogus app model generating excessive traffic "
                  "(or if this is normal, increase txQueueLimit!)",
                  txQueue.innerQueue->getQueueLimit());*/
            ev << "Queue limit reached -> discarding frame" << endl;
            queueDroppedFrames++;
            queueDroppedFramesVector.record(queueDroppedFrames);
            delete (frame);
            return;
            /*EXTRA*/
        }

        // store frame and possibly begin transmitting
        EV << "Frame " << frame << " arrived from higher layer, enqueueing\n";
        txQueue.innerQueue->insertFrame(frame);

        /*EXTRA*/
        if(!queueLengthIntervalIter)
        {
            queueLength.record(txQueue.innerQueue->length());
            queueLengthIntervalIter = queueLengthInterval;
        }
        else
            queueLengthIntervalIter--;
        /*EXTRA*/

        if (!curTxFrame && !txQueue.innerQueue->empty())
        {
            curTxFrame = (EtherFrame*)txQueue.innerQueue->pop();
            /*EXTRA*/ queueLength.record(txQueue.innerQueue->length()); /*EXTRA*/
        }
    }

    if ((duplexMode || receiveState == RX_IDLE_STATE) && transmitState == TX_IDLE_STATE)
    {
        EV << "No incoming carrier signals detected, frame clear to send\n";
        startFrameTransmission();
    }
}


void EtherMACNew::handleEndTxPeriod()
{
    // we only get here if transmission has finished successfully, without collision
    if (transmitState != TRANSMITTING_STATE || (!duplexMode && receiveState != RX_IDLE_STATE))
        error("End of transmission, and incorrect state detected");

    currentSendPkTreeID = 0;

    if (curTxFrame == NULL)
        error("Frame under transmission cannot be found");

    emit(packetSentToLowerSignal, curTxFrame);  //consider: emit with start time of frame

    if (dynamic_cast<EtherPauseFrame*>(curTxFrame) != NULL)
    {
        numPauseFramesSent++;
        emit(txPausePkUnitsSignal, ((EtherPauseFrame*)curTxFrame)->getPauseTime());
    }
    else
    {
        unsigned long curBytes = curTxFrame->getFrameByteLength();
        numFramesSent++;
        numBytesSent += curBytes;
        emit(txPkSignal, curTxFrame);
    }

    //TODO: EXTRA
    /*EXTRA*/
    //If not source host, we measure the processing times (usually at switches) with the timestamp
    if (!(dynamic_cast<EtherFrame*>(curTxFrame)->getSrc().equals(address)))
    {
        simtime_t procTime = simTime() - curTxFrame->getTimestamp();
        processingTimes.record(procTime);

        //Calculate average, max and min (set to 'procTime' if it's the first frame)
        if(simTime() > statsTime)
        {
            nProcTime++;
            if(nProcTime > 1) //Not first frame (antes numFramesReceivedOK)
            {
                averageProcTime = (averageProcTime*(nProcTime-1)+procTime)/nProcTime;
                if(procTime > maxProcTime) maxProcTime = procTime;
                if(procTime < minProcTime) minProcTime = procTime;
            }
            else //First frame
                averageProcTime = maxProcTime = minProcTime = procTime;

            EV << "!EtherMACNew processing times updated - avg: " << averageProcTime << "; max: " << maxProcTime << "; min: " << minProcTime << endl;

            if(procTime > 1)
                debugTime = simTime();
        }
    }
    /*EXTRA*/


    EV << "Transmission of " << curTxFrame << " successfully completed\n";
    delete curTxFrame;
    curTxFrame = NULL;
    lastTxFinishTime = simTime();
    getNextFrameFromQueue();

    // only count transmissions in totalSuccessfulRxTxTime if channel is half-duplex
    if (!duplexMode)
    {
        simtime_t dt = simTime() - channelBusySince;
        totalSuccessfulRxTxTime += dt;
    }

    backoffs = 0;

    // check for and obey received PAUSE frames after each transmission
    if (pauseUnitsRequested > 0)
    {
        // if we received a PAUSE frame recently, go into PAUSE state
        EV << "Going to PAUSE mode for " << pauseUnitsRequested << " time units\n";
        scheduleEndPausePeriod(pauseUnitsRequested);
        pauseUnitsRequested = 0;
    }
    else
    {
        EV << "Start IFG period\n";
        scheduleEndIFGPeriod();
        if (!txQueue.extQueue)
            fillIFGIfInBurst();
    }
}


void EtherMACNew::finish()
{
    EtherMACBase::finish();

    simtime_t t = simTime();
    simtime_t totalChannelIdleTime = t - totalSuccessfulRxTxTime - totalCollisionTime;
    recordScalar("rx channel idle (%)", 100*(totalChannelIdleTime/t));
    recordScalar("rx channel utilization (%)", 100*(totalSuccessfulRxTxTime/t));
    recordScalar("rx channel collision (%)", 100*(totalCollisionTime/t));
    recordScalar("collisions", numCollisions);
    recordScalar("backoffs", numBackoffs);

    /*EXTRA*/
    recordScalar("average latency", averageLatency);
    recordScalar("max latency", maxLatency);
    recordScalar("min latency", minLatency);
    recordScalar("average processing time", averageProcTime);
    recordScalar("max processing time", maxProcTime);
    recordScalar("min processing time", minProcTime);
    recordScalar("debug time", debugTime);
    recordScalar("ARP requests rcvd",ARPReqRcvd);
    /*EXTRA*/
}


void EtherMACNew::processReceivedDataFrame(EtherFrame *frame)
{
    // strip physical layer overhead (preamble, SFD, carrier extension) from frame
    frame->setByteLength(frame->getFrameByteLength());

    // statistics
    unsigned long curBytes = frame->getByteLength();
    numFramesReceivedOK++;
    numBytesReceivedOK += curBytes;
    emit(rxPkOkSignal, frame);

    //TODO: EXTRA
     /*EXTRA*/
     //Get creation time at destination host (to measure latencies)
     if (frame->getDest().equals(address))
     {
         simtime_t delay = simTime() - frame->getCreationTime();
         latencyTimes.record(delay);

         //Calculate average, max and min (set to 'delay' if it's the first frame)
         if(simTime() > statsTime)
         {
             nLatency++;
             if(nLatency > 1) //Not first frame (antes numFramesReceivedOK)
             {
                 averageLatency = (averageLatency*(nLatency-1)+delay)/nLatency;
                 if(delay > maxLatency) maxLatency = delay;
                 if(delay < minLatency) minLatency = delay;
             }
             else //First frame
                 averageLatency = maxLatency = minLatency = delay;

             EV << "!EtherMACNew latency times updated - avg: " << averageLatency << "; max: " << maxLatency << "; min: " << minLatency << endl;
         }
     }
     //If not destination host, we measure the processing times (usually at switches) with the timestamp
     else
     {
         frame->setTimestamp(); //Added timestamp once the frame is received
     }

     //Count ARP Request received
     if (!strcmp(frame->getFullName(),"arpREQ"))
         ++ARPReqRcvd;
     /*EXTRA*/

    numFramesPassedToHL++;
    emit(packetSentToUpperSignal, frame);
    // pass up to upper layer
    send(frame, "upperLayerOut");
}
