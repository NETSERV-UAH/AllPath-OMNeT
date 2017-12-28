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

#include "MACRelayUnitAPB.h"

#include "inet/common/LayeredProtocolBase.h"
#include "inet/common/lifecycle/NodeOperations.h"
#include "inet/common/ModuleAccess.h"
#include "inet/linklayer/ethernet/EtherFrame.h"
#include "inet/linklayer/ethernet/EtherMACBase.h"
#include "inet/linklayer/ethernet/Ethernet.h"
#include "LinkFailPkt_m.h"
#include "inet/networklayer/arp/ipv4/ARPPacket_m.h"
#include "inet/networklayer/ipv4/IPv4Datagram.h"

#include "../EtherMACFullDuplexNew.h"
using namespace inet;

namespace allpath {

Define_Module(MACRelayUnitAPB);

void MACRelayUnitAPB::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        //our change
        isArpPath = par("isArpPath");
        isRerouteArpPath = par("isRerouteArpPath");
        isArpPathLinkFail = par("isArpPathLinkFail");
        isIndependentPath = par("isIndependentPath");
        isLowResArpPath = par("isLowResArpPath");
        isUsingL3inL2 = par("isUsingL3inL2");
        isOurFlowPath = par("isOurFlowPath");
        isMulticastPath = par("isMulticastPath");
        isEdgeSwitch = par("isEdgeSwitch");
        blockingTableAgingTime = par("blockingTableAgingTime");
        learningTableAgingTime = par("learningTableAgingTime");

        if (isArpPath || isRerouteArpPath || isArpPathLinkFail || isIndependentPath || isLowResArpPath)
        {
            BT=new ArpPathMacAddressTable(blockingTableAgingTime);  // without IP address
            LT=new ArpPathMacAddressTable(learningTableAgingTime);
        }else if (isUsingL3inL2)
        {
            BT2=new ArpPathIPMacAddressTable(blockingTableAgingTime); // with IP Address, IP is not the key
            LT2=new ArpPathIPMacAddressTable(learningTableAgingTime);
        }else if (isOurFlowPath)
        {
            BT3=new ArpPathIPandMacAddressTable(blockingTableAgingTime); // with IP Address, IP is the key too
            LT3=new ArpPathIPandMacAddressTable(learningTableAgingTime);
        }else if (isMulticastPath)
        {
            BT4 = new ArpPathMulticastMacAddressTable(blockingTableAgingTime);
            LT4 = new ArpPathMulticastMacAddressTable(learningTableAgingTime);
        }
        //listener = new MyListener;
        //getSimulation()->getSystemModule()->subscribe(PRE_MODEL_CHANGE, listener);
        getSimulation()->getSystemModule()->subscribe(PRE_MODEL_CHANGE, this);



        // number of ports
        numPorts = gate("ifOut", 0)->size();
        if (gate("ifIn", 0)->size() != numPorts)
            throw cRuntimeError("the sizes of the ifIn[] and ifOut[] gate vectors must be the same");

        numProcessedFrames = numDiscardedFrames = 0;

        addressTable = check_and_cast<IMACAddressTable *>(getModuleByPath(par("macTablePath")));

        WATCH(numProcessedFrames);
        WATCH(numDiscardedFrames);
        WATCH(forwaringState);
        WATCH(numARP_PathBroadcastFrames);
        WATCH(numARP_PathMulticastFrames);
        WATCH(numARP_PathUnicastFrames);

    }
    else if (stage == INITSTAGE_LINK_LAYER) {
        NodeStatus *nodeStatus = dynamic_cast<NodeStatus *>(findContainingNode(this)->getSubmodule("status"));
        isOperational = (!nodeStatus) || nodeStatus->getState() == NodeStatus::UP;
    }
}

void MACRelayUnitAPB::handleMessage(cMessage *msg)
{
    if (!isOperational) {
        EV << "Message '" << msg << "' arrived when module status is down, dropped it\n";
        delete msg;
        return;
    }
    EtherFrame *frame = check_and_cast<EtherFrame *>(msg);
    // Frame received from MAC unit
    emit(LayeredProtocolBase::packetReceivedFromLowerSignal, frame);
    handleAndDispatchFrame(frame);
}

