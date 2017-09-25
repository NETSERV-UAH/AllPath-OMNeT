/*
 * Copyright (C) 2012/2013 Elisa Rojas; GIST, University of Alcala, Spain
 *              based on work of (C) 2010 Diego Rivera (for previous versions of Omnet and the inet framework.
 *
 * MACRelayUnitAPB module defines the ARP-Path switch behaviour (SAu version). It is based on the MACRelayUnitNP module from the Inet Framework.
 * Since the MACRelayUnitAPB couldn't be written as an extension of MACRelayUnitNP (due to the characteristics of it),
 * but as an extended copy of that module, any update of the inet framework MACRelayUnitNP should be made in this module as well.
 * We've tried to keep this module as organized as possible, so that future updates are easy.
 * LAST UPDATE OF THE INET FRAMEWORK: inet2.0 @ 14/09/2012
*/

/*NOTA: Las impresiones en pantalla (tablas, etc...) están ajustadas para que salgan correctamente en la interfaz visual, no en línea de comandos... (no se pueden ajustar para ambos a la vez)*/

#include "MACRelayUnitAPB.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

Define_Module(MACRelayUnitAPB);

//class constructor
MACRelayUnitAPB::MACRelayUnitAPB()
{
    endProcEvents = NULL;
    numCPUs = 0;
}

//class destructor
MACRelayUnitAPB::~MACRelayUnitAPB()
{
    for (int i=0; i<numCPUs; i++)
    {
        cMessage *endProcEvent = endProcEvents[i];
        EtherFrame *etherFrame = (EtherFrame *)endProcEvent->getContextPointer();
        if (etherFrame)
        {
            endProcEvent->setContextPointer(NULL);
            delete etherFrame;
        }
        cancelAndDelete(endProcEvent);
    }
    delete [] endProcEvents;
}

//Initialize variables at simulation start.
void MACRelayUnitAPB::initialize(int stage)
{
    /*MACRelayUnitNP.h*/
    //     MACRelayUnitBase::initialize();
    MACRelayUnitBase::initialize(stage);

    if (stage != 0)
        return;

    bufferLevel.setName("buffer level");
    queue.setName("queue");

    numProcessedFrames = numDroppedFrames = 0;
    WATCH(numProcessedFrames);
    WATCH(numDroppedFrames);

    numCPUs = par("numCPUs");

    protocolVersion = par("protocolVersion");
    processingTime = par("processingTime");
    bufferSize = par("bufferSize");
    highWatermark = par("highWatermark");
    pauseUnits = par("pauseUnits");

    bufferUsed = 0;
    WATCH(bufferUsed);

    endProcEvents = new cMessage *[numCPUs];
    for (int i=0; i<numCPUs; i++)
    {
        char msgname[40];
        sprintf(msgname, "endProcessing-cpu%d", i);
        endProcEvents[i] = new cMessage(msgname, i);
    }

    /* EXTRA-IMY */
    /*MACRelayUnitAPB.h*/
    // Parameters assigned to variables:
    blockingTime = par("blockingTime");
    helloTime = par("helloTime");
    repairingTime = par("repairingTime");
    generateSwitchAddress();
    repairingAddress.setAddress(ARPPATH_MCAST_ADD);
    repairType = par("repairType");

    portLinkDown = par("portLinkDown").stringValue(); //Ports which links are down and times (port number ':' init time '-' end time, and separated by ';')
    switchDown = par("switchDown").stringValue(); //Times in which the switch will be down (init time '-' end time, and separated by ';')
    switchIsDown = false; //Initially UP

    relayQueueLimit = par("relayQueueLimit");
    queueLength.setName("queue length");

    broadcastSeed = par("broadcastSeed");
    srand (broadcastSeed); //Seed for the random functions

    multicastActive = par("multicastActive");

    ARPReqRcvd = ARPRepRcvd = 0;
    nRepairStarted = HelloRcvd = PathFailRcvd = PathRequestRcvd = PathReplyRcvd = LinkFailRcvd = LinkReplyRcvd = 0;
    lostPackets = 0;
    WATCH(ARPReqRcvd);
    WATCH(ARPRepRcvd);
    WATCH(nRepairStarted);
    WATCH(HelloRcvd);
    WATCH(PathFailRcvd);
    WATCH(PathRequestRcvd);
    WATCH(PathReplyRcvd);
    WATCH(LinkFailRcvd);
    WATCH(LinkReplyRcvd);
    WATCH(lostPackets);

    //PathFail
    averagePFTime = 0.0;
    maxPFTime = 0.0;
    minPFTime = 100.0;
    WATCH(averagePFTime);
    WATCH(maxPFTime);
    WATCH(minPFTime);
    nPF = 0;
    //PathRequest
    averagePRqTime = 0.0;
    maxPRqTime = 0.0;
    minPRqTime = 100.0;
    WATCH(averagePRqTime);
    WATCH(maxPRqTime);
    WATCH(minPRqTime);
    nPRq = 0;
    //PathReply
    averagePRyTime = 0.0;
    maxPRyTime = 0.0;
    minPRyTime = 100.0;
    WATCH(averagePRyTime);
    WATCH(maxPRyTime);
    WATCH(minPRyTime);
    nPRy = 0;
    //LinkFail
    averageLFTime = 0.0;
    maxLFTime = 0.0;
    minLFTime = 100.0;
    WATCH(averageLFTime);
    WATCH(maxLFTime);
    WATCH(minLFTime);
    nLF = nLFi = 0;
    //PathReply
    averageLRTime = 0.0;
    maxLRTime = 0.0;
    minLRTime = 100.0;
    WATCH(averageLRTime);
    WATCH(maxLRTime);
    WATCH(minLRTime);
    nLR = nLRi = 0;
    //RepairTimes
    averageRepairTime = 0.0;
    maxRepairTime = 0.0;
    minRepairTime = 100.0;
    WATCH(averageRepairTime);
    WATCH(maxRepairTime);
    WATCH(minRepairTime);
    nRepairFinished = 0;

    generateParametersTablesAndEvents(); //Initialize tables with the given parameters and generate the corresponding events (link and switch failure events)
    printLinkDownTable(); //Print the table just created by the link failure parameters
    printSwitchDownStruct(); //Print the struct just created by the switch failure parameters

    /* EXTRA-IMY */

    EV << "Parameters of (" << getClassName() << ") " << getFullPath() << "\n";
    EV << "  number of processors: " << numCPUs << "\n";
    EV << "  processing time: " << processingTime << "\n";
    EV << "  ports: " << numPorts << "\n";
    EV << "  buffer size: " << bufferSize << "\n";
    EV << "  address table size: " << addressTableSize << "\n";
    EV << "  aging time: " << agingTime << "\n";
    EV << "  high watermark: " << highWatermark << "\n";
    EV << "  pause time: " << pauseUnits << "\n";

    /* EXTRA-IMY */
    EV << "  --- ARP-Path parameters --- " << endl;
    EV << "  protocol version: " << protocolVersion << endl;
    EV << "  blockingTime: " << blockingTime << endl;
    EV << "  repairingTime: " << repairingTime << endl;
    EV << "  myAddress: " << myAddress << endl;
    EV << "  repairingAddress: " << repairingAddress << endl;
    EV << "  repairType: " << repairType << endl;
    EV << endl;

    //Hello! events are only generated if repairType!=0, i.e. if some repair method will be needed during execution
    if(repairType != 0)
    {
        sendHello(); //Envío inicial de mensajes Hello (se podría enviar con algún margen de tiempo, ahora se envía en el segundo 0
        cMessage *timer = new cMessage("Hello!");
        scheduleAt(simTime()+helloTime, timer); //Next Hello broadcasting
    }
    /* EXTRA-IMY */
}

//Method to handle each incoming message (including self messages).
void MACRelayUnitAPB::handleMessage(cMessage *msg)
{
    EV << "->MACRelayUnitAPB::handleMessage()" << endl;
    if (!msg->isSelfMessage())
    {
        // Frame received from MAC unit
        handleIncomingFrame(check_and_cast<EtherFrame *>(msg));
    }
    else
    {
        std::string eventName = msg->getName();
        //Hello message
        if(!eventName.compare("Hello!"))
        {
            EV << "Hello! event to be handled..." << endl;

            //Send Hello (if switch is UP), then reschedule next sending
            if(!switchIsDown)
                sendHello();
            scheduleAt(simTime()+helloTime, msg); //Next Hello broadcasting
        }
        //Link DOWN (only used when repairType = 3)
        else if(!eventName.compare("LinkDown!"))
        {
            EV << "LinkDown! event to be handled..." << endl;
            startPathRepair();
            delete msg; //Delete event message
        } //TODO: Se podría añadir un LinkUp! para mandar Hello al reiniciar, pero añade coste a las simulaciones y de momento no es necesario
        //Switch DOWN
        else if(!eventName.compare("SwitchDown!"))
        {
            EV << "SwitchDown! event to be handled..." << endl;

            //Reset all tables and set switch state to DOWN
            removeAllEntriesFromTables();
            switchIsDown = true;
            delete msg; //Delete event message
        }
        //Switch UP
        else if(!eventName.compare("SwitchUp!"))
        {
            EV << "SwitchUp! event to be handled..." << endl;

            //Send after-initialization Hello and set switch state back to UP
            sendHello();
            switchIsDown = false;
            delete msg; //Delete event message
        }
        //Frame ready from processing
        else
        {
            // Self message signal used to indicate a frame has finished processing
            processFrame(msg);
        }
    }
}

//Method to record scalar values at the end of simulation.
void MACRelayUnitAPB::finish()
{
    recordScalar("processed frames", numProcessedFrames);
    recordScalar("dropped frames", numDroppedFrames);

    recordScalar("ARP requests rcvd",ARPReqRcvd);
    recordScalar("ARP replies rcvd",ARPRepRcvd);

    recordScalar("number of repairs started",nRepairStarted);
    recordScalar("Hellos rcvd",HelloRcvd);
    recordScalar("PathFails rcvd",PathFailRcvd);
    recordScalar("PathRequests rcvd",PathRequestRcvd);
    recordScalar("PathReplies rcvd",PathReplyRcvd);
    recordScalar("LinkFails rcvd",LinkFailRcvd);
    recordScalar("LinkReplies rcvd",LinkReplyRcvd);
    recordScalar("number of lost packets", lostPackets);

    //PathFail, PathRequest, PathReply
    recordScalar("average PathFail time",averagePFTime);
    recordScalar("max PathFail time",maxPFTime);
    recordScalar("min PathFail time",minPFTime);
    recordScalar("PathFails rcvd at destination",nPF);
    recordScalar("average PathRequest time",averagePRqTime);
    recordScalar("max PathRequest time",maxPRqTime);
    recordScalar("min PathRequest time",minPRqTime);
    recordScalar("PathRequests rcvd at destination",nPRq);
    recordScalar("average PathReply time",averagePRyTime);
    recordScalar("max PathReply time",maxPRyTime);
    recordScalar("min PathReply time",minPRyTime);
    recordScalar("PathReplies rcvd at destination",nPRy);
    //LinkFail, LinkReply
    recordScalar("average LinkFail time",averageLFTime);
    recordScalar("max LinkFail time",maxLFTime);
    recordScalar("min LinkFail time",minLFTime);
    recordScalar("LinkFails rcvd at destination",nLF);
    recordScalar("addresses repaired by LinkFails rcvd at destination",nLFi);
    recordScalar("average LinkReply time",averageLRTime);
    recordScalar("max LinkReply time",maxLRTime);
    recordScalar("min LinkReply time",minLRTime);
    recordScalar("LinkReplies rcvd at destination",nLR);
    recordScalar("addresses repaired by LinkReplies rcvd at destination",nLRi);
    //Repair time - TOTAL
    recordScalar("average repair time",averageRepairTime);
    recordScalar("max repair time",maxRepairTime);
    recordScalar("min repair time",minRepairTime);
    recordScalar("number of repairs finished",nRepairFinished);

    //for each switch from the net, its address table is printed, so each established path can be easily tracked.
    printTables(); //printAddressTable(); es el método original de MACRelayUnitBase
}

//Method to handle incoming frames, enqueueing, dropping or processing them.
void MACRelayUnitAPB::handleIncomingFrame(EtherFrame *frame)
{
    queueLength.record(queue.length()); //Added the queue length to the original function (that's why parent is not called, since it's modified) - TO BE UPDATED WITH INET (i.e. if the inet framework is modified)

    // If buffer not full, insert payload frame into buffer and process the frame in parallel.

    long length = frame->getByteLength();
    if (length + bufferUsed < bufferSize)
    {
        bufferUsed += length;

        // send PAUSE if above watermark
        if (pauseUnits>0 && highWatermark>0 && bufferUsed>=highWatermark)
            sendPauseFramesIfNeeded(pauseUnits);

        // assign frame to a free CPU (if there is one)
        int i;
        for (i=0; i<numCPUs; i++)
            if (!endProcEvents[i]->isScheduled())
                break;
        ev << "RELAY UNIT QUEUE (added to INET)" << endl;
        if (i==numCPUs)
        {
            ev << "RELAY UNIT QUEUE LENGTH (before enqueueing): " << queue.length() << endl;
            ev << "RELAY UNIT QUEUE LIMIT: " << relayQueueLimit << endl;
            if (queue.length()>relayQueueLimit)
            {
                EV << "RELAY UNIT QUEUE: dropping frame" << endl;
                delete (frame);
                ++numDroppedFrames;
            }
            else
            {
                EV << "All CPUs busy, enqueueing incoming frame " << frame << " for later processing\n";
                queue.insert(frame);
                ev << "RELAY UNIT QUEUE LENGTH (after enqueueing): " << queue.length() << endl;
            }
        }
        else
        {
            EV << "Idle CPU-" << i << " starting processing of incoming frame " << frame << endl;
            cMessage *msg = endProcEvents[i];
            ASSERT(msg->getContextPointer()==NULL);
            msg->setContextPointer(frame);
            scheduleAt(simTime() + processingTime, msg);
        }
    }
    // Drop the frame and record the number of dropped frames
    else
    {
        EV << "Buffer full, dropping frame " << frame << endl;
        delete frame;
        ++numDroppedFrames;
    }

    // Record statistics of buffer usage levels
    bufferLevel.record(bufferUsed);
}

//Method to process frames, by using the "handleAndDispatchFrame" method.
void MACRelayUnitAPB::processFrame(cMessage *msg)
{
    int cpu = msg->getKind();
    EtherFrame *frame = (EtherFrame *) msg->getContextPointer();
    ASSERT(frame);
    msg->setContextPointer(NULL);
    long length = frame->getByteLength();
    int inputport = frame->getArrivalGate()->getIndex();

    EV << "CPU-" << cpu << " completed processing of frame " << frame << endl;

    handleAndDispatchFrame(frame, inputport);
    printAddressTable();

    bufferUsed -= length;
    bufferLevel.record(bufferUsed);

    numProcessedFrames++;

    // Process next frame in queue if they are pending
    if (!queue.empty())
    {
        EtherFrame *newframe = (EtherFrame *) queue.pop();
        msg->setContextPointer(newframe);
        EV << "CPU-" << cpu << " starting processing of frame " << newframe << endl;
        scheduleAt(simTime()+processingTime, msg);
    }
    else
    {
        EV << "CPU-" << cpu << " idle\n";
    }
}

//Method that includes the learning and frame dispatching process.
//it removes obsolete from learning table and then the ARP-Path behaviour is
//implemented, to learn from ARP packets,
void MACRelayUnitAPB::handleAndDispatchFrame(EtherFrame *frame, int inputport)
{
    EV << "->MACRelayUnitAPB::handleAndDispatchFrame()" << endl;
    if(protocolVersion == 2)
        handleAndDispatchFrameV2(frame, inputport);
    else if(protocolVersion == 3)
        handleAndDispatchFrameV3(frame, inputport);
    else
        EV << "  Protocol version " << protocolVersion << " is not yet implemented! ERROR!" << endl;
    EV << "<-MACRelayUnitAPB::handleAndDispatchFrame()" << endl;
}

//Implements the logic of "ARP-Path unidireccional (v2)"
void MACRelayUnitAPB::handleAndDispatchFrameV2(EtherFrame *frame, int inputport)
{
    EV << "  Protocol version: 2 -> ARP-Path Unidirectional (v2)" << endl;

    MACAddress srcAddress = frame->getSrc();
    MACAddress dstAddress = frame->getDest();
    bool entryExists;
    EV << "  DEST ADDRESS: " << dstAddress << endl;
    EV << "  SRC ADDRESS: " << srcAddress << endl;
    EV << "  FRAME: " << frame << endl;
    EV << "  INPUTPORT:  " << inputport << endl;
    updateTables(); //Updates all entries from all the tables in the switch (aged entries, failed links, etc...)

    /*Once we know source and address, the frame is processed differently in 4 cases: 2 (bcast + ucast) for ARP
     * (in which learning is applied) + 2 (bcast/mcast + ucast) for the rest (in which learning is not applied)
     */
    //1º) If ARP Request (ARP + Broadcast)
    if(!strcmp("arpREQ",frame->getFullName()))
    {
        EV << "    Frame is ARP Request" << endl;
        ARPReqRcvd++;

        //Update + Print Route
        MACRelayUnitRoute * pRelayUnitRoute = check_and_cast<MACRelayUnitRoute *>(this->getParentModule()->getSubmodule("relayUnitRoute"));
        pRelayUnitRoute->updateAndSaveRoute(frame);

        //Learning + Blocking/Broadcasting
        AddressEntry entry;
        entryExists = findEntry(srcAddress, entry);
        if (entryExists)
        {
            if(entry.status == locked)
            {
                EV << "      Entry found -> 'locked' status..." << endl;
                if (inputport == entry.port)
                {
                    EV << "        ...refresh + broadcast!" << endl;
                    entry.inTime = simTime(); //Refresco
                    addressTable[srcAddress] = entry;
                    broadcastFrame(frame, inputport);
                }
                else
                {
                    EV << "        ...discard!" << endl;
                    delete (frame);
                }
            }
            else
            {
                EV << "      Entry found -> 'learnt' status goes 'locked' as a new entry..." << endl;
                addEntry(entry, srcAddress, inputport);

                EV << "        ...broadcast!" << endl;
                broadcastFrame(frame,inputport);
            }
        }
        else
        {
            EV << "      Entry not found -> Add new 'locked' entry..." << endl;
            addEntry(entry, srcAddress, inputport);

            EV << "        ...broadcast!" << endl;
            broadcastFrame(frame,inputport);
        }

    }
    //2º) If ARP Reply (ARP + Unicast)
    else if(!strcmp("arpREPLY",frame->getFullName()))
    {
        EV << "    Frame is ARP Reply" << endl;
        ARPRepRcvd++;

        //Update + Print Route
        MACRelayUnitRoute * pRelayUnitRoute = check_and_cast<MACRelayUnitRoute *>(this->getParentModule()->getSubmodule("relayUnitRoute"));
        pRelayUnitRoute->updateAndSaveRoute(frame);

        //Learning
        AddressEntry entry;
        entryExists = findEntry(srcAddress, entry);
        if (entryExists)
        {
            if(entry.status == locked)
            {
                EV << "      Entry found -> 'locked' status..." << endl;
                if (inputport == entry.port)
                {
                    entry.inTime = simTime(); //Refresco
                    addressTable[srcAddress] = entry;
                }
                //else //No hacemos nada si no coincide (forward as if entry port was the same, but no refresh)
            }
            else
            {
                EV << "      Entry found -> 'learnt' status goes 'locked' as a new entry..." << endl;
                addEntry(entry, srcAddress, inputport);
            }
        }
        else
        {
            EV << "      Entry not found -> Add new 'locked' entry..." << endl;
            addEntry(entry, srcAddress, inputport);
        }
        //TODO:Se podría pensar sobre si añadir las entradas como 'locked' (como ahora) o directamente 'learnt'

        //Forwarding
        AddressEntry destEntry;
        entryExists = findEntry(dstAddress, destEntry);
        if (entryExists)
        {
            EV << "      Output entry found at port: " << destEntry.port << endl;
            EV << "        ...refresh destination + forward" << endl;
            if(destEntry.status == learnt)
            {
                destEntry.inTime = simTime(); //Refresco sólo si la entrada ya está en estado 'learnt' (si no, estaríamos refrescando el estado 'locked' y eso no se hace con unicast)
                addressTable[dstAddress] = destEntry;
            }
            send(frame,"lowerLayerOut",destEntry.port);
        }
        else
        {
            EV << "      Output entry not found -> Starting repair..." << endl;
            startPathRepair(srcAddress,dstAddress);
            lostPackets++;
            delete(frame); //Original frame is discarded/lost
        }
    }
    //3º) If not ARP but BROADCAST/MULTICAST (Broadcast/Multicast)
    else if(!dstAddress.compareTo(repairingAddress)) //TODO: repairingAddress debería ser multicast e ir dentro del siguiente 'else if', pero actualmente OMNeT++ da error cuando esta dirección es mcast o bcast
    {
        EV << "    Frame is a special Multicast ARP-Path Frame (hello or repair)!" << endl;
        handlePathRepairFrame(frame,inputport);
    }
    else if(dstAddress.isBroadcast() || dstAddress.isMulticast())
    {
        //Multicast -> ARP-Path
        if (!repairingAddress.compareTo(dstAddress)) //TODO: Aquí debería ir el código de reparación, pero actualmente estas líneas no se ejecutan (por el problema de mcast de OMNeT++ y la solución con dirección ucast - mirar arriba)
        {
            EV << "    Frame is a special Multicast ARP-Path Frame (hello or repair)!" << endl;
            handlePathRepairFrame(frame,inputport);
        }
        //Any other multicast/broadcast frame
        else
        {
            EV << "    Frame is MULTICAST/BROADCAST" << endl;

            AddressEntryBasic entryBasic;
            entryExists = findEntryBasic(srcAddress, entryBasic);
            if (!entryExists) //Not found, add address to bcast table
            {
                EV << "      Entry not found -> Add new 'broadcast' entry..." << endl;
                addEntryBasic(entryBasic, srcAddress, inputport);

                EV << "        ...broadcast!" << endl;
                broadcastFrame(frame,inputport);
            }
            else //Address found
            {
                EV << "      Entry was found..." << endl;
                if (entryBasic.port == inputport) //If port is coincident -> refresh entry
                {
                    EV << "        ...refresh + broadcast!" << endl;
                    entryBasic.inTime = simTime();
                    addressTableBasic[srcAddress] = entryBasic;
                    broadcastFrame(frame,inputport);
                }
                else //Else -> discard
                {
                    EV << "        ...discard!" << endl;
                    delete (frame);
                }
            }
        }
    }
    //4º) If not ARP but UNICAST (Unicast)
    else
    {
        EV << "    Frame is UNICAST" << endl;

        //Forwarding
        AddressEntry destEntry;
        entryExists = findEntry(dstAddress, destEntry);
        if (entryExists)
        {
            EV << "      Output entry found at port: " << destEntry.port << endl;
            EV << "        ...refresh destination + forward" << endl;
            if(destEntry.status == learnt)
            {
                destEntry.inTime = simTime(); //Refresco sólo si la entrada ya está en estado 'learnt' (si no, estaríamos refrescando el estado 'locked' y eso no se hace con unicast)
                addressTable[dstAddress] = destEntry;
            }
            send(frame,"lowerLayerOut",destEntry.port);
        }
        else
        {
            EV << "      Output entry not found -> Starting repair..." << endl;
            startPathRepair(srcAddress,dstAddress);
            lostPackets++;
            delete(frame); //Original frame is discarded/lost
        }
    }
}