void MACRelayUnitAPB::handleAndDispatchFrame(EtherFrame *frame)
{
    int inputport = frame->getArrivalGate()->getIndex();

    numProcessedFrames++;


    //our change
    if (isArpPath)                     //Standard ARP-Path
        handleArpPath(frame, inputport);
    else if (isArpPathLinkFail)        //Standard ARP-Path with Link Fail property
        handleLinkFailArpPath(frame, inputport);
    else if (isRerouteArpPath)         //this is not a new approach because of oscillation.
        handleReroutableARPPath(frame, inputport);
    else if(isLowResArpPath)            //Single table ARP-Path
        handleLowResArpPath(frame, inputport);
    else if (isIndependentPath)         //Independent-Path
        handleIndependentPath(frame, inputport);
    else if (isUsingL3inL2)             //Using L3 addresses beside L2 addresses in ARP-Path as source address
        handleUsingL3inL2Path(frame, inputport);
    else if (isOurFlowPath)             //Using L3 addresses beside L2 addresses in ARP-Path as destination address to use lower granularity
        handleOurFlowPath(frame, inputport);
    else if (isMulticastPath)           //this approach accumulates late frames and learns port of late frames and creates a multicast group based on this information.
        handleMulticastPath(frame, inputport);


   else    //traditional switching
   {
       // update address table
       addressTable->updateTableWithAddress(inputport, frame->getSrc());



           // handle broadcast frames first
       if (frame->getDest().isBroadcast()) {
           EV << "Broadcasting broadcast frame " << frame << endl;
           broadcastFrame(frame, inputport);
           return;
       }

       // Finds output port of destination address and sends to output port
       // if not found then broadcasts to all other ports instead
       int outputport = addressTable->getPortForAddress(frame->getDest());
       // should not send out the same frame on the same ethernet port
       // (although wireless ports are ok to receive the same message)
       if (inputport == outputport) {
           EV << "Output port is same as input port, " << frame->getFullName()
              << " dest " << frame->getDest() << ", discarding frame\n";
           numDiscardedFrames++;
           delete frame;
           return;
       }

       if (outputport >= 0) {
           EV << "Sending frame " << frame << " with dest address " << frame->getDest() << " to port " << outputport << endl;
           emit(LayeredProtocolBase::packetSentToLowerSignal, frame);
           send(frame, "ifOut", outputport);
       }
       else {
           EV << "Dest address " << frame->getDest() << " unknown, broadcasting frame " << frame << endl;
           broadcastFrame(frame, inputport);
       }

   } //end of our else






}