//Implements the logic of "ARP-Path unidireccional condicionado (v3)"
void MACRelayUnitAPB::handleAndDispatchFrameV3(EtherFrame* frame, int inputport)
{
    EV << "  Protocol version: 3 -> ARP-Path Unidirectional Conditional (v3)" << endl;

    MACAddress srcAddress = frame->getSrc();
    MACAddress dstAddress = frame->getDest();
    bool entryExists;
    EV << "  DEST ADDRESS: " << dstAddress << endl;
    EV << "  SRC ADDRESS: " << srcAddress << endl;
    EV << "  FRAME: " << frame << endl;
    EV << "  INPUTPORT:  " << inputport << endl;
    updateTables(); //Updates all entries from all the tables in the switch (aged entries, failed links, etc...)

    /*Once we know source and address, the frame is processed differently in 4 cases: 2 (bcast + ucast) for ARP
     * (in which learning is applied) + 2 (bcast/mcast + ucast) for the rest (in which learning is not applied)
     */
    //1º) If ARP Request (ARP + Broadcast)
    if(!strcmp("arpREQ",frame->getFullName()))
    {
        EV << "    Frame is ARP Request" << endl;
        ARPReqRcvd++;

        //Update + Print Route
        MACRelayUnitRoute * pRelayUnitRoute = check_and_cast<MACRelayUnitRoute *>(this->getParentModule()->getSubmodule("relayUnitRoute"));
        pRelayUnitRoute->updateAndSaveRoute(frame);

        //Learning + Blocking/Broadcasting
        AddressEntry entry;
        entryExists = findEntry(srcAddress, entry);
        if (entryExists)
        {
            if(entry.status == locked)
            {
                EV << "      Entry found -> 'locked' status..." << endl;
                if (inputport == entry.port)
                {
                    EV << "        ...refresh + broadcast!" << endl;
                    entry.inTime = simTime(); //Refresco
                    addressTable[srcAddress] = entry;
                    broadcastFrame(frame, inputport);
                }
                else
                {
                    EV << "        ...discard!" << endl;
                    delete (frame);

                    if(inputport == entry.portToSend) //#V3
                    {
                        EV << "        ...(original port was recovered)!" << endl;
                        entry.port = entry.portToSend; //Se recupera/sincroniza puerto original (aunque la trama se descarta)
                        addressTable[srcAddress] = entry;
                    }
                }
            }
            else
            {
                EV << "      Entry found -> 'learnt' status goes 'locked' ";
                if(inputport == entry.portToSend) //same old port
                    EV << "(inputport = original port)" << endl;
                else //new conditional/possible port (v3)
                    EV << "and waits for a frame at original port..." << endl;
                addEntry(entry, srcAddress, inputport);

                EV << "        ...broadcast!" << endl;
                broadcastFrame(frame,inputport);
            }
        }
        else
        {
            EV << "      Entry not found -> Add new 'locked' entry..." << endl;
            addEntry(entry, srcAddress, inputport);

            EV << "        ...broadcast!" << endl;
            broadcastFrame(frame,inputport);
        }

    }
    //2º) If ARP Reply (ARP + Unicast)
    else if(!strcmp("arpREPLY",frame->getFullName()))
    {
        EV << "    Frame is ARP Reply" << endl;
        ARPRepRcvd++;

        //Update + Print Route
        MACRelayUnitRoute * pRelayUnitRoute = check_and_cast<MACRelayUnitRoute *>(this->getParentModule()->getSubmodule("relayUnitRoute"));
        pRelayUnitRoute->updateAndSaveRoute(frame);

        //Learning
        AddressEntry entry;
        bool entryExists = findEntry(srcAddress, entry);
        if (entryExists)
        {
            if(entry.status == locked)
            {
                EV << "      Entry found -> 'locked' status..." << endl;
                if (inputport == entry.port)
                {
                    entry.inTime = simTime(); //Refresco
                    addressTable[srcAddress] = entry;
                }
                //else //No hacemos nada si no coincide (forward as if entry port was the same, but no refresh)
            }
            /*else //#V3 - Si no coincide, no se cambia nada en v3
            {
                EV << "      Entry found -> 'learnt' status goes 'locked' as a new entry..." << endl;
                addEntry(entry, srcAddress, inputport);
            }*/
        }
        else
        {
            EV << "      Entry not found -> Add new 'locked' entry..." << endl;
            addEntry(entry, srcAddress, inputport);
        }
        //TODO:Se podría pensar sobre si añadir las entradas como 'locked' (como ahora) o directamente 'learnt'

        //Forwarding
        AddressEntry destEntry;
        entryExists = findEntry(dstAddress, destEntry);
        if (entryExists)
        {
            EV << "      Output entry found at port: " << destEntry.portToSend << endl;
            EV << "        ...refresh destination + forward" << endl;
            if(destEntry.status == learnt)
            {
                destEntry.inTime = simTime(); //Refresco sólo si la entrada ya está en estado 'learnt' (si no, estaríamos refrescando el estado 'locked' y eso no se hace con unicast)
                addressTable[dstAddress] = destEntry;
            }
            send(frame,"lowerLayerOut",destEntry.portToSend); //#V3 (Ahora se manda por portToSend, que es el puerto fijo, condicionado en el aprendizaje)
        }
        else
        {
            EV << "      Output entry not found -> Starting repair..." << endl;
            startPathRepair(srcAddress,dstAddress);
            lostPackets++;
            delete(frame); //Original frame is discarded/lost
        }
    }
    //3º) If not ARP but BROADCAST/MULTICAST (Broadcast/Multicast)
    else if(!dstAddress.compareTo(repairingAddress)) //TODO: repairingAddress debería ser multicast e ir dentro del siguiente 'else if', pero actualmente OMNeT++ da error cuando esta dirección es mcast o bcast
    {
        EV << "    Frame is a special Multicast ARP-Path Frame (hello or repair)!" << endl;
        handlePathRepairFrame(frame,inputport);
    }
    else if(dstAddress.isBroadcast() || dstAddress.isMulticast())
    {
        //Multicast -> ARP-Path
        if (!repairingAddress.compareTo(dstAddress)) //TODO: Aquí debería ir el código de reparación, pero actualmente estas líneas no se ejecutan (por el problema de mcast de OMNeT++ y la solución con dirección ucast - mirar arriba)
        {
            EV << "    Frame is a special Multicast ARP-Path Frame (hello or repair)!" << endl;
            handlePathRepairFrame(frame,inputport);
        }
        //Any other multicast/broadcast frame

        //3aº) MULTICAST
        else if(dstAddress.isMulticast() && multicastActive)
        {
            EV << "    Frame is MULTICAST" << endl;

            //IGMP Report o IGMP Query
            int IGMPreport = strcmp("IGMPv2 report",frame->getFullName());
            int IGMPquery = strcmp("IGMPv2 query",frame->getFullName());
            if(!IGMPreport || !IGMPquery)
            {
                EV << "    IGMPreport = " << IGMPreport << "; IGMPquery = " << IGMPquery << endl;

                EV << "\n\t\t TIPO DE TRAMA:  " << frame->getName() << "\n" << endl;
                EV << "\t\t ORIGEN:........" << frame->getSrc() << endl;
                EV << "\t\t DESTINO:......." << frame->getDest() << endl;
                EV << "\t\t INPUTPORT:..." << inputport << "\n" << endl;

                //A) Locking
                EV << "    (A) Apply BT" << endl;
                bool learnMulticast = false;
                AddressEntryBasic entryBasic;
                entryExists = findEntryBasic(srcAddress, entryBasic);
                if (!entryExists) //No encontrada, añadida entrada a la tabla bcast
                {
                    EV << "      Entry not found -> Add new 'broadcast' entry..." << endl;
                    addEntryBasic(entryBasic, srcAddress, inputport);
                    learnMulticast = true;
                }
                else //Dirección encontrada
                {
                    EV << "        Entry was found..." << endl;
                    if (entryBasic.port == inputport) //If port is coincident -> refresh entry
                    {
                        EV << "        ...refresh + broadcast!" << endl;
                        entryBasic.inTime = simTime();
                        addressTableBasic[srcAddress] = entryBasic;
                        learnMulticast = true;
                    }
                    else //Else -> discard
                    {
                        EV << "        ...discard!" << endl;
                        delete (frame);
                    }
                }

                //B) Learning (if not locked before)
                if(learnMulticast)
                {
                    EV << "    (B) Update MT" << endl;
                    AddressEntryMulticast entryMulticast;
                    entryExists = findEntryMulticast(dstAddress, entryMulticast);
                    if (!entryExists) //Not found, add address to mcast table
                    {
                        EV << "      Entry not found -> Add new 'multicast' entry..." << endl;
                        addEntryMulticast(entryMulticast, dstAddress, inputport, true);
                        broadcastFrame(frame,inputport);
                    }
                    else //Address found
                    {
                        EV << "        Entry was found..." << endl;
                        EV << "        ...refresh ";
                        entryMulticast.inTime = simTime(); //Refresco
                        addressTableMulticast[dstAddress] = entryMulticast;

                        if(IGMPreport && !entryMulticast.port[inputport]) //IGMP Report - mcast only if necessary and not learnt
                        {
                            EV << "+ multicast! (IGMP Report)" << endl;
                            multicastFrame(frame,inputport, entryMulticast);
                        }
                        else //IGMP Query - bcast always
                        {
                            EV << "+ broadcast! (IGMP Query)" << endl;
                            broadcastFrame(frame,inputport);
                        }
                    }
                }
            }


            //Multicast data
            else//SI LA TRAMA ES MULTICAST-DATOS
            {
                EV << "\n\t\t TIPO DE TRAMA:  " << frame->getName() << "\n" << endl;
                EV << "\t\t ORIGEN:........" << frame->getSrc() << endl;
                EV << "\t\t DESTINO:......." << frame->getDest() << endl;
                EV << "\t\t INPUTPORT:..." << inputport << "\n" << endl;

                //Primero locking de BT (evitar bucles)
                AddressEntryBasic entryBasic;
                bool entryExists = findEntryBasic(srcAddress, entryBasic);
                if (!entryExists) //Not found, add address to bcast table
                {
                    EV << "      Entry not found -> Add new 'multicast' entry..." << endl;
                    addEntryBasic(entryBasic, srcAddress, inputport);

                    AddressEntryMulticast entry;
                    bool entryExists = findEntryMulticast(dstAddress, entry);
                    if (entryExists)//SI ENCONTRADO/*tx pertenece al grupo*/
                    {
                        EV << "\t\tEntrada DATOS en MulticastTable encontrada" << endl;

 //                       removeEntryMulticast(srcAddress,addressMulticast);
                        entry.inTime = simTime(); //Refresco
                        addressTableMulticast.insert(std::pair<MACAddress,AddressEntryMulticast>(srcAddress,entry));

                        EV << "\t\tDifusión DATOS: Multicast" << endl;
//                        multicastFrame(frame, inputport);
                    }
                    else
                        delete(frame);
                }
                else //Address found
                {
                    EV << "        Entry was found..." << endl;
                    if (entryBasic.port == inputport) //si los puertos coinciden, → refrescar entrada
                    {
                        EV << "        ...refresh + broadcast!" << endl;
                        entryBasic.inTime = simTime();
                        addressTableBasic[srcAddress] = entryBasic;

                        AddressEntryMulticast entry;
                        bool entryExists = findEntryMulticast(dstAddress, entry);
                        if (entryExists)//SI ENCONTRADO/*tx pertenece al grupo*/
                        {
                            EV << "\t\tEntrada DATOS en MulticastTable encontrada" << endl;

//                            removeEntryMulticast(srcAddress,addressMulticast);
                            entry.inTime = simTime(); //Refresco
                            addressTableMulticast.insert(std::pair<MACAddress,AddressEntryMulticast>(srcAddress,entry));

                            EV << "\t\tDifusión DATOS: Multicast" << endl;
//                            multicastFrame(frame, inputport);
                        }
                        else
                            delete(frame);
                    }
                    else //Else -> discard
                    {
                        EV << "        ...discard!" << endl;
                        delete (frame);
                    }
                }
            }
        }



        //3bº) BROADCAST (or MULTICAST with multicastActive == false)
        else
        {
            if(multicastActive)
                EV << "    Frame is BROADCAST" << endl;
            else
                EV << "    Frame is MULTICAST/BROADCAST" << endl;

            AddressEntryBasic entryBasic;
            bool entryExists = findEntryBasic(srcAddress, entryBasic);
            if (!entryExists) //Not found, add address to bcast table
            {
                EV << "      Entry not found -> Add new 'broadcast' entry..." << endl;
                addEntryBasic(entryBasic, srcAddress, inputport);

                EV << "        ...broadcast!" << endl;
                broadcastFrame(frame,inputport);
            }
            else //Address found
            {
                EV << "      Entry was found..." << endl;
                if (entryBasic.port == inputport) //If port is coincident -> refresh entry
                {
                    EV << "        ...refresh + broadcast!" << endl;
                    entryBasic.inTime = simTime();
                    addressTableBasic[srcAddress] = entryBasic;
                    broadcastFrame(frame,inputport);
                }
                else //Else -> discard
                {
                    EV << "        ...discard!" << endl;
                    delete (frame);
                }
            }
        }
    }
    //4º) If not ARP but UNICAST (Unicast)
    else
    {
        EV << "    Frame is UNICAST" << endl;

        //Forwarding
        AddressEntry destEntry;
        bool entryExists = findEntry(dstAddress, destEntry);
        if (entryExists)
        {
            EV << "      Output entry found at port: " << destEntry.portToSend << endl;
            EV << "        ...refresh destination + forward" << endl;
            if(destEntry.status == learnt)
            {
                destEntry.inTime = simTime(); //Refresco sólo si la entrada ya está en estado 'learnt' (si no, estaríamos refrescando el estado 'locked' y eso no se hace con unicast)
                addressTable[dstAddress] = destEntry;
            }
            send(frame,"lowerLayerOut",destEntry.portToSend); //#V3 (Ahora se manda por portToSend, que es el puerto fijo, condicionado en el aprendizaje)
        }
        else
        {
            EV << "      Output entry not found -> Starting repair..." << endl;
            startPathRepair(srcAddress,dstAddress);
            lostPackets++;
            delete(frame); //Original frame is discarded/lost
        }
    }
}

//Assign automatic/manual MAC address to the switch
void MACRelayUnitAPB::generateSwitchAddress()
{
    myAddress.setAddress(par("address"));

    for(unsigned int i=0; i<MAC_ADDRESS_SIZE; i++)
        if(myAddress.getAddressByte(i)!=0)
            return; //If some byte!=0, 'myAddress' has a manual and valid value, and we don't need to assign it some

    //If all bytes==0, 'myAddress' needs to be assigned an automatic MAC address value
    int switchId = this->getId();
    myAddress.setAddressByte(0,0xBB); //B de 'Bridge' y así sabemos que es una MAC de puente :D
    myAddress.setAddressByte(1,0xBB);
    myAddress.setAddressByte(2,switchId/0xFFFFFF);
    switchId = switchId-switchId/0xFFFFFF;
    myAddress.setAddressByte(3,switchId/0xFFFF);
    switchId = switchId-switchId/0xFFFF;
    myAddress.setAddressByte(4,switchId/0xFF);
    switchId = switchId-switchId/0xFF;
    myAddress.setAddressByte(5,switchId);
}

//Create tables with the given parameters
void MACRelayUnitAPB::generateParametersTablesAndEvents(void)
{
    std::string mac = "";
    std::string sport = "";
    size_t pos;

    EV << "->MACRelayUnitAPB::generateParametersTables()" << endl;

    //Create links down table
    mac = portLinkDown;
    while (mac != "")
    {
        //Port
        pos = mac.find(":"); //if mac != "", the character ":" must always be found
        sport = mac.substr(0, pos); //until ':'
        int port = atoi(sport.c_str());
        mac = mac.substr(pos+1);
        //Init time
        pos = mac.find("-");
        sport = mac.substr(0, pos); //until '-'
        int init = atoi(sport.c_str());
        mac = mac.substr(pos+1);
        //End time
        pos = mac.find(";");
        sport = mac.substr(0, pos); //until ';' or end
        int end = atoi(sport.c_str());

        if(repairType == 3) //To avoid the generation of many events, we only create an event for repairType==3, i.e. when repair is proactive (and not reactive like in 1 and 2)
        {
            //Generate a 'LinkDown' event - Some link will be down at that time (once we receive the event, we check the table)
            cMessage *timer = new cMessage("LinkDown!");
            scheduleAt(init, timer);
            EV << "  LinkDown! event scheduled at time " << init << "(s)" << endl;
        }

        //Add/Update entry
        if (linkDownTable.find(port) == linkDownTable.end())
        {
            InitEndTimeEntry entry;
            entry.nEvents = 1;
            entry.initTime[0] = init;
            entry.endTime[0] = end;
            linkDownTable[port] = entry;
        }
        else
        {
            InitEndTimeEntry* entry = &(linkDownTable[port]);
            if(entry->nEvents < MAX_EVENTS) //Up to 10 events (only)
            {
                entry->initTime[entry->nEvents] = init;
                entry->endTime[entry->nEvents] = end;
                entry->nEvents++;
            }
        }

        //Next entry
        if (pos == std::string::npos) //string end was reached
        {
            mac = ""; //break
            break;
        }
        else //next port location
            mac = mac.substr(pos+1);
    }

    //Create switch down structure
    switchDownStruct.nEvents = 0;
    mac = switchDown;
    while (mac != "")
    {
        //Init time
        pos = mac.find("-");
        sport = mac.substr(0, pos); //until '-'
        int init = atoi(sport.c_str());
        mac = mac.substr(pos+1);
        //End time
        pos = mac.find(";");
        sport = mac.substr(0, pos); //until ';' or end
        int end = atoi(sport.c_str());

        //Generate a 'SwitchDown' event - Current switch will be down at that time (once we receive the event, we can check the struct)
        cMessage *timer1 = new cMessage("SwitchDown!");
        scheduleAt(init, timer1);
        EV << "  SwitchDown! event scheduled at time " << init << "(s)" << endl;

        //Generate a 'SwitchUp' event - Current switch will be up at that time (once we receive the event, we can check the struct)
        cMessage *timer2 = new cMessage("SwitchUp!");
        scheduleAt(end, timer2);
        EV << "  SwitchUp! event scheduled at time " << end << "(s)" << endl;

        //Update entry
        if(switchDownStruct.nEvents < MAX_EVENTS) //Up to 10 events (only)
        {
            switchDownStruct.initTime[switchDownStruct.nEvents] = init;
            switchDownStruct.endTime[switchDownStruct.nEvents] = end;
            switchDownStruct.nEvents++;
        }

        //Next entry
        if (pos == std::string::npos) //string end was reached
        {
            mac = ""; //break
            break;
        }
        else //next port location
            mac = mac.substr(pos+1);
    }

    EV << "<-(generateParametersTables):" << endl;
}

//Method that updates all the tables in the switch before handling any frame
void MACRelayUnitAPB::updateTables()
{
    EV << "->MACRelayUnitAPB::updateTables()" << endl;
    EV << "    Updating entries from LT (aged 'locked', 'learnt' and ports associated with down links entries):" << endl;
    updateAndRemoveAgedEntriesFromLT();
    EV << "    Updating entries from BT (aged 'locked' and ports associated with down links entries):" << endl;
    updateAndRemoveAgedEntriesFromBT();
    EV << "    Updating entries from HeT (ports associated with down links entries):" << endl;
    updateAndRemoveAgedEntriesFromHeT();
    EV << "    Updating entries from RT (aged entries):" << endl;
    updateAndRemoveAgedEntriesFromRT();
    EV << "<-MACRelayUnitAPB::updateTables()" << endl;
}

//Method to remove obsolete entries from LT (Lookup/Learning Table)
//and it will also change entry status from 'locked' to 'learnt' when the blocking timer expires
void MACRelayUnitAPB::updateAndRemoveAgedEntriesFromLT()
{
    EV << "    LT (" << this->getParentModule()->getFullName() << ") (" << addressTable.size() << " entries):\n";
    EV << "      ----------------------------------------------------------------------------------" << endl;
    EV << "      | Address                     | Port        | Timer | Status  | IP                  |" << endl;
    EV << "      ----------------------------------------------------------------------------------" << endl;

    for (AddressTable::iterator iter = addressTable.begin(); iter != addressTable.end();)
    {
        AddressTable::iterator cur = iter++; // iter will get invalidated after erase()
        AddressEntry& entry = cur->second;

        EV << "      | " << cur->first << " |    " << entry.port << " [ " << entry.portToSend << " ] | ";
                float time = simTime().dbl()-entry.inTime.dbl();
                char buffer[20];
                sprintf(buffer, "%.03f | ", time);
                EV << buffer; //Print timer just with 3 decimals
                if(entry.status == locked) EV << "locked | ";
                else EV << "learnt | ";
                EV << entry.ip << "    |";

        if(linkDownForPort(entry.port))
        {
            EV << " -> Removing entry from LT (link is down!)";
            addressTable.erase(cur); //erase does not return the following iterator
        }
        else if (entry.inTime + agingTime <= simTime()) //First we remove any aged entry
        {
            EV << " -> Removing aged learnt entry from LT with time: " << simTime()-entry.inTime << " (over " << agingTime << ")";
            addressTable.erase(cur); //erase does not return the following iterator
        }
        else if (entry.status == locked && entry.inTime + blockingTime <= simTime()) //Later, we check if some 'locked' entry (still not completely aged) needs to update to 'learnt' status
        {
            EV << " -> Updating locked entry to learnt status with time: " << simTime()-entry.inTime << " (over " << blockingTime << ")";
            addressTable[cur->first].status = learnt;
            if(protocolVersion == 3)
            {
                EV << " -> Updating portToSend! (current: " << entry.port << " previous: " << entry.portToSend << ")";
                addressTable[cur->first].portToSend = addressTable[cur->first].port;
            }
        }
        EV << endl;
    }
}

//Method to remove obsolete entries from BT (Blocking/Broadcast Table)
void MACRelayUnitAPB::updateAndRemoveAgedEntriesFromBT()
{
    EV << "    BT (" << this->getParentModule()->getFullName() << ") (" << addressTableBasic.size() << " entries):\n";
    EV << "      ------------------------------------------------" << endl;
    EV << "      | Address                     | Port | Timer |" << endl;
    EV << "      ------------------------------------------------" << endl;

    for (AddressTableBasic::iterator iter = addressTableBasic.begin(); iter != addressTableBasic.end();)
    {
        AddressTableBasic::iterator cur = iter++; // iter will get invalidated after erase()
        AddressEntryBasic& entry = cur->second;

        EV << "      | " << cur->first << " |    " << entry.port << "    | ";
                float time = simTime().dbl()-entry.inTime.dbl();
                char buffer[20];
                sprintf(buffer, "%.03f |", time);
                EV << buffer; //Print timer just with 3 decimals

        if(linkDownForPort(entry.port))
        {
            EV << " -> Removing entry from BT (link is down!)";
            addressTableBasic.erase(cur); //erase does not return the following iterator
        }
        else if (entry.inTime + blockingTime <= simTime())
        {
            EV << "  -> Removing aged entry from BT with time: " << simTime()-entry.inTime << " (over " << blockingTime << ")";
            addressTableBasic.erase(cur);
        }
        EV << endl;
    }
}

//Method to remove obsolete entries from HeT (Hello Table)
void MACRelayUnitAPB::updateAndRemoveAgedEntriesFromHeT()
{
    EV << "    HeT (" << this->getParentModule()->getFullName() << ") (" << notHostList.size() << " entries):\n";
    EV << "      ---------" << endl;
    EV << "      | Port |" << endl;
    EV << "      ---------" << endl;

    for(unsigned int i=0; i<notHostList.size(); i++)
    {
        EV << "      |     " << notHostList[i] << "   |";
        if (linkDownForPort(notHostList[i]))
        {
            EV << " -> Removing entry from HeT (link is down!)";
            notHostList.erase(notHostList.begin()+i); //erase does not return the following iterator
            i--; //Return to previous element (i++ will be executed after this line)
        }
        //No 'if-timer' needed (entries only expire with down links, not with timers)
        EV << endl;
    }
}

//Method to remove obsolete entries from RT (Repair Table)
void MACRelayUnitAPB::updateAndRemoveAgedEntriesFromRT()
{
    EV << "    RT (" << this->getParentModule()->getFullName() << ") (" << repairTable.size() << " entries):\n";
    EV << "      -------------------------------------------" << endl;
    EV << "      | Address                       | Timer |" << endl;
    EV << "      -------------------------------------------" << endl;

    for (RepairTable::iterator iter = repairTable.begin(); iter != repairTable.end();)
    {
        RepairTable::iterator cur = iter++; // iter will get invalidated after erase()
        RepairEntry& entry = cur->second;

        EV << "      | " << cur->first << " |    ";
                float time = simTime().dbl()-entry.inTime.dbl();
                char buffer[20];
                sprintf(buffer, "%.03f |", time);
                EV << buffer; //Print timer just with 3 decimals

        if (entry.inTime + repairingTime <= simTime())
        {
            EV << " -> Removing aged entry from RT with time: " << simTime()-entry.inTime << " (over " << repairingTime << ")";
            repairTable.erase(cur);
        }
        EV << endl;
    }
}

//Method to reset all tables
void MACRelayUnitAPB::removeAllEntriesFromTables()
{
    EV << "->MACRelayUnitAPB::removeAllEntriesFromTables()" << endl;

    addressTable.clear();       //LT
    addressTableBasic.clear();  //BT
    notHostList.clear();        //HeT
    repairTable.clear();        //RT
}

//Method to add an entry in the address table, given its MAC address and inputport
void MACRelayUnitAPB::addEntry(AddressEntry entry, MACAddress address, int inputport)
{
    entry.inTime = simTime();
    entry.port = inputport;
    entry.status = locked;
    if(protocolVersion == 3 && entry.portToSend == -1) //If v3 and portOrig still not set, set it to the same value than port
        entry.portToSend = entry.port;
    addressTable[address] = entry;
}