void MACRelayUnitAPB::handleArpPath(EtherFrame *frame, int inputport){

    if ((frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
        if(((BT->getPortForAddress(frame->getSrc()))==-1)||(((BT->getPortForAddress(frame->getSrc()))!=-1)&&((BT->getPortForAddress(frame->getSrc()))==inputport)))
        {
            if((strcmp(frame->getName(),"arpREQ")==0)&&((LT->getPortForAddress(frame->getSrc()))==-1))
                LT->updateTableWithAddress(inputport,frame->getSrc());
            BT->updateTableWithAddress(inputport,frame->getSrc());
            //EV<<"BT update port : #"<<inputport<<"  src address:"<<frame->getSrc();
            // EV << "Broadcasting broadcast frame " << frame << endl;
            broadcastFrame(frame, inputport);
        }else
        {
            //EV << "Frame is received before, " << frame->getFullName()
            //     << " dest " << frame->getDest() << ", discarding frame\n";
            // numDiscardedFrames++;
            delete frame;
        }else if (!(frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
        {
            if((strcmp(frame->getName(),"arpREPLY")==0)&&((LT->getPortForAddress(frame->getSrc()))==-1))
            {
                LT->updateTableWithAddress(inputport,frame->getSrc());
                forwaringState++;
            }
            int outport = LT->getPortForAddress(frame->getDest());
            LT->updateTableWithAddress(outport,frame->getDest());
            send(frame, "ifOut", outport);
        }

}

void MACRelayUnitAPB::handleReroutableARPPath(EtherFrame *frame, int inputport){

    if ((frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
       if(((BT->getPortForAddress(frame->getSrc()))==-1)||(((BT->getPortForAddress(frame->getSrc()))!=-1)&&((BT->getPortForAddress(frame->getSrc()))==inputport)))
       {
           if((strcmp(frame->getName(),"arpREQ")==0)    )//&&((LT->getPortForAddress(frame->getSrc()))==-1))
                LT->updateTableWithAddress(inputport,frame->getSrc());
            BT->updateTableWithAddress(inputport,frame->getSrc());


            //EV<<"BT update port : #"<<inputport<<"  src address:"<<frame->getSrc();
           // EV << "Broadcasting broadcast frame " << frame << endl;
           broadcastFrame(frame, inputport);
        }else
        {
            //EV << "Frame is received before, " << frame->getFullName()
                  //     << " dest " << frame->getDest() << ", discarding frame\n";
            //numDiscardedFrames++;
            delete frame;
        }
    else if (!(frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
    {
        if((strcmp(frame->getName(),"arpREPLY")==0)&&((LT->getPortForAddress(frame->getSrc()))==-1))
        {
            LT->updateTableWithAddress(inputport,frame->getSrc());
            forwaringState++;
        }
        int outport = LT->getPortForAddress(frame->getDest());
        LT->updateTableWithAddress(outport,frame->getDest());
        send(frame, "ifOut", outport);


    }

}

void MACRelayUnitAPB::handleLinkFailArpPath(EtherFrame *frame, int inputport){

    if ((frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
       if(((BT->getPortForAddress(frame->getSrc()))==-1)||(((BT->getPortForAddress(frame->getSrc()))!=-1)&&((BT->getPortForAddress(frame->getSrc()))==inputport)))
       {
           if(((strcmp(frame->getName(),"arpREQ")==0)&&((LT->getPortForAddress(frame->getSrc()))==-1))||(strcmp(frame->getName(),"LinkFail")==0))
                LT->updateTableWithAddress(inputport,frame->getSrc());
            BT->updateTableWithAddress(inputport,frame->getSrc());

            if((strcmp(frame->getName(),"LinkFail")==0)&&(isEncapsulatedMacDirectlyConnected(frame)))
            {
                sendToEncapsulatedMacDirectlyConnected(frame, inputport);

            }else
            //EV<<"BT update port : #"<<inputport<<"  src address:"<<frame->getSrc();
           // EV << "Broadcasting broadcast frame " << frame << endl;
                     broadcastFrame(frame, inputport);
        }else{
            //EV << "Frame is received before, " << frame->getFullName()
                  //     << " dest " << frame->getDest() << ", discarding frame\n";
            //numDiscardedFrames++;
            delete frame;
        }
    else if (!(frame->getDest().isBroadcast())||(frame->getDest().isMulticast())){
        if(((strcmp(frame->getName(),"arpREPLY")==0)&&((LT->getPortForAddress(frame->getSrc()))==-1))||(strcmp(frame->getName(),"LinkReply")==0)){
            LT->updateTableWithAddress(inputport,frame->getSrc());
            forwaringState++;
        }
        int outport = LT->getPortForAddress(frame->getDest());
        LT->updateTableWithAddress(outport,frame->getDest());
        if(outport!=-1)
            send(frame, "ifOut", outport);


    }

}

void MACRelayUnitAPB::handleLowResArpPath(EtherFrame *frame, int inputport){

    if ((frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
 //      if(((BT->getPortForAddress(frame->getSrc()))==-1)||(((BT->getPortForAddress(frame->getSrc()))!=-1)&&((BT->getPortForAddress(frame->getSrc()))==inputport))){


        if ((LT->getPortForAddress(frame->getSrc()))==-1)//// if((strcmp(frame->getName(),"arpREQ")==0)&&((LT->getPortForAddress(frame->getSrc()))==-1))
            {
                LT->updateTableWithAddress(inputport,frame->getSrc());
     //       BT->updateTableWithAddress(inputport,frame->getSrc());
            //EV<<"BT update port : #"<<inputport<<"  src address:"<<frame->getSrc();
           // EV << "Broadcasting broadcast frame " << frame << endl;
            broadcastFrame(frame, inputport);
        }else{
            //EV << "Frame is received before, " << frame->getFullName()
                  //     << " dest " << frame->getDest() << ", discarding frame\n";
            //numDiscardedFrames++;
            delete frame;
        }
    else if (!(frame->getDest().isBroadcast())||(frame->getDest().isMulticast())){
        if (LT->getPortForAddress(frame->getSrc())==-1){ //////if((strcmp(frame->getName(),"arpREPLY")==0)&&((LT->getPortForAddress(frame->getSrc()))==-1)){
            LT->updateTableWithAddress(inputport,frame->getSrc());
            forwaringState++;
        }
        int outport = LT->getPortForAddress(frame->getDest());
        LT->updateTableWithAddress(outport,frame->getDest());
        send(frame, "ifOut", outport);


    }

}

void MACRelayUnitAPB::handleIndependentPath(EtherFrame *frame, int inputport){
    if ((frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
        if(((BT->getPortForAddress(frame->getSrc()))==-1)||(((BT->getPortForAddress(frame->getSrc()))!=-1)&&((BT->getPortForAddress(frame->getSrc()))==inputport))){
            if((LT->getPortForAddress(frame->getSrc()))==-1)
                LT->updateTableWithAddress(inputport,frame->getSrc());
            BT->updateTableWithAddress(inputport,frame->getSrc());
    //EV<<"BT update port : #"<<inputport<<"  src address:"<<frame->getSrc();
                       // EV << "Broadcasting broadcast frame " << frame << endl;
                        broadcastFrame(frame, inputport);
                    }else{
                        //EV << "Frame is received before, " << frame->getFullName()
                              //     << " dest " << frame->getDest() << ", discarding frame\n";
                        //numDiscardedFrames++;
                        delete frame;
                    }
                else if (!(frame->getDest().isBroadcast())||(frame->getDest().isMulticast())){
                        if((strcmp(frame->getName(),"arpREPLY")==0)&&((LT->getPortForAddress(frame->getSrc()))==-1)){
                            LT->updateTableWithAddress(inputport,frame->getSrc());
                            forwaringState++;
                            }
                        int outport = LT->getPortForAddress(frame->getDest());
                        LT->updateTableWithAddress(outport,frame->getDest());
                        send(frame, "ifOut", outport);


                }


}

void MACRelayUnitAPB::handleUsingL3inL2Path(EtherFrame *frame, int inputport){

    if ((frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
       if(((BT2->getPortForAddress(frame->getSrc()))==-1)||(((BT2->getPortForAddress(frame->getSrc()))!=-1)&&((BT2->getPortForAddress(frame->getSrc()))==inputport)))
       {
           if((strcmp(frame->getName(),"arpREQ")==0)&&((LT2->getPortForAddress(frame->getSrc()))==-1))
                LT2->updateTableWithAddress(inputport,frame->getSrc(),getSourceIPAddress(frame));
            BT2->updateTableWithAddress(inputport,frame->getSrc(),getSourceIPAddress(frame));


            //EV<<"BT update port : #"<<inputport<<"  src address:"<<frame->getSrc();
           // EV << "Broadcasting broadcast frame " << frame << endl;
            int outport1 = LT2->getPortForAddress(getDestinationIPAddress(frame));
            if (outport1 != -1)
            {
                numARP_PathUnicastFrames ++;
                send(frame, "ifOut", outport1);
            }
           else
           {
               broadcastFrame(frame, inputport);
           }
        }else{
            //EV << "Frame is received before, " << frame->getFullName()
                  //     << " dest " << frame->getDest() << ", discarding frame\n";
            //numDiscardedFrames++;
            delete frame;
        }
    else if (!(frame->getDest().isBroadcast())||(frame->getDest().isMulticast())){
        if((strcmp(frame->getName(),"arpREPLY")==0)&&((LT2->getPortForAddress(frame->getSrc()))==-1)){
            LT2->updateTableWithAddress(inputport,frame->getSrc(),getSourceIPAddress(frame));
            forwaringState++;
        }
        int outport = LT2->getPortForAddress(frame->getDest());
        LT2->updateTableWithAddress(outport,frame->getDest(),getDestinationIPAddress(frame));
        send(frame, "ifOut", outport);


    }


}


void MACRelayUnitAPB::handleOurFlowPath(EtherFrame *frame, int inputport)
{
    if ((frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
       if(((BT3->getPortForAddress(frame->getSrc(),getDestinationIPAddress(frame)))==-1)||(((BT3->getPortForAddress(frame->getSrc(),getDestinationIPAddress(frame)))!=-1)&&((BT3->getPortForAddress(frame->getSrc(),getDestinationIPAddress(frame)))==inputport)))
       {
           if((strcmp(frame->getName(),"arpREQ")==0)&&((LT3->getPortForAddress(frame->getSrc(),getDestinationIPAddress(frame)))==-1))
                LT3->updateTableWithAddress(inputport,frame->getSrc(),getDestinationIPAddress(frame));
           BT3->updateTableWithAddress(inputport,frame->getSrc(),getDestinationIPAddress(frame));


           //EV<<"BT update port : #"<<inputport<<"  src address:"<<frame->getSrc();
           // EV << "Broadcasting broadcast frame " << frame << endl;
           broadcastFrame(frame, inputport);
        }else{
            //EV << "Frame is received before, " << frame->getFullName()
                  //     << " dest " << frame->getDest() << ", discarding frame\n";
            //numDiscardedFrames++;
            delete frame;
        }
    else if (!(frame->getDest().isBroadcast())||(frame->getDest().isMulticast())){
        if((strcmp(frame->getName(),"arpREPLY")==0)&&((LT3->getPortForAddress(frame->getSrc(),getDestinationIPAddress(frame)))==-1)){
            LT3->updateTableWithAddress(inputport,frame->getSrc(),getDestinationIPAddress(frame));
            forwaringState++;
        }
        int outport = LT3->getPortForAddress(frame->getDest(),getSourceIPAddress(frame));
        LT3->updateTableWithAddress(outport,frame->getDest(),getSourceIPAddress(frame));
        send(frame, "ifOut", outport);


    }

}

void MACRelayUnitAPB::handleMulticastPath(EtherFrame *frame, int inputport){

    if ((frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
        if(((BT4->getPortForAddress(frame->getSrc()))==-1)||(((BT4->getPortForAddress(frame->getSrc()))!=-1)&&((BT4->getPortForAddress(frame->getSrc()))==inputport)))
        {
            if((strcmp(frame->getName(),"arpREQ")==0)&&((LT4->getPortForAddress(frame->getSrc()))==-1))
                LT4->updateTableWithAddress(inputport,frame->getSrc());
            BT4->updateTableWithAddress(inputport,frame->getSrc());
            //EV<<"BT4 update port : #"<<inputport<<"  src address:"<<frame->getSrc();
            // EV << "Broadcasting broadcast frame " << frame << endl;
            arpPathMulticastFrame(frame, inputport);
        }else
        {
            //EV << "Frame is received before, " << frame->getFullName()
            //     << " dest " << frame->getDest() << ", discarding frame\n";
            // numDiscardedFrames++;
            LT4->updateLateListWithAddress(inputport, frame->getSrc());
            delete frame;
        }else if (!(frame->getDest().isBroadcast())||(frame->getDest().isMulticast()))
        {
            if((strcmp(frame->getName(),"arpREPLY")==0)&&((LT4->getPortForAddress(frame->getSrc()))==-1))
            {
                LT4->updateTableWithAddress(inputport,frame->getSrc());
                forwaringState++;
            }
            int outport = LT4->getPortForAddress(frame->getDest());
            LT4->updateTableWithAddress(outport,frame->getDest());
            send(frame, "ifOut", outport);
        }

}


void MACRelayUnitAPB::broadcastFrame(EtherFrame *frame, int inputport)
{
    for (int i = 0; i < numPorts; ++i) {
        if (i != inputport) {
            emit(LayeredProtocolBase::packetSentToLowerSignal, frame);
            numARP_PathBroadcastFrames++;
            send((EtherFrame *)frame->dup(), "ifOut", i);
        }
    }

    delete frame;
}

void MACRelayUnitAPB::arpPathMulticastFrame(EtherFrame *frame, int inputport)
{
    uint64 lateList = LT4->getLateListForAddress(frame->getSrc());

    for (int i = 0; i < numPorts; ++i) {

        uint64 flag = 1;
        flag = flag << (i);    //2^latePort, pow(2, latePort)

        if ((i != inputport) && !(lateList & flag)) {
            emit(LayeredProtocolBase::packetSentToLowerSignal, frame);
            numARP_PathMulticastFrames++;
            send((EtherFrame *)frame->dup(), "ifOut", i);
        }
    }

    delete frame;
}


void MACRelayUnitAPB::start()
{
    addressTable->clearTable();
    // our change
    //BT->clearTable();

    isOperational = true;
}

void MACRelayUnitAPB::stop()
{
    addressTable->clearTable();
    isOperational = false;
}

bool MACRelayUnitAPB::handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback)
{
    Enter_Method_Silent();

    if (dynamic_cast<NodeStartOperation *>(operation)) {
        if ((NodeStartOperation::Stage)stage == NodeStartOperation::STAGE_LINK_LAYER) {
            start();
        }
    }
    else if (dynamic_cast<NodeShutdownOperation *>(operation)) {
        if ((NodeShutdownOperation::Stage)stage == NodeShutdownOperation::STAGE_LINK_LAYER) {
            stop();
            EV<<"BEEEEEEEEEEEEP";
        }
    }
    else if (dynamic_cast<NodeCrashOperation *>(operation)) {
        if ((NodeCrashOperation::Stage)stage == NodeCrashOperation::STAGE_CRASH) {
            stop();
        }
    }
    else {
        throw cRuntimeError("Unsupported operation '%s'", operation->getClassName());
    }

    return true;
}

void MACRelayUnitAPB::finish()
{
    recordScalar("processed frames", numProcessedFrames);
    recordScalar("discarded frames", numDiscardedFrames);
   /* if(isArpPath==false)
            {

                forwaringState=addressTable->printState();
              //  cout<<"forwaringState "<<forwaringState<<endl;
            }*/
    EV << endl << "           Switch parameters " << endl;
    EV << "ARP Broadcast    ARP Multicast    ARP Unicast" << endl;
    EV << numARP_PathBroadcastFrames << "          " << numARP_PathMulticastFrames << "          " << numARP_PathUnicastFrames << endl;


}
//our change
void MACRelayUnitAPB::receiveSignal(cComponent *src, simsignal_t id, cObject *value,cObject *details)
{
    if (dynamic_cast<cPreGateDisconnectNotification *>(value))
    {
        cPreGateDisconnectNotification *data = (cPreGateDisconnectNotification *)value;


        if (strcmp(src->getName(),this->getParentModule()->getName())==0)
        {
            //gate(data->gate->getIndex())->disconnect();
            if (LT->isPortintable(data->gate->getIndex()))
            {
                Enter_Method("Receive Signal");
                int numMACAddresses = LT->getnumbersOfAddressesAssociated(data->gate->getIndex());
                MACAddress *macAddresses = LT->getAddressesAssociatedToPort(data->gate->getIndex());
                LinkFailPkt *LinkFailpkt = new LinkFailPkt("LinkFail");

                LinkFailpkt->setAddressesArraySize(numMACAddresses);
                for(int i=0;i<numMACAddresses;i++)
                {
                    LinkFailpkt->setAddresses(i,macAddresses[i]);
                    LT->removeMACAddressFromVlan(macAddresses[i]);

                }
                delete[] macAddresses;
                EtherFrame *LinkFail = new EtherFrame("LinkFail");
                LinkFail->setDest(MACAddress::BROADCAST_ADDRESS);
                LinkFail->setSrc(getPortMACAddress(data->gate->getIndex()));
                LinkFail->encapsulate(LinkFailpkt);
                if (LinkFail -> getByteLength()<MIN_ETHERNET_FRAME_BYTES)
                        LinkFail->addByteLength(MIN_ETHERNET_FRAME_BYTES);
                broadcastFrame(LinkFail,data->gate->getIndex());
            }
        }
    }
}

MACAddress MACRelayUnitAPB::getPortMACAddress(int portnumber)
{
   // cGate *outGate = gate("ifOut",1);
    //gate("ifOut",portnumber)->
    return check_and_cast<EtherMACFullDuplexNew *>(this->getParentModule()->getSubmodule("eth",portnumber)->getSubmodule("mac"))->getAddress();
}

bool MACRelayUnitAPB::isEncapsulatedMacDirectlyConnected(EtherFrame *frame)
{
    if(isEdgeSwitch)//if (strcmp(frame->getName(),"LinkFail")==0) //this condition is checked in main bode according to compiler behavior
    {
        LinkFailPkt *pkt=check_and_cast<LinkFailPkt *>(frame->decapsulate());
        frame->encapsulate(pkt);
        int numMACAddresses =pkt->getAddressesArraySize();
        if (numMACAddresses>0)
        {
            int neighbourIndex = -1;
            MACAddress neighbourMac;
            for(int i=0;i<numPorts;i++)
            {
                neighbourIndex = this->getParentModule()->gate("ethg$o",i)->getNextGate()->getIndex();
                neighbourMac = check_and_cast<EtherMACFullDuplexNew *>(this->getParentModule()->gate("ethg$o",i)->getNextGate()->getOwnerModule()->getSubmodule("eth",neighbourIndex)->getSubmodule("mac"))->getAddress();
                for(int j=0;j<numMACAddresses;j++)
                {
                    if(pkt->getAddresses(j)==neighbourMac)
                        return true;
                 }
            }
       }


    }
    return false;
}

void MACRelayUnitAPB::sendToEncapsulatedMacDirectlyConnected(EtherFrame *frame,int inputport)
{
    if(isEdgeSwitch)    // if (strcmp(frame->getName(),"LinkFail")==0) //this condition is checked in main bode
    {
        LinkFailPkt *pkt=check_and_cast<LinkFailPkt *>(frame->decapsulate());
        frame->encapsulate(pkt);
        int numMACAddresses =pkt->getAddressesArraySize();
        if (numMACAddresses>0)
        {
            int neighbourIndex = -1;
            MACAddress neighbourMac;
            for(int i=0;i<numPorts;i++)
            {
                neighbourIndex = this->getParentModule()->gate("ethg$o",i)->getNextGate()->getIndex();
                neighbourMac = check_and_cast<EtherMACFullDuplexNew *>(this->getParentModule()->gate("ethg$o",i)->getNextGate()->getOwnerModule()->getSubmodule("eth",neighbourIndex)->getSubmodule("mac"))->getAddress();
                for(int j=0;j<numMACAddresses;j++)
                {
                    if(pkt->getAddresses(j)==neighbourMac)
                    {
                        EtherFrame *LinkReply = new EtherFrame("LinkReply");
                        LinkReply->setSrc(neighbourMac);
                        LinkReply->setDest(frame->getSrc());
                        if (LinkReply -> getByteLength()<MIN_ETHERNET_FRAME_BYTES)
                            LinkReply->addByteLength(MIN_ETHERNET_FRAME_BYTES);
                        send(LinkReply, "ifOut", inputport);
                     }
                }
             }



        }

    }

}

IPv4Address MACRelayUnitAPB::getSourceIPAddress(EtherFrame *frame)
{
    cPacket *pkt = check_and_cast <cPacket *>(frame->decapsulate());
    frame->encapsulate(pkt);
    IPv4Address Adrs;
    if (dynamic_cast<ARPPacket *>(pkt))
        Adrs = check_and_cast<ARPPacket *>(pkt)->getSrcIPAddress();
    else if (dynamic_cast<IPv4Datagram *>(pkt))
        Adrs = check_and_cast<IPv4Datagram *>(pkt)->getSrcAddress();
    return Adrs;
}

IPv4Address MACRelayUnitAPB::getDestinationIPAddress(EtherFrame *frame)
{
    cPacket *pkt = check_and_cast <cPacket *>(frame->decapsulate());
    frame->encapsulate(pkt);
    IPv4Address Adrs;
    if (dynamic_cast<ARPPacket *>(pkt))
        Adrs = check_and_cast<ARPPacket *>(pkt)->getDestIPAddress();
    else if (dynamic_cast<IPv4Datagram *>(pkt))
        Adrs = check_and_cast<IPv4Datagram *>(pkt)->getDestAddress();
    return Adrs;
}


} // namespace inet