//Method to add an entry in the special broadcast address table, given its MAC address and inputport
void MACRelayUnitAPB::addEntryBasic(AddressEntryBasic entry, MACAddress address, int inputport)
{
    entry.inTime = simTime();
    entry.port = inputport;
    addressTableBasic[address] = entry;
}

void MACRelayUnitAPB::addEntryMulticast(AddressEntryMulticast entry, MACAddress address, int inputport, bool isNew)
{
    entry.inTime = simTime();
    if(isNew) //If new entry, first set all port to false (just in case they have any other random value...)
        for(unsigned int i=0; i<NPORTS; i++)
            entry.port[i] = false;
    entry.port[inputport] = true;
    addressTableMulticast[address] = entry;
}

//Method to find an entry in the address table, given its MAC address.
bool MACRelayUnitAPB::findEntry(MACAddress address, AddressEntry& entry)
{
    bool found = false;
    AddressTable::iterator iter = addressTable.find(address);

    if (iter != addressTable.end())
    {
        entry.inTime = iter->second.inTime;
        entry.ip = iter->second.ip;
        entry.port = iter->second.port;
        entry.portToSend = iter->second.portToSend;
        entry.status = iter->second.status;
        found = true;
    }
    else
    {
        entry.port = -1;
        entry.portToSend = -1;
    }
    return found;
}

//Method to find an entry in the special broadcast address table, given its MAC address.
bool MACRelayUnitAPB::findEntryBasic(MACAddress address, AddressEntryBasic& entryBasic)
{
    bool found = false;
    AddressTableBasic::iterator iter = addressTableBasic.find(address);

    if (iter != addressTableBasic.end())
    {
        entryBasic.inTime = iter->second.inTime;
        entryBasic.port = iter->second.port;
        found = true;
    }
    else
    {
        entryBasic.port = -1;
    }
    return found;
}

//Method to find an entry in the special multicast address table, given its mcast/group MAC address.
bool MACRelayUnitAPB::findEntryMulticast(MACAddress address, AddressEntryMulticast& entryMulticast)
{
    bool found = false;
    AddressTableMulticast::iterator iter = addressTableMulticast.find(address);

    if (iter != addressTableMulticast.end())
    {
        entryMulticast.inTime = iter->second.inTime;
        //entryMulticast.port = iter->second.port;
        found = true;
    }
    return found;
}

//Method to check if a link associated with a port is down
bool MACRelayUnitAPB::linkDownForPort(int port)
{
    //If there's a 'link down' entry for that port, look for times
    if (linkDownTable.find(port) != linkDownTable.end())
        for(unsigned int i=0; i<linkDownTable[port].nEvents; i++)
            if (simTime()>=linkDownTable[port].initTime[i] && simTime() <linkDownTable[port].endTime[i])
                return true;

    return false;
}

//Method to create and send hello frames to other switches (currently only at the beginning).
void MACRelayUnitAPB::sendHello()
{
    EV << "->MACRelayUnitAPB::sendHello()" << endl;
    broadcastFrame(repairFrame(Hello),-1,true); //Al poner 'true' los switches van aprendiendo qué hosts tienen conectados directamente e irán dejando de mandarles mensajes Hello a ellos (sin 'true' mandarían siempre por todos los puertos al margen de dicho aprendizaje)
}

//Method that checks if the port belongs to a link connected to another switch or not (it's connected to a host or any other device, e.g. a non-ARP-Path switch)
bool MACRelayUnitAPB::inHeT(int port)
{
    unsigned int n=notHostList.size();

    for(unsigned int i=0; i<n; i++)
        if(notHostList[i] == port)
            return true;

    return false;
}

//Method that checks if the port belongs to a non-ARP-Path device (host, etc...). Returns false when inHeT is true and viceversa, but returns false also when no Hello was recorded and inHeT returns false as well.
bool MACRelayUnitAPB::inHoT(int port)
{
    if(inHeT(port))
        return false;

    for (AddressTable::iterator iter = addressTable.begin(); iter!=addressTable.end(); iter++)
        if(iter->second.port == port)
            return true;

    return false; //Return false when inHeT return 'false' and port is not in LT - Possible because no Hello/message was recorded (just after initialization or just after a switch failure)
}

//Method to check if a path for a given address is being repaired.
bool MACRelayUnitAPB::inRepairTable(MACAddress address)
{
    RepairTable::iterator iter = repairTable.find(address);

    if (iter != repairTable.end())
        return true;

    return false;
}

//If the host provided in 'address' is directly connected to the switch, it returns the port associated, if not it returns -1
int MACRelayUnitAPB::portOfHost(MACAddress address)
{
    AddressTable::iterator iter = addressTable.find(address);

    if (iter != addressTable.end() && !inHeT(iter->second.port))
        return iter->second.port; //If address found at LT and its port is not connected to another switch, then that address is directly connected to the switch through that port

    return -1; //Not directly connected through any port
}

//Method that creates special single-address repair frames (Hello, PathFail, PathRequest, PathReply)
EtherFrame* MACRelayUnitAPB::repairFrame(RepairMessageType type, MACAddress srcAddress, MACAddress dstAddress, simtime_t repairTime)
{
    const char* name;

    if(type == Hello)
    {
        EV << "  Hello created!" << endl;
        name = "Hello";
    }
    else if(type == PathFail)
    {
        EV << "  PathFail created!" << endl;
        name = "PathFail";
    }
    else if(type == PathRequest)
    {
        EV << "  PathRequest created!" << endl;
        name = "PathRequest";
    }
    else //if(type == PathReply)
    {
        EV << "  PathReply created!" << endl;
        name = "PathReply";
    }

    PathRepair * prf = new PathRepair(name);
    prf->setDest(repairingAddress);     //ARP-Path multicast address
    prf->setSrc(myAddress);
    //prf->setDsap(43);
    //prf->setSsap(43);
    prf->setType(type);                 //PathRepair message type
    prf->setSrcMACAddress(srcAddress);  //Address to be learnt/repaired
    prf->setDestMACAddress(dstAddress); //2nd address to be learnt/repaired (if applicable)
    prf->setRepairTime(repairTime);     //Current repair time (=0 if Hello or PathFail), which includes all latencies previously needed by other PathRepair messages
    prf->setByteLength(MIN_ETHERNET_FRAME_BYTES);             //Set to the minimum frame (64 bytes)

    //Las siguientes líneas son del ARP, que se emite en broadcast sin error (ya se han probado y sigue sin funcionar... pero las dejamos por si acaso)
    // add control info with MAC address
    //Ieee802Ctrl *controlInfo = new Ieee802Ctrl();
    //controlInfo->setDest(macAddress);
    //controlInfo->setEtherType(etherType);
    //prf->setControlInfo(controlInfo);
    //static MACAddress broadcastAddress("ff:ff:ff:ff:ff:ff");

    return check_and_cast<EtherFrame*>(prf);
}

//Method that creates special group-address repair frames (LinkFail, LinkReply)
EtherFrame* MACRelayUnitAPB::groupRepairFrame(RepairMessageType type, MACAddress repairSwitch, MACAddressVector addressVector, simtime_t repairTime)
{
    const char* name;

    if(type == LinkFail)
    {
        EV << "  LinkFail created!" << endl;
        name = "LinkFail";
    }
    else //if(type == LinkReply)
    {
        EV << "  LinkReply created!" << endl;
        name = "LinkReply";
    }

    PathRepair * prf = new PathRepair(name);
    prf->setDest(repairingAddress);             //ARP-Path multicast address
    prf->setSrc(myAddress);
    prf->setType(type);                         //PathRepair message type
    prf->setRepairSwitch(repairSwitch);         //Switch that originated the repair
    prf->setRepairMACAddresses(addressVector);  //Group of addresses to be repaired
    prf->setRepairTime(repairTime);             //Current repair time (=0 if Hello or PathFail), which includes all latencies previously needed by other PathRepair messages
    prf->setByteLength(MIN_ETHERNET_FRAME_BYTES);             //Set to the minimum frame (64 bytes)

    return check_and_cast<EtherFrame*>(prf);
}

void MACRelayUnitAPB::startPathRepair(MACAddress srcAddress, MACAddress dstAddress)
{
    //Reparación proactiva (3)
    if(repairType == 3) //No addresses given by the parameters and no need to check inRepairTable because the repair is just called once (when the link goes DOWN)
    {
        if(srcAddress.isUnspecified() || dstAddress.isUnspecified())
        {
            EV << "Starting repair... (type=3)" << endl;
            nRepairStarted++;

            //First, check the link (associated port) which is now DOWN and addresses affected by that link
            MACAddressVector addressVector;
            for (AddressTable::iterator iter = addressTable.begin(); iter != addressTable.end(); iter++)
            {
                EV << "  Address: " << iter->first;
                if(linkDownForPort(iter->second.port))
                {
                    EV << " -> Needs to be repaired (link is down!) -> Added to LinkFail";
                    addressVector.push_back(iter->first);
                }
                EV << endl;
            }

            //Second, create LinkFail message if some address is affected by the link status change
            if(!addressVector.empty())
                broadcastFrame(groupRepairFrame(LinkFail,myAddress,addressVector),-1,true);
            //TODO: Ahora mismo sólo se inicia una vez la reparación (cuando el enlace cae), podría repetirse cada X tiempo, por si acaso no funcionara a la primera (pues descartaría todo el tráfico que sigue si no se repara a la primera)
        }
        else //A frame is received while the path is still being repaired, discard!
        {
            EV << "Already repairing this path, deleting frame (waiting for repair to finish)" << endl;
        }
    }
    //Reparación reactiva (1 y 2)
    else if (!inRepairTable(dstAddress))
    {
        EV << "Starting repair... (type=1|2)" << endl;
        nRepairStarted++;

        RepairEntry rEntry;
        rEntry.inTime = simTime();
        repairTable[dstAddress] = rEntry; //Add new repair entry

        if(repairType == 1) //PathFail+PathRequest+PathReply
        {
            int port = portOfHost(srcAddress);
            if (port !=-1) //If src directly connected, omit PathFail and send PathRequest
                broadcastFrame(repairFrame(PathRequest,srcAddress,dstAddress),port,true);
            else //Send PathFail otherwise
                broadcastFrame(repairFrame(PathFail,srcAddress,dstAddress),-1,true);
        }
        else //if(repairType == 2) //PathFail+PathRequest
        {
            int port = portOfHost(dstAddress);
            if (port !=-1) //If dst directly connected (this will never happen since we have checked and cause the repair because the entry does not exists in LT), omit PathFail and send PathRequest
                broadcastFrame(repairFrame(PathRequest,dstAddress,srcAddress),port,true);
            else //Send PathFail otherwise (always)
                broadcastFrame(repairFrame(PathFail,dstAddress,srcAddress),-1,true);
        }
        //TODO: Falta implementar el tipo 3, que es proactivo y no reactivo

    }
    else
    {
        EV << "Already repairing this path, deleting frame (waiting for flag)" << endl;
    }
}

//Method to handle reparation special frames (PathFail, PathReq, PathReply)
void MACRelayUnitAPB::handlePathRepairFrame(EtherFrame *frame, int inputport)
{
    EV << "->MACRelayUnitAPB::handlePathRepairFrame()" << endl;
    if(protocolVersion == 2)
        handlePathRepairFrameV2(frame, inputport);
    else if(protocolVersion == 3)
        handlePathRepairFrameV3(frame, inputport);
    else
        EV << "  Protocol version " << protocolVersion << " is not yet implemented! ERROR!" << endl;
    EV << "<-MACRelayUnitAPB::handlePathRepairFrame()" << endl;
}

//Implements the logic of "ARP-Path unidireccional condicionado (v3)"
void MACRelayUnitAPB::handlePathRepairFrameV2(EtherFrame* frame, int inputport)
{
    PathRepair* prf = check_and_cast<PathRepair*>(frame);
    MACAddress switchAddress = prf->getSrc();    //MAC address/ID of the message's source switch
    unsigned int type = prf->getType();
    simtime_t repairTime = simTime()-frame->getCreationTime(); //Get current repair time (i.e. simTime - creation time of the frame)

    if (!myAddress.compareTo(switchAddress))
    {
        ev << "  This switch is src of the frame -> discarded!" << endl;
        delete (frame);
    }
    else
    {
        EV << "  Path repair type: " << type << endl;
        if (type == Hello) //Hello
        {
            HelloRcvd++;
            EV << "  'Hello' received from port: " << inputport << ". Not host connected!" << endl;

            //Include port in the list if not already in it
            if(!inHeT(inputport))
                notHostList.insert(notHostList.end(),inputport);

            delete(frame);
        }
        else //Repair message
        {
            AddressEntry entry;
            AddressEntryBasic entryBasic;
            bool entryExists;

            MACAddress srcAddress = prf->getSrcMACAddress();
            MACAddress dstAddress = prf->getDestMACAddress();

            bool discardFrame = false;

            if (type == PathFail) //PathFail
            {
                PathFailRcvd++;
                EV << "  'PathFail' received from port: " << inputport << endl;

                //1st) Apply 'locking' filter to avoid loops (BT)
                entryExists = findEntryBasic(switchAddress, entryBasic);
                if (!entryExists) //Not found, add address to bcast table
                {
                    EV << "      Entry not found -> Add new 'broadcast' entry..." << endl;
                    addEntryBasic(entryBasic, switchAddress, inputport);
                }
                else //Address found
                {
                    EV << "      Entry was found..." << endl;
                    if (entryBasic.port == inputport) //If port is coincident -> refresh entry
                    {
                        EV << "        ...refresh!" << endl;
                        entryBasic.inTime = simTime();
                        addressTableBasic[switchAddress] = entryBasic;
                    }
                    else //Else -> discard
                    {
                        EV << "        ...discard!" << endl;
                        discardFrame = true;
                    }
                }

                //2nd) If src directly connected send PathRequest, otherwise broadcast! (late copies are previously discarded)
                if(!discardFrame)
                {
                    int port = portOfHost(srcAddress);
                    if (port !=-1 /*&& entry.port != -1*/)  //If src directly connected, send PathRequest
                    {
                        EV << "    Switch is directly connected to src host!" << endl; //TODO: Aquí podría añadirse otra RT (frontera)
                        entryExists = findEntry(srcAddress, entry);
                        entry.inTime = simTime();
                        entry.status = locked;     //Update entry to 'locked' (the learning process starts again)
                        addressTable[srcAddress] = entry;
                        broadcastFrame(repairFrame(PathRequest, srcAddress, dstAddress, repairTime),port,true); //Include repairTime for the new PathRepair message
                        discardFrame = true;

                        updateRepairStatistics(prf, type); //Update repair statistics (PathFail time) with PathRepair frame
                    }
                    else //Otherwise, continue broadcasting PathFail
                    {
                        EV << "    Src host not found (not directly connected)! ...broadcast!" << endl;
                        broadcastFrame(frame,inputport,true);
                    }
                }
            }
            else if (type == PathRequest) //PathRequest
            {
                PathRequestRcvd++;
                EV << "  'PathRequest' received from port: " << inputport << endl;

                //1st) Apply 'locking' filter to avoid loops and 'learn' info about srcAddress (LT)
                bool entryExists = findEntry(srcAddress, entry);
                if (entryExists)
                {
                    if(entry.status == locked)
                    {
                        EV << "      Entry found -> 'locked' status..." << endl;
                        if (inputport == entry.port)
                        {
                            EV << "        ...refresh!" << endl;
                            entry.inTime = simTime(); //Refresco
                            addressTable[srcAddress] = entry;
                        }
                        else
                        {
                            EV << "        ...discard!" << endl;
                            discardFrame = true;
                        }
                    }
                    else
                    {
                        EV << "      Entry found -> 'learnt' status goes 'locked' as a new entry..." << endl;
                        addEntry(entry, srcAddress, inputport);
                    }
                }
                else
                {
                    EV << "      Entry not found -> Add new 'locked' entry..." << endl;
                    addEntry(entry, srcAddress, inputport);
                }

                //2nd) If dst directly connected send PathReply (if applicable), otherwise broadcast! (late copies are previously discarded)
                if(!discardFrame)
                {
                    int port = portOfHost(dstAddress);
                    if (port !=-1 /*&& entry.port != -1*/) //If dst directly connected, send PathReply only if applicable (i.e. version 1)
                    {
                         EV << "    Switch is directly connected to dst host!" << endl; //TODO: Aquí podría añadirse otra RT (frontera) - sólo para PathReply (versión 1)
                         if(repairType == 1)
                         {
                             entryExists = findEntry(dstAddress, entry);
                             entry.inTime = simTime();
                             entry.status = locked;     //Update entry to 'locked' (the learning process starts again)
                             addressTable[dstAddress] = entry;
                             send((EtherFrame*)repairFrame(PathReply, dstAddress, srcAddress, repairTime), "lowerLayerOut", inputport); //Reverse srcAddress/dstAddress to create the PathReply and sent through inputport (the port that received the PathRequest) //Include repairTime for the new PathRepair message
                             EV << "Forwarding through port: " << inputport << endl;
                         }
                         discardFrame = true;

                         updateRepairStatistics(prf, type); //Update repair statistics (PathRequest time and Repair time if repairType == 2) with PathRepair frame
                     }
                     else //Otherwise, continue broadcasting PathRequest
                     {
                         EV << "    Dst host not found (not directly connected)! ...broadcast!" << endl;
                         broadcastFrame(frame,inputport,true);
                     }
                }
            }
            else if (type == PathReply) //PathReply
            {
                PathReplyRcvd++;
                EV << "  'PathReply' received from port: " << inputport << endl;

                //1st) Learning
                entryExists = findEntry(srcAddress, entry);
                if (entryExists)
                {
                    if(entry.status == locked)
                    {
                        EV << "      Entry found -> 'locked' status..." << endl;
                        if (inputport == entry.port)
                        {
                            entry.inTime = simTime(); //Refresco
                            addressTable[srcAddress] = entry;
                        }
                        //else //No hacemos nada si no coincide (forward as if entry port was the same, but no refresh)
                    }
                    else
                    {
                        EV << "      Entry found -> 'learnt' status goes 'locked' as a new entry..." << endl;
                        addEntry(entry, srcAddress, inputport);
                    }
                }
                else
                {
                    EV << "      Entry not found -> Add new 'locked' entry..." << endl;
                    addEntry(entry, srcAddress, inputport);
                }
                //TODO:Se podría pensar sobre si añadir las entradas como 'locked' (como ahora) o directamente 'learnt'

                //2nd) Forwarding
                AddressEntry destEntry;
                entryExists = findEntry(dstAddress, destEntry);
                if (entryExists)
                {
                    EV << "      Output entry found at port: " << destEntry.port << endl;
                    EV << "        ...refresh destination + forward" << endl;
                    if(destEntry.status == learnt)
                    {
                        destEntry.inTime = simTime(); //Refresco sólo si la entrada ya está en estado 'learnt' (si no, estaríamos refrescando el estado 'locked' y eso no se hace con unicast)
                        addressTable[dstAddress] = destEntry;
                    }
                    if(!inHoT(destEntry.port)) //Forward only if destination host is not directly connected (i.e. that port is not in HoT's list)
                    {
                        send(frame,"lowerLayerOut",destEntry.port);
                        EV << "Forwarding through port: " << destEntry.port << endl;
                    }
                    else
                    {
                        EV << "    Switch is directly connected to dst host! Stop repair v1 (finished!)" << endl;
                        discardFrame = true;
                        updateRepairStatistics(prf, type); //Update repair statistics (PathReply time and Repair time if repairType == 1) with PathRepair frame
                    }
                }
                else
                {
                    EV << "      Output entry not found -> Starting repair..." << endl;
                    startPathRepair(srcAddress,dstAddress);
                    discardFrame = true; //Original frame is discarded/lost
                }
            }
            //La reparación tipo 3 no está implementada para la v2.0 del protocolo pues se supone muy posterior y el código v2.0 sólo se mantiene por si se quiere consultar/probar algo puntualmente...

            //Finally, if frame was discarded, delete it
            if(discardFrame)
                delete (frame);
        }
    }
}

//Implements the logic of "ARP-Path unidireccional condicionado (v3)"
void MACRelayUnitAPB::handlePathRepairFrameV3(EtherFrame* frame, int inputport)
{
    PathRepair* prf = check_and_cast<PathRepair*>(frame);
    MACAddress switchAddress = prf->getSrc();    //MAC address/ID of the message's source switch
    unsigned int type = prf->getType();
    simtime_t repairTime = simTime()-frame->getCreationTime(); //Get current repair time (i.e. simTime - creation time of the frame)

    if (!myAddress.compareTo(switchAddress))
    {
        ev << "  This switch is src of the frame -> discarded!" << endl;
        delete (frame);
    }
    else
    {
        EV << "  Path repair type: " << type << endl;
        if (type == Hello) //Hello
        {
            HelloRcvd++;
            EV << "  'Hello' received from port: " << inputport << ". Not host connected!" << endl;

            //Include port in the list if not already in it
            if(!inHeT(inputport))
                notHostList.insert(notHostList.end(),inputport);

            delete(frame);
        }
        else //Repair message
        {
            AddressEntry entry;
            AddressEntryBasic entryBasic;
            bool entryExists;

            MACAddress srcAddress = prf->getSrcMACAddress();
            MACAddress dstAddress = prf->getDestMACAddress();
            MACAddress repairSwitch = prf->getRepairSwitch();
            MACAddressVector addressVector = prf->getRepairMACAddresses();

            bool discardFrame = false;

            if (type == PathFail) //PathFail
            {
                PathFailRcvd++;
                EV << "  'PathFail' received from port: " << inputport << endl;

                //1st) Apply 'locking' filter to avoid loops (BT)
                entryExists = findEntryBasic(switchAddress, entryBasic);
                if (!entryExists) //Not found, add address to bcast table
                {
                    EV << "      Entry not found -> Add new 'broadcast' entry..." << endl;
                    addEntryBasic(entryBasic, switchAddress, inputport);
                }
                else //Address found
                {
                    EV << "      Entry was found..." << endl;
                    if (entryBasic.port == inputport) //If port is coincident -> refresh entry
                    {
                        EV << "        ...refresh!" << endl;
                        entryBasic.inTime = simTime();
                        addressTableBasic[switchAddress] = entryBasic;
                    }
                    else //Else -> discard
                    {
                        EV << "        ...discard!" << endl;
                        discardFrame = true;
                    }
                }

                //2nd) If src directly connected send PathRequest, otherwise broadcast! (late copies are previously discarded)
                if(!discardFrame)
                {
                    int port = portOfHost(srcAddress);
                    if (port !=-1 /*&& entry.port != -1*/)  //If src directly connected, send PathRequest
                    {
                        EV << "    Switch is directly connected to src host!" << endl; //TODO: Aquí podría añadirse otra RT (frontera)
                        entryExists = findEntry(srcAddress, entry);
                        entry.inTime = simTime();
                        entry.status = locked;     //Update entry to 'locked' (the learning process starts again)
                        addressTable[srcAddress] = entry;
                        broadcastFrame(repairFrame(PathRequest, srcAddress, dstAddress, repairTime),port,true); //Include repairTime for the new PathRepair message
                        discardFrame = true;

                        updateRepairStatistics(prf, type); //Update repair statistics (PathFail time) with PathRepair frame
                    }
                    else //Otherwise, continue broadcasting PathFail
                    {
                        EV << "    Src host not found (not directly connected)! ...broadcast!" << endl;
                        broadcastFrame(frame,inputport,true);
                    }
                }
            }
            else if (type == PathRequest) //PathRequest
            {
                PathRequestRcvd++;
                EV << "  'PathRequest' received from port: " << inputport << endl;

                //1st) Apply 'locking' filter to avoid loops and 'learn' info about srcAddress (LT)
                entryExists = findEntry(srcAddress, entry);
                if (entryExists)
                {
                    if(entry.status == locked)
                    {
                        EV << "      Entry found -> 'locked' status..." << endl;
                        if (inputport == entry.port)
                        {
                            EV << "        ...refresh!" << endl;
                            entry.inTime = simTime(); //Refresco
                            addressTable[srcAddress] = entry;
                        }
                        else
                        {
                            EV << "        ...discard!" << endl;
                            discardFrame = true;
                        }
                    }
                    else
                    {
                        EV << "      Entry found -> 'learnt' status goes 'locked' as a new entry..." << endl;
                        entry.portToSend = -1; //#V3 - En v3 se resetea el portToSend a -1 porque si no conservaría el valor viejo tras 'addEntry'
                        addEntry(entry, srcAddress, inputport);
                    }
                }
                else
                {
                    EV << "      Entry not found -> Add new 'locked' entry..." << endl;
                    addEntry(entry, srcAddress, inputport);
                }

                //2nd) If dst directly connected send PathReply (if applicable), otherwise broadcast! (late copies are previously discarded)
                if(!discardFrame)
                {
                    int port = portOfHost(dstAddress);
                    if (port !=-1 /*&& entry.port != -1*/) //If dst directly connected, send PathReply only if applicable (i.e. version 1)
                    {
                         EV << "    Switch is directly connected to dst host!" << endl; //TODO: Aquí podría añadirse otra RT (frontera) - sólo para PathReply (versión 1)
                         if(repairType == 1)
                         {
                             entryExists = findEntry(dstAddress, entry);
                             entry.inTime = simTime();
                             entry.status = locked;     //Update entry to 'locked' (the learning process starts again)
                             addressTable[dstAddress] = entry;
                             send((EtherFrame*)repairFrame(PathReply, dstAddress, srcAddress, repairTime), "lowerLayerOut", inputport); //Reverse srcAddress/dstAddress to create the PathReply and sent through inputport (the port that received the PathRequest) //Include repairTime for the new PathRepair message
                             EV << "Forwarding through port: " << inputport << endl;
                         }
                         discardFrame = true;

                         updateRepairStatistics(prf, type); //Update repair statistics (PathRequest time and Repair time if repairType == 2) with PathRepair frame
                     }
                     else //Otherwise, continue broadcasting PathRequest
                     {
                         EV << "    Dst host not found (not directly connected)! ...broadcast!" << endl;
                         broadcastFrame(frame,inputport,true);
                     }
                }
            }
            else if (type == PathReply) //PathReply
            {
                PathReplyRcvd++;
                EV << "  'PathReply' received from port: " << inputport << endl;

                //1st) Learning
                entryExists = findEntry(srcAddress, entry);
                if (entryExists)
                {
                    if(entry.status == locked)
                    {
                        EV << "      Entry found -> 'locked' status..." << endl;
                        if (inputport == entry.port)
                        {
                            entry.inTime = simTime(); //Refresco
                            addressTable[srcAddress] = entry;
                        }
                        //else //No hacemos nada si no coincide (forward as if entry port was the same, but no refresh)
                    }
                    else
                    {
                        EV << "      Entry found -> 'learnt' status goes 'locked' as a new entry..." << endl;
                        entry.portToSend = -1; //#V3 - En v3 se resetea el portToSend a -1 porque si no conservaría el valor viejo tras 'addEntry'
                        addEntry(entry, srcAddress, inputport);
                    }
                }
                else
                {
                    EV << "      Entry not found -> Add new 'locked' entry..." << endl;
                    addEntry(entry, srcAddress, inputport);
                }
                //TODO:Se podría pensar sobre si añadir las entradas como 'locked' (como ahora) o directamente 'learnt'

                //2nd) Forwarding
                AddressEntry destEntry;
                entryExists = findEntry(dstAddress, destEntry);
                if (entryExists)
                {
                    EV << "      Output entry found at port: " << destEntry.port << endl;
                    EV << "        ...refresh destination + forward" << endl;
                    if(destEntry.status == learnt)
                    {
                        destEntry.inTime = simTime(); //Refresco sólo si la entrada ya está en estado 'learnt' (si no, estaríamos refrescando el estado 'locked' y eso no se hace con unicast)
                        addressTable[dstAddress] = destEntry;
                    }
                    if(!inHoT(destEntry.port)) //Forward only if destination host is not directly connected (i.e. that port is not in HoT's list)
                    {
                        send(frame,"lowerLayerOut",destEntry.port);
                        EV << "Forwarding through port: " << destEntry.port << endl;
                    }
                    else
                    {
                        EV << "    Switch is directly connected to dst host! Stop repair v1 (finished!)" << endl;
                        discardFrame = true;
                        updateRepairStatistics(prf, type); //Update repair statistics (PathReply time and Repair time if repairType == 1) with PathRepair frame
                    }
                }
                else
                {
                    EV << "      Output entry not found -> Starting repair..." << endl;
                    startPathRepair(srcAddress,dstAddress);
                    discardFrame = true; //Original frame is discarded/lost
                }
            }
            else if (type == LinkFail) //LinkFail
            {
                LinkFailRcvd++;
                EV << "  'LinkFail' received from port: " << inputport << endl;

                //1st) Apply 'locking' filter to avoid loops and 'learn' info about switchAddress (LT)
                entryExists = findEntry(switchAddress, entry);
                if (entryExists)
                {
                    if(entry.status == locked)
                    {
                        EV << "      Entry found -> 'locked' status..." << endl;
                        if (inputport == entry.port)
                        {
                            EV << "        ...refresh!" << endl;
                            entry.inTime = simTime(); //Refresco
                            addressTable[switchAddress] = entry;
                        }
                        else
                        {
                            EV << "        ...discard!" << endl;
                            discardFrame = true;
                        }
                    }
                    else
                    {
                        EV << "      Entry found -> 'learnt' status goes 'locked' as a new entry..." << endl;
                        entry.portToSend = -1; //#V3 - En v3 se resetea el portToSend a -1 porque si no conservaría el valor viejo tras 'addEntry'
                        addEntry(entry, switchAddress, inputport);
                    }
                }
                else
                {
                    EV << "      Entry not found -> Add new 'locked' entry..." << endl;
                    addEntry(entry, switchAddress, inputport);
                }

                /* Segunda opción sería guardar en BT en lugar de LT (a continuación...)
                //1st) Apply 'locking' filter to avoid loops (BT)
                entryBasic = findEntryBasic(switchAddress);
                if (entryBasic.port == -1) //Not found, add address to bcast table
                {
                    EV << "      Entry not found -> Add new 'broadcast' entry..." << endl;
                    addEntryBasic(entryBasic, switchAddress, inputport);
                }
                else //Address found
                {
                    EV << "      Entry was found..." << endl;
                    if (entryBasic.port == inputport) //If port is coincident -> refresh entry
                    {
                        EV << "        ...refresh!" << endl;
                        entryBasic.inTime = simTime();
                        addressTableBasic[switchAddress] = entryBasic;
                    }
                    else //Else -> discard
                    {
                        EV << "        ...discard!" << endl;
                        discardFrame = true;
                        return; //No more actions to be applied
                    }
                }*/

                //2nd) If src directly connected send LinkReply, otherwise broadcast! (late copies are previously discarded)
                if(!discardFrame)
                {
                    MACAddressVector replyAddressVector;
                    for(unsigned int i=0; i<addressVector.size(); i++)
                    {
                        EV << "    Updating LinkFail..." << endl;
                        EV << "      Address: " << addressVector[i];
                        int port = portOfHost(addressVector[i]);
                        if (port != -1)
                        {
                            //Update associated entry
                            entryExists = findEntry(addressVector[i], entry);
                            entry.inTime = simTime();
                            entry.status = locked;     //Update entry to 'locked' (the learning process starts again)
                            addressTable[srcAddress] = entry;

                            EV << " -> Moved to LinkReply!";
                            replyAddressVector.push_back(addressVector[i]); //Add to reply list
                            addressVector.erase(addressVector.begin()+i);   //Erase from original list
                            i--;
                        }
                        EV << endl;
                    }

                    if(!addressVector.empty()) //If still addresses to be found, broadcast LinkFail
                    {
                        EV << "    Broadcasting LinkFail!" << endl;
                        broadcastFrame(frame,inputport,true);
                    }
                    else
                        discardFrame = true;

                    if(!replyAddressVector.empty()) //If some address is directly connected, send LinkReply
                    {
                        EV << "    Switch is directly connected to some src host -> Forwarding LinkReply (new)!" << endl;
                        send((EtherFrame*)groupRepairFrame(LinkReply, switchAddress, replyAddressVector, repairTime), "lowerLayerOut", inputport); //LinkReply needs switchAddress (switch that originated the repair) in order to be forwarded //Include repairTime for the new PathRepair message
                        EV << "Forwarding through port: " << inputport << endl;

                        updateRepairStatistics(prf, type, replyAddressVector.size()); //Update repair statistics (LinkFail time) with PathRepair frame - we indicate the number of host directly connected/being repaired
                    }
                }
            }
            else if (type == LinkReply) //LinkReply
            {
                LinkReplyRcvd++;
                EV << "  'LinkReply' received from port: " << inputport << endl;

                //1st) Learning
                for(unsigned int i=0; i<addressVector.size(); i++)
                {
                    entryExists = findEntry(addressVector[i], entry);
                    if (entryExists)
                    {
                        if(entry.status == locked)
                        {
                            EV << "      Entry found -> 'locked' status..." << endl;
                            if (inputport == entry.port)
                            {
                                entry.inTime = simTime(); //Refresco
                                addressTable[addressVector[i]] = entry;
                            }
                            //else //No hacemos nada si no coincide (forward as if entry port was the same, but no refresh)
                        }
                        else
                        {
                            EV << "      Entry found -> 'learnt' status goes 'locked' as a new entry..." << endl;
                            entry.portToSend = -1; //#V3 - En v3 se resetea el portToSend a -1 porque si no conservaría el valor viejo tras 'addEntry'
                            addEntry(entry, addressVector[i], inputport);
                        }
                    }
                    else
                    {
                        EV << "      Entry not found -> Add new 'locked' entry..." << endl;
                        addEntry(entry, addressVector[i], inputport);
                    }
                    //TODO:Se podría pensar sobre si añadir las entradas como 'locked' (como ahora) o directamente 'learnt'
                }

                //2nd) Forwarding
                if(myAddress == repairSwitch) //If we reached the switch which originated the repair, stop repair
                {
                    EV << "    Repair switch reached! Stop repair v3 (finished!)" << endl;
                    discardFrame = true;
                    updateRepairStatistics(prf, type, addressVector.size()); //Update repair statistics (PathReply time and Repair time) with PathRepair frame - we indicate the number of host directly connected/being repaired
                }
                else
                {
                    AddressEntry destEntry;
                    entryExists = findEntry(repairSwitch, destEntry); //To the 'LinkFail switch'
                    if (entryExists)
                    {
                        EV << "      Output entry found at port: " << destEntry.port << endl;
                        EV << "        ...refresh destination + forward" << endl;
                        if(destEntry.status == learnt)
                        {
                            destEntry.inTime = simTime(); //Refresco sólo si la entrada ya está en estado 'learnt' (si no, estaríamos refrescando el estado 'locked' y eso no se hace con unicast)
                            addressTable[dstAddress] = destEntry;
                        }

                        send(frame,"lowerLayerOut",destEntry.port);
                        EV << "Forwarding through port: " << destEntry.port << endl;
                    }
                    else
                    {
                        EV << "      Output entry not found -> Discard frame and wait for a new repair..." << endl;
                        discardFrame = true; //Original frame is discarded/lost
                    }
                }
            }

            //Finally, if frame was discarded, delete it
            if(discardFrame)
                delete (frame);
        }
    }
}

void MACRelayUnitAPB::updateRepairStatistics(PathRepair* prframe, unsigned int type, unsigned int nRepair)
{
    simtime_t delay = simTime()-prframe->getCreationTime();
    //unsigned int type = prframe->getType(); //ERS: Es raro, pero esta línea da error de ejecución (quizás es muy evidente el error y no lo veo, así que lo paso como parámetro 27/09/2013)

    if(type == PathFail)
    {
        nPF++;
        if(nPF > 1) //Not first PathFail
        {
            averagePFTime = (averagePFTime*(nPF-1)+delay)/nPF;
            if(delay > maxPFTime) maxPFTime = delay;
            if(delay < minPFTime) minPFTime = delay;
        }
        else //First frame
            averagePFTime = maxPFTime = minPFTime = delay;
    }
    else if(type == PathRequest)
    {
        nPRq++;
        if(nPRq > 1) //Not first PathFail
        {
            averagePRqTime = (averagePRqTime*(nPRq-1)+delay)/nPRq;
            if(delay > maxPRqTime) maxPRqTime = delay;
            if(delay < minPRqTime) minPRqTime = delay;
        }
        else //First frame
            averagePRqTime = maxPRqTime = minPRqTime = delay;

        //Update global repair time if repair type == 2 (PathRequest was the last message)
        if(repairType==2)
        {
            simtime_t globalRepairTime = delay + prframe->getRepairTime(); //Calculate global repair time with current delay + cumulative previous delay from other packets

            nRepairFinished++;
            if(nRepairFinished > 1) //Not first finished repair
            {
                averageRepairTime = (averageRepairTime*(nRepairFinished-1)+globalRepairTime)/nRepairFinished;
                if(globalRepairTime > maxRepairTime) maxRepairTime = globalRepairTime;
                if(globalRepairTime < minRepairTime) minRepairTime = globalRepairTime;
            }
            else //First finished repair
                averageRepairTime = maxRepairTime = minRepairTime = globalRepairTime;
        }
    }
    else if(type == PathReply)
    {
        nPRy++;
        if(nPRy > 1) //Not first PathFail
        {
            averagePRyTime = (averagePRqTime*(nPRy-1)+delay)/nPRy;
            if(delay > maxPRyTime) maxPRyTime = delay;
            if(delay < minPRyTime) minPRyTime = delay;
        }
        else //First frame
            averagePRyTime = maxPRyTime = minPRyTime = delay;

        //Update global repair time if repair type == 1 (PathReply was the last message)
        if(repairType==1) //If we are here, repairType will always be 1, no need to check indeed
        {
            simtime_t globalRepairTime = delay + prframe->getRepairTime(); //Calculate global repair time with current delay + cumulative previous delay from other packets

            nRepairFinished++;
            if(nRepairFinished > 1) //Not first finished repair
            {
                averageRepairTime = (averageRepairTime*(nRepairFinished-1)+globalRepairTime)/nRepairFinished;
                if(globalRepairTime > maxRepairTime) maxRepairTime = globalRepairTime;
                if(globalRepairTime < minRepairTime) minRepairTime = globalRepairTime;
            }
            else //First finished repair
                averageRepairTime = maxRepairTime = minRepairTime = globalRepairTime;
        }
    }
    else if(type == LinkFail)
    {
        delay = delay/nRepair; //delay per host if there is a group
        if(nLF > 0) //Not first LinkFail
        {
            averageLFTime = (averageLFTime*nLFi+delay*nRepair)/(nLFi+nRepair);
            if(delay > maxLFTime) maxLFTime = delay;
            if(delay < minLFTime) minLFTime = delay;
        }
        else //First frame
            averageLFTime = maxLFTime = minLFTime = delay;
        nLFi = nLFi + nRepair; //Total number of addresses repaired by LinkFail +nRepair
        nLF++; //Total number of LinkFail +1
    }
    else if(type == LinkReply)
    {
        delay = delay/nRepair; //delay per host if there is a group
        if(nLR > 0) //Not first LinkFail
        {
            averageLRTime = (averageLRTime*nLRi+delay*nRepair)/(nLRi+nRepair);
            if(delay > maxLRTime) maxLRTime = delay;
            if(delay < minLRTime) minLRTime = delay;
        }
        else //First frame
            averageLRTime = maxLRTime = minLRTime = delay;
        nLRi = nLRi + nRepair; //Total number of addresses repaired by LinkReply +nRepair
        nLR++; //Total number of LinkReply +1

        //Update global repair time
        simtime_t globalRepairTime = simTime()-prframe->getCreationTime() + prframe->getRepairTime(); //Calculate global repair time with current delay + cumulative previous delay from other packets
        globalRepairTime = globalRepairTime/nRepair; //global time per host if there is a group

        if(nRepairFinished > 0) //Not first finished repair
        {
            averageRepairTime = (averageRepairTime*nRepairFinished+globalRepairTime*nRepair)/(nRepairFinished+nRepair);
            if(globalRepairTime > maxRepairTime) maxRepairTime = globalRepairTime;
            if(globalRepairTime < minRepairTime) minRepairTime = globalRepairTime;
        }
        else //First finished repair
            averageRepairTime = maxRepairTime = minRepairTime = globalRepairTime;
        nRepairFinished = nRepairFinished + nRepair; //Indicates the number of addresses repaired (not the total numer of finished repairs... TODO: This could be changed if needed, but it's more useful in this way)
    }
    else
        EV << "  updateRepairStatistics ERROR! Type is not correct! (" << type << ")" << endl;
}

void MACRelayUnitAPB::multicastFrame(EtherFrame *frame, int inputport, AddressEntryMulticast entry)
{
    std::vector<int> sequence;
    generateSendSequence(sequence);

    EV << "Multicasting... (inputport=" << inputport << ") -> ports order = ";
    for (int i=0; i<numPorts; ++i)
    {
        int port = sequence.at(i);
        EV << port;
        if (port!=inputport) //inputport =-1 if we should broadcast through all the ports
        {
            if(entry.port[port]) //If port is subscribed (==true)
            {
                if (!linkDownForPort(port))
                {
                    send((EtherFrame*)frame->dup(), "lowerLayerOut", port);
                    EV << "(s) "; //Sent
                }
                else EV << "(ld!) "; //Link is down! (not sent)
            }
            else EV << "(ns!)" ; //Not subscribed! (not sent)
        }
        else EV << "(ip!) "; //Input port! (not sent)
    }
    EV << endl;
    delete frame;
}

void MACRelayUnitAPB::broadcastFrame(EtherFrame *frame, int inputport, bool repairMessage)
{
    std::vector<int> sequence;
    generateSendSequence(sequence);

    EV << "Broadcasting... (inputport=" << inputport << "; repairMessage=" << repairMessage << ") -> ports order = ";
    for (int i=0; i<numPorts; ++i)
    {
        int port = sequence.at(i);
        EV << port;
        if (port!=inputport) //inputport =-1 if we should broadcast through all the ports
        {
            if(!repairMessage || !inHoT(port)) //If it is not a repair message or if it is, but the port is not directly connected to a host (special repair messages are not sent to hosts)
            {
                if (!linkDownForPort(port))
                {
                    send((EtherFrame*)frame->dup(), "lowerLayerOut", port);
                    EV << "(s) "; //Sent
                }
                else EV << "(ld!) "; //Link is down! (not sent)
            }
            else EV << "(#)" ; //Link is directly connected to a host and we are broadcasting a repair message
        }
        else EV << "(ip!) "; //Input port (not sent)
    }
    EV << endl;
    delete frame;
}

void MACRelayUnitAPB::generateSendSequence(std::vector<int>& sequence)
{
    int r_iter;
    std::vector<int>::iterator iter;

    iter = sequence.begin();
    for (int i=0;i<numPorts;i++) //Vamos insertando por orden los puertos del 0 a numPorts
    {
        if (sequence.size()!=0)
        {
            r_iter = rand()%(sequence.size()+1); //r_iter da la próxima posición para colocar el puerto, que va desde 0 a 'sequence.size()' o 'i' (que siempre serán iguales porque size aumenta 1 cada vez que i aumenta 1 y ambos empiezan en cero)
            iter = sequence.begin()+r_iter; //Coloca el iterador..
        }
        sequence.insert(iter,i); //... e inserta en dicha posición al azar (el 0 siempre se inserta el primero en .begin())
    }
}

int MACRelayUnitAPB::getNumberOfEntriesForPort(int port){
     AddressTable::iterator iter;
     int number=0;
     for (iter = addressTable.begin(); iter!=addressTable.end(); ++iter)
     {
         if (iter->second.port == port ){//&& !strcmp(iter->second.status,"learnt")){
             number++;
         }
     }
    return number;
}

//Method that prints the main tables of the switch
void MACRelayUnitAPB::printTables()
{
    printLT();
    printBT();
    printHeT();
    printRT();
}

//Method to print the contents of the address table (LT).
void MACRelayUnitAPB::printLT()
{
    AddressTable::iterator iter;
    EV << endl;
    EV << "Address Table (" << this->getParentModule()->getFullName() << ") (" << addressTable.size() << " entries):\n";
    EV << "  ----------------------------------------------------------------------------------" << endl;
    EV << "  | Address                     | Port        | Timer | Status  | IP                  |" << endl;
    EV << "  ----------------------------------------------------------------------------------" << endl;
    for (iter = addressTable.begin(); iter!=addressTable.end(); iter++)
    {
        EV << "  | " << iter->first << " |    " << iter->second.port << " [ " << iter->second.portToSend << " ] | ";
                float time = simTime().dbl()-iter->second.inTime.dbl();
                char buffer[20];
                sprintf(buffer, "%.03f | ", time);
                EV << buffer; //Print timer just with 3 decimals
                if(iter->second.status == locked) EV << "locked | ";
                else EV << "learnt | ";
                EV << iter->second.ip << "    |" << endl;
     //   EV << "  ---------------------------------------------------------------" << endl;
    }
}

//Method to print the contents of BT
void MACRelayUnitAPB::printBT()
{
    EV << endl;
    EV << "    BT (" << this->getParentModule()->getFullName() << ") (" << addressTableBasic.size() << " entries):\n";
    EV << "      ------------------------------------------------" << endl;
    EV << "      | Address                     | Port | Timer |" << endl;
    EV << "      ------------------------------------------------" << endl;

    for (AddressTableBasic::iterator iter = addressTableBasic.begin(); iter != addressTableBasic.end(); iter++)
    {
        AddressEntryBasic& entry = iter->second;

        EV << "      | " << iter->first << " |    " << entry.port << "    | ";
                float time = simTime().dbl()-entry.inTime.dbl();
                char buffer[20];
                sprintf(buffer, "%.03f |", time);
                EV << buffer << endl; //Print timer just with 3 decimals
    }
}

//Method to print the contents of HeT
void MACRelayUnitAPB::printHeT()
{
    EV << endl;
    EV << "    HeT (" << this->getParentModule()->getFullName() << ") (" << notHostList.size() << " entries):\n";
    EV << "      ---------" << endl;
    EV << "      | Port |" << endl;
    EV << "      ---------" << endl;

    for(unsigned int i=0; i<notHostList.size(); i++)
        EV << "      |     " << notHostList[i] << "   |" << endl;
}

//Method to print the contents of RT
void MACRelayUnitAPB::printRT()
{
    EV << endl;
    EV << "    RT (" << this->getParentModule()->getFullName() << ") (" << repairTable.size() << " entries):\n";
    EV << "      -------------------------------------------" << endl;
    EV << "      | Address                       | Timer |" << endl;
    EV << "      -------------------------------------------" << endl;

    for (RepairTable::iterator iter = repairTable.begin(); iter != repairTable.end(); iter++)
    {
        RepairEntry& entry = iter->second;

        EV << "      | " << iter->first << " |    ";
                float time = simTime().dbl()-entry.inTime.dbl();
                char buffer[20];
                sprintf(buffer, "%.03f |", time);
                EV << buffer << endl; //Print timer just with 3 decimals
    }
}

//Print linkDownTable
void MACRelayUnitAPB::printLinkDownTable(void)
{
    PortEventTable::iterator iter;
    //linkDownTable
    EV << endl;
    EV << "Link Down Table (" << this->getParentModule()->getFullName() << ") (" << linkDownTable.size() << " entries):\n";
    EV << "  ---------------------------------------------------" << endl;
    EV << "  | Port | #Events | Init time | End time |" << endl;
    EV << "  ---------------------------------------------------" << endl;
    for (iter = linkDownTable.begin(); iter!=linkDownTable.end(); ++iter)
    {
        EV << "  |  " << iter->first;
        EV << "     |    " << iter->second.nEvents << "       ";
        for(unsigned int i=0; i<iter->second.nEvents; i++)
        {
            EV << "    |     " << iter->second.initTime[i];
            EV << "           |     " << iter->second.endTime[i] << "           |" <<endl;
            if (i<iter->second.nEvents-1)
                EV << "                    -----------------------" << endl << "               ";
        }
        EV << "  ---------------------------------------------------" << endl;
    }
}

//Print switchDownStruct
void MACRelayUnitAPB::printSwitchDownStruct(void)
{
    //switchDownStruct
    EV << endl;
    EV << "Switch Down Struct (" << this->getParentModule()->getFullName() << ") (" << switchDownStruct.nEvents << " entries):\n";
    EV << "  -------------------------------------------" << endl;
    EV << "  | #Events | Init time | End time |" << endl;
    EV << "  -------------------------------------------" << endl;
    if(!switchDownStruct.nEvents) return;
    EV << "  |  " << switchDownStruct.nEvents << "        ";
    for(unsigned int i=0; i<switchDownStruct.nEvents; i++)
    {
        EV << "    |     " << switchDownStruct.initTime[i];
        EV << "           |     " << switchDownStruct.endTime[i] << "           |" <<endl;
    }
    EV << "  -------------------------------------------" << endl;
}

void MACRelayUnitAPB::printRouteStatistics()
{
    EV << "->MACRelayUnitAPB::printRouteStatistics()" << endl;

    MACRelayUnitRoute * pRelayUnitRoute = check_and_cast<MACRelayUnitRoute *>(this->getParentModule()->getSubmodule("relayUnitRoute"));
    pRelayUnitRoute->printRouteStatistics();

    EV << "<-MACRelayUnitAPB::printRouteStatistics()" << endl;
}




