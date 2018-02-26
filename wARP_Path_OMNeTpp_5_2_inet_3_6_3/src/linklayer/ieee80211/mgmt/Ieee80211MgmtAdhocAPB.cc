//
// Copyright (C) 2006 Andras Varga
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

#include "src/linklayer/ieee80211/mgmt/Ieee80211MgmtAdhocAPB.h"
#include "src/linklayer/ethernet/ARPPathSwitch/MACRelayUnitWAPB.h"
#include "inet/linklayer/common/Ieee802Ctrl.h"
#include "inet/linklayer/ethernet/EtherFrame.h"

namespace wapb {
namespace ieee80211 {

using namespace inet;
using namespace inet::ieee80211;

Define_Module(Ieee80211MgmtAdhocAPB);

void Ieee80211MgmtAdhocAPB::initialize(int stage)
{
    Ieee80211MgmtBase::initialize(stage);
    //EXTRA
    previous_resolution_address = previous_resolution_address.UNSPECIFIED_ADDRESS;
    //NEW
    implementation = par("implementation").stringValue();
    jitterPar = &par("jitter");
    maxJitter = par("maxJitter").doubleValue();
    isSlottedJitter = par("isSlottedJitter").boolValue();
}

void Ieee80211MgmtAdhocAPB::handleTimer(cMessage *msg)
{
    ASSERT(false);
}

void Ieee80211MgmtAdhocAPB::handleUpperMessage(cPacket *msg)
{
    //Ieee80211DataFrame *frame = encapsulate(msg);
    //sendDown(frame);

    EV << "->Ieee80211MgmtAdhocAPB::handleUpperMessage()" << endl;
    if (strcmp(implementation,"new") == 0)
        handleUpperMessageNewWAPB(msg);
    else if (strcmp(implementation,"old") == 0)
        handleUpperMessageOldWAPB(msg);
    EV << "<- Ieee80211MgmtAdhocAPB::handleUpperMessage()" << endl;
}

void Ieee80211MgmtAdhocAPB::handleUpperMessageNewWAPB(cPacket *msg)
{
    MACAddress nextHop;
    inet::ieee80211::Ieee80211DataFrame *frame = encapsulate(msg);

    EV << "Ieee80211DataFrame received from upper layer, frame.ReceiverAddress = " << frame->getReceiverAddress() << ", TransmitterAddress = " << frame->getTransmitterAddress() << ", frame.Address3 = " << frame->getAddress3() << ", frame.Address4 = "  << frame->getAddress4() << endl;
 /*   if (frame->getAddress3() == MACAddress::BROADCAST_ADDRESS)
    {
        nextHop = MACAddress::BROADCAST_ADDRESS;
    }else
    {*/
        EtherFrame *frame_eth=convertToEtherFrame(frame->dup());
        EV << "EtherFrame is Generated to send to macRelay Unit, frame.src = " << frame_eth->getSrc() << "frame.dest = " << frame_eth->getDest() << endl;
        MACRelayUnitWAPB *pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getParentModule()->getSubmodule("relayUnit"));
        nextHop=pRelayUnitWAPB->handleAndDispatchV2WArpPath(frame_eth,MACAddress::UNSPECIFIED_ADDRESS);
        EV << "Next hop received from macRelay = " << nextHop << endl;
 /*   }*/

    frame->setReceiverAddress(nextHop);
    EV << "Next hop address (" << frame->getReceiverAddress() <<  ") is added to the field of ReceiverAddress" << endl;
    EV << "Ieee80211DataFrame completed to send to lower layer." << endl;
    EV << "ReceiverAddress (Physical Receiver) = " << frame->getReceiverAddress() << ", TransmitterAddress (Physical Transmitter) = " << frame->getTransmitterAddress() << ", frame.Address3 (Logical Receiver) = " << frame->getAddress3() << ", frame.Address4 (Logical Transmitter) = "  << frame->getAddress4() << endl;
    sendDown(frame);
}

void Ieee80211MgmtAdhocAPB::handleUpperMessageOldWAPB(cPacket *msg)
{
    MACAddress nextHop;
    inet::ieee80211::Ieee80211DataFrame *frame = encapsulate(msg);

    EV << "Ieee80211DataFrame received from upper layer, frame.ReceiverAddress = " << frame->getReceiverAddress() << ", TransmitterAddress = " << frame->getTransmitterAddress() << ", frame.Address3 = " << frame->getAddress3() << ", frame.Address4 = "  << frame->getAddress4() << endl;
    EtherFrame *frame_eth=convertToEtherFrame(frame->dup());
    EV << "EtherFrame is Generated to send to macRelay Unit, frame.src = " << frame_eth->getSrc() << "frame.dest = " << frame_eth->getDest() << endl;
    MACRelayUnitWAPB *pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getParentModule()->getSubmodule("relayUnit"));
    nextHop=pRelayUnitWAPB->handleAndDispatchFrameV2Route(frame_eth);
    EV << "Next hop received from macRelay = " << nextHop << endl;

    frame->setReceiverAddress(nextHop);
    EV << "Next hop address (" << frame->getReceiverAddress() <<  ") is added to the field of ReceiverAddress" << endl;
    EV << "Ieee80211DataFrame completed to send to lower layer." << endl;
    EV << "ReceiverAddress (Physical Receiver) = " << frame->getReceiverAddress() << ", TransmitterAddress (Physical Transmitter) = " << frame->getTransmitterAddress() << ", frame.Address3 (Logical Receiver) = " << frame->getAddress3() << ", frame.Address4 (Logical Transmitter) = "  << frame->getAddress4() << endl;
    sendDown(frame);
}

void Ieee80211MgmtAdhocAPB::handleCommand(int msgkind, cObject *ctrl)
{
    throw cRuntimeError("handleCommand(): no commands supported");
}

Ieee80211DataFrame *Ieee80211MgmtAdhocAPB::encapsulate(cPacket *msg)
{
    EV << "-> Ieee80211MgmtAdhocAPB::encapsulate()" << endl;

    Ieee80211DataFrameWithSNAP *frame = new Ieee80211DataFrameWithSNAP(msg->getName());

    // copy receiver address from the control info (sender address will be set in MAC)
    Ieee802Ctrl *ctrl = check_and_cast<Ieee802Ctrl *>(msg->removeControlInfo());
    frame->setAddress3(ctrl->getDest());  // Address3 is the address of the logical receiver (dst of the explored path )
    frame->setAddress4(myAddress);//ctrl->getSrc());  // Address3 is the address of the logical transmitter (src of the explored path )
    frame->setTransmitterAddress(myAddress);//ctrl->getSrc());  // TransmitterAddress is the address of the physical Transmitter (address of this nic)
    //frame->setReceiverAddress(ctrl->getDest()); // ReceiverAddress is address of the physical receiver (address of the next hop)
    frame->setEtherType(ctrl->getEtherType());
    EV << "Physical Transmitter address is added to field TransmitterAddress(" << frame->getTransmitterAddress() << "), Logical Receiver address is added to the field of Address3 (" << frame->getAddress3() << ") , Logical Transmitter address is added to the field of Address4 (" << frame->getAddress4() << endl;
    EV << "Frame Type : " << frame->getEtherType() << endl;


    int up = ctrl->getUserPriority();
    if (up >= 0) {
        // make it a QoS frame, and set TID
        frame->setType(ST_DATA_WITH_QOS);
        frame->addBitLength(QOSCONTROL_BITS);
        frame->setTid(up);
    }
    delete ctrl;

    frame->encapsulate(msg);
    EV << "<- Ieee80211MgmtAdhocAPB::encapsulate()" << endl;

    return frame;
}

EtherFrame *Ieee80211MgmtAdhocAPB::convertToEtherFrame(Ieee80211DataFrame *frame_)
{
    Ieee80211DataFrameWithSNAP *frame = check_and_cast<Ieee80211DataFrameWithSNAP *>(frame_);

    // create a matching ethernet frame
    EthernetIIFrame *ethframe = new EthernetIIFrame(frame->getName()); //TODO option to use EtherFrameWithSNAP instead
    ethframe->setDest(frame->getAddress3());  // Address3 is address of logical receiver (dst of the explored path )
    ethframe->setSrc(frame->getAddress4());   // Address4 is address of logical transmitter (src of the explored path )
    ethframe->setEtherType(frame->getEtherType());

    // encapsulate the payload in there
    cPacket *payload = frame->decapsulate();
    delete frame;
    ASSERT(payload!=NULL);
    ethframe->encapsulate(payload);
    if (ethframe->getByteLength() < MIN_ETHERNET_FRAME_BYTES)
        ethframe->setByteLength(MIN_ETHERNET_FRAME_BYTES);

    // done
    return ethframe;
}

cPacket *Ieee80211MgmtAdhocAPB::decapsulate(Ieee80211DataFrame *frame)
{
    cPacket *payload = frame->decapsulate();

    Ieee802Ctrl *ctrl = new Ieee802Ctrl();
    ctrl->setSrc(frame->getAddress4());
    ctrl->setDest(frame->getAddress3()); //modified for ARP-PATH, original is : frame->getReceiverAddress()
    //ctrl->setSrc(frame->getTransmitterAddress());
    //ctrl->setDest(frame->getReceiverAddress());
    int tid = frame->getTid();
    if (tid < 8)
        ctrl->setUserPriority(tid); // TID values 0..7 are UP
    Ieee80211DataFrameWithSNAP *frameWithSNAP = dynamic_cast<Ieee80211DataFrameWithSNAP *>(frame);
    if (frameWithSNAP)
        ctrl->setEtherType(frameWithSNAP->getEtherType());
    payload->setControlInfo(ctrl);

    delete frame;
    return payload;
}

void Ieee80211MgmtAdhocAPB::handleDataFrame(Ieee80211DataFrame *frame)
{
    //sendUp(decapsulate(frame));
    EV << "->Ieee80211MgmtAdhocAPB::handleDataFrame()" << endl;

    if (strcmp(implementation,"new") == 0)
        handleDataFrameNewVersionWAPB(frame);
    else if (strcmp(implementation,"old") == 0)
        handleDataFrameOldVersionWAPB(frame);

    EV << "<-Ieee80211MgmtAdhocAPB::handleDataFrame()" << endl;
}

void Ieee80211MgmtAdhocAPB::handleDataFrameNewVersionWAPB(Ieee80211DataFrame *frame)
{
    EV << "Ieee80211DataFrame received from lower layer, frame.ReceiverAddress = " << frame->getReceiverAddress() << ", TransmitterAddress = " << frame->getTransmitterAddress() << ", frame.Address3 = " << frame->getAddress3() << ", frame.Address4 = "  << frame->getAddress4() << endl;

    //Realizamos el aprendizaje ya que los paquetes llegan desde abajo
    MACAddress previousHop=frame->getTransmitterAddress();
    EV << "  Previous Hop ADDRESS: " << previousHop << endl;

    if(frame->getAddress4()==myAddress)  //Line 1 pseudo-code (listing 1) // if i am Logical Transmitter!, my frame return back to me!
    {
        EV << "  Delete packect, src address (Logical Transmitter address) is my own address (Step 1 pseudo-code), frame is discarded. " << frame->getAddress3() << endl;
        delete (frame);
    }
    else
    {
        //Line 4 pseudo-code (listing 1)
        EtherFrame *frame_eth=convertToEtherFrame(frame->dup());
        MACRelayUnitWAPB *pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getParentModule()->getSubmodule("relayUnit"));
        MACAddress nextHop = pRelayUnitWAPB->handleAndDispatchV2WArpPath(frame_eth,previousHop);

        //Line 6 pseudo-code (listing 1)
        if ((frame->getAddress3())==myAddress)
        {
            sendUp(decapsulate(frame->dup()));
            EV << frame->getName() << "frame is mine and is sent to upper layer. next hop is : " << nextHop << endl;
        }else if ((frame->getAddress3().isBroadcast()) && (!nextHop.isUnspecified()))// second conditional expression for preventing extra process on upper layer. for example if another copy of ARP Request received, This frame does not need to be processed again on its higher layer   // TODO check frame->getReceiverAddress().isBroadcast(), note that this address field is used for hop to hop steps
        {
            sendUp(decapsulate(frame->dup()));
            EV << frame->getName() << "frame is broadcast and a copy of frame is sent to upper layer. next hop is : " << nextHop << endl;
        } //else is not needed because if frame is a broadcast frame, it must be retransmitted to other hops

            //Line 10 pseudo-code (listing 1)
        if (!nextHop.isUnspecified())//(nextHop != MACAddress::UNSPECIFIED_ADDRESS)
        {
            frame->setReceiverAddress(nextHop);
            frame->setTransmitterAddress(myAddress);
            EV << "frame is sent to next hop:" << nextHop << endl;
            EV << "ReceiverAddress (Physical Receiver): " << frame->getReceiverAddress() << "TransmitterAddress (Physical Transmitte): " << frame->getTransmitterAddress() << ", Address3 (Logical Receiver) :" << frame->getAddress3() << ", Address4 (Logical Transmitter)" << frame->getAddress4() << endl;
            //sendDown(frame);

            //using jitter instead of direct senddown()
            if ((strcmp("arpREQ",frame->getName())==0) || (strcmp("arpREPLY",frame->getName()))==0)
                if (!isSlottedJitter)
                {
                    sendDownDelayed(frame, jitterPar->doubleValue());
                    EV << frame->getName() << " Jitter is used for transmiting this frame " << frame->getName() << ", maxJitter = " << maxJitter << ", jitter value = " << jitterPar->doubleValue() << endl;
                }else    //slotted jitter:
                {
                    sendDownSlottedDelayed(frame, maxJitter);
                    EV << frame->getName() << " Slotted Jitter is used for transmiting this frame " << frame->getName() << ", maxJitter = " << maxJitter << endl<< endl;
                }
            else
                sendDown(frame);
        }else
        {
            EV << frame->getName() << " frame has not next hop and is deleted. next hop is : " << nextHop << endl;
            delete frame;
        }

    }


/*    EV << "Ieee80211DataFrame received from lower layer, frame.ReceiverAddress = " << frame->getReceiverAddress() << ", TransmitterAddress = " << frame->getTransmitterAddress() << ", frame.Address3 = " << frame->getAddress3() << ", frame.Address4 = "  << frame->getAddress4() << endl;

    //Realizamos el aprendizaje ya que los paquetes llegan desde abajo
    MACAddress previousHop=frame->getTransmitterAddress();
    EV << "  Previous Hop ADDRESS: " << previousHop << endl;

    if(frame->getAddress4()==myAddress)  //Step 1 pseudo-code (listing 2) // if i am Logical Transmitter!, my frame return back to me!
    {
        EV << "  Delete packect, src address (Logical Transmitter address) is my own address (Step 1 pseudo-code), frame is discarded. " << frame->getAddress3() << endl;
        delete (frame);
    }
    else
    {
        //Step2 pseudo-code (listing 2)
        EtherFrame *frame_eth=convertToEtherFrame(frame->dup());
        MACRelayUnitWAPB *pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getParentModule()->getSubmodule("relayUnit"));
        MACAddress nextHop = pRelayUnitWAPB->handleAndDispatchV2WArpPath(frame_eth,previousHop); //previous hop must be learned, next hop is not important

        //Step3 pseudo-code (listing 2)
        if (((frame->getAddress3())==myAddress) || (frame->getAddress3().isBroadcast()))   // TODO check frame->getReceiverAddress().isBroadcast(), note that this address field is used for hop to hop steps
        {
            sendUp(decapsulate(frame->dup()));
            EV << frame->getName() << "frame is sent to upper layer. next hop is : " << nextHop << endl;
        } //else is not needed because if frame is a broadcast frame, it must be retransmitted to other hops
            //Step4 pseudo-code (listing 2)
        if (!nextHop.isUnspecified())//(nextHop != MACAddress::UNSPECIFIED_ADDRESS)
        {
            frame->setReceiverAddress(nextHop);
            frame->setTransmitterAddress(myAddress);
            EV << "frame is sent to next hop:" << nextHop << endl;
            EV << "ReceiverAddress (Physical Receiver): " << frame->getReceiverAddress() << "TransmitterAddress (Physical Transmitte): " << frame->getTransmitterAddress() << ", Address3 (Logical Receiver) :" << frame->getAddress3() << ", Address4 (Logical Transmitter)" << frame->getAddress4() << endl;
            sendDown(frame);
        }else
        {
            EV << frame->getName() << " frame has not next hop and is deleted. next hop is : " << nextHop << endl;
            delete frame;
        }

    }
*/


    /*
    EV << "Ieee80211DataFrame received from lower layer, frame.ReceiverAddress = " << frame->getReceiverAddress() << ", TransmitterAddress = " << frame->getTransmitterAddress() << ", frame.Address3 = " << frame->getAddress3() << ", frame.Address4 = "  << frame->getAddress4() << endl;

    //Realizamos el aprendizaje ya que los paquetes llegan desde abajo
    MACAddress previousHop=frame->getTransmitterAddress();
    EV << "  Previous Hop ADDRESS: " << previousHop << endl;

    if(frame->getAddress4()==myAddress)  //Step 1 pseudo-code (listing 2) // if i am Logical Transmitter!, my frame return back to me!
    {
        EV << "  Delete packect, src address (Logical Transmitter address) is my own address (Step 1 pseudo-code), frame is discarded. " << frame->getAddress3() << endl;
        delete (frame);
    }
    else if((frame->getAddress3())==myAddress)   //Step2 pseudo-code (listing 2) (yes:), it is not arpREQ, and it is mine
    {
        EtherFrame *frame_eth=convertToEtherFrame(frame->dup());
        MACRelayUnitWAPB *pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getParentModule()->getSubmodule("relayUnit"));
        MACAddress nextHop = pRelayUnitWAPB->handleAndDispatchV2WArpPath(frame_eth,previousHop); //previous hop must be learned, next hop is not important
        EV << "frame is sent to upper layer" << endl;
        sendUp(decapsulate(frame));
    }
    else   // ARP-Path mechanism
    {
        EtherFrame *frame_eth=convertToEtherFrame(frame->dup());
        MACRelayUnitWAPB *pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getParentModule()->getSubmodule("relayUnit"));
        MACAddress nextHop = pRelayUnitWAPB->handleAndDispatchV2WArpPath(frame_eth,previousHop);
        sendUp(decapsulate(frame->dup())); // to check if ip address is mine. // dup() : maybe frame will be deleted in upper layer, then simulation is encountered with error
        if (!nextHop.isUnspecified())//(nextHop != MACAddress::UNSPECIFIED_ADDRESS)
        {
            frame->setReceiverAddress(nextHop);
            frame->setTransmitterAddress(myAddress);
            EV << "frame is sent to next hop:" << nextHop << endl;
            EV << "ReceiverAddress (Physical Receiver): " << frame->getReceiverAddress() << "TransmitterAddress (Physical Transmitte): " << frame->getTransmitterAddress() << ", Address3 (Logical Receiver) :" << frame->getAddress3() << ", Address4 (Logical Transmitter)" << frame->getAddress4() << endl;
            sendDown(frame);
        }else
            delete frame;
    }
    */
}

void Ieee80211MgmtAdhocAPB::handleDataFrameOldVersionWAPB(Ieee80211DataFrame *frame)
{
    EV << "Ieee80211DataFrame received from lower layer, frame.ReceiverAddress = " << frame->getReceiverAddress() << ", TransmitterAddress = " << frame->getTransmitterAddress() << ", frame.Address3 = " << frame->getAddress3() << ", frame.Address4 = "  << frame->getAddress4() << endl;

    //sendUp(decapsulate(frame));
    if(frame->getAddress4()==myAddress)  //Step 1 pseudo-code (listing 1) // if i am Logical Transmitter!, my frame return back to me!
    {
        EV << "  Delete packect, src address (Logical Transmitter address) is my own address (Step 1 pseudo-code), frame is discarded. " << frame->getAddress3() << endl;
        delete (frame);
    }
    else
    {
      //Realizamos el aprendizaje ya que los paquetes llegan desde abajo
       MACAddress previous_MAC=frame->getTransmitterAddress();

       //EV << "  Original ARP-Resolution ADDRESS: " << orig_ARPResolution << endl;

       EV << "  Previous ADDRESS: " << previous_MAC << endl;


       if((!strcmp("arpREQ",frame->getFullName())) || (!strcmp("arpREPLY",frame->getFullName())))   //Step 2 pseudo-code
       {
           if(previous_resolution_address==frame->getAddress4())       //this part was eliminated because of increasing reliability of unsuccessful broadcasts. in layer 2, broadcast service is an unreliable service
           {
               EV << "It is the previous ARP Resolution, is not necessary forwarding this frame" << endl;
               delete (frame);
           }else
           {
               previous_resolution_address=frame->getAddress4(); //estaba en 4

           //if(orig_ARPResolution == (frame->getAddress3()))
           //{
               //EV << " Is replicated ARP-Resolution, delete frame" << endl;
               //delete frame;
           //}
           //else
           //{
               //orig_ARPResolution = frame->getAddress3();
               //EV << "  Original ARP-Resolution ADDRESS: " << orig_ARPResolution << endl;
               EtherFrame *frame_eth=convertToEtherFrame(frame->dup());
               MACRelayUnitWAPB *pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getParentModule()->getSubmodule("relayUnit"));
               pRelayUnitWAPB->handleAndDispatchFrameV2Learning(frame_eth,previous_MAC);//pasamos la trama ethernet y el siguiente salto al inputport
               //contador para eliminar los ARP-REQs replicados
               sendUp(decapsulate(frame->dup())); // to check if ip address is mine. // dup() : maybe frame will be deleted in upper layer, then simulation is encountered with error

               EtherFrame *frame_eth2 = convertToEtherFrame(frame->dup()); //previous frame_eth is deleted.
               MACAddress nextHop = pRelayUnitWAPB->handleAndDispatchFrameV2Route(frame_eth2);
               if (nextHop != MACAddress::UNSPECIFIED_ADDRESS)
               {
                   frame->setReceiverAddress(nextHop);
                   frame->setTransmitterAddress(myAddress);
                   EV << "frame is sent to next hop:" << nextHop << endl;
                   EV << "ReceiverAddress (Physical Receiver): " << frame->getReceiverAddress() << "TransmitterAddress (Physical Transmitte): " << frame->getTransmitterAddress() << ", Address3 (Logical Receiver) :" << frame->getAddress3() << ", Address4 (Logical Transmitter)" << frame->getAddress4() << endl;
                   sendDown(frame);
               }

           }
           //}
       }else if((frame->getAddress3())==myAddress)   //Step3 pseudo-code (yes:), it is not arpREQ and arpREPLY (it is data), and it is mine
       {
           sendUp(decapsulate(frame));
       }else   //Step 3 (no:)
       {
           EtherFrame * frame_eth=convertToEtherFrame(frame->dup());
           MACAddress next_hop;
           MACRelayUnitWAPB *pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getParentModule()->getSubmodule("relayUnit"));
           next_hop=pRelayUnitWAPB->handleAndDispatchFrameV2Route(frame_eth);
           frame->setReceiverAddress(next_hop);
           sendDown(frame);
       }
    }

}

void Ieee80211MgmtAdhocAPB::sendDownDelayed(cPacket *frame, double delay)
{
    ASSERT(isOperational);
    sendDelayed(frame, delay, "macOut");
}

void Ieee80211MgmtAdhocAPB::sendDownSlottedDelayed(cPacket *frame, double delay)
{
    ASSERT(isOperational);
    double ArpDuration = 0.000448;// in this transmission mode BPSK + DSSS in IEEE802.11g, ARP Request duration = 23 time slots (0.448 ms). each time slots = 0.00002 s;
    int numSlots = delay / ArpDuration; //ceil(delay / (float)ArpDuration);
    int selectedSlot = intrand(numSlots+1);
    double newSlottedDelay = selectedSlot * ArpDuration;
    EV_DETAIL << "Starting slotted jitter: numSlots = " << numSlots << ", selectedSlot = " << selectedSlot << ", newSlottedDelay" << newSlottedDelay << endl;

    sendDelayed(frame, newSlottedDelay, "macOut");
}

void Ieee80211MgmtAdhocAPB::handleAuthenticationFrame(Ieee80211AuthenticationFrame *frame)
{
    dropManagementFrame(frame);
}

void Ieee80211MgmtAdhocAPB::handleDeauthenticationFrame(Ieee80211DeauthenticationFrame *frame)
{
    dropManagementFrame(frame);
}

void Ieee80211MgmtAdhocAPB::handleAssociationRequestFrame(Ieee80211AssociationRequestFrame *frame)
{
    dropManagementFrame(frame);
}

void Ieee80211MgmtAdhocAPB::handleAssociationResponseFrame(Ieee80211AssociationResponseFrame *frame)
{
    dropManagementFrame(frame);
}

void Ieee80211MgmtAdhocAPB::handleReassociationRequestFrame(Ieee80211ReassociationRequestFrame *frame)
{
    dropManagementFrame(frame);
}

void Ieee80211MgmtAdhocAPB::handleReassociationResponseFrame(Ieee80211ReassociationResponseFrame *frame)
{
    dropManagementFrame(frame);
}

void Ieee80211MgmtAdhocAPB::handleDisassociationFrame(Ieee80211DisassociationFrame *frame)
{
    dropManagementFrame(frame);
}

void Ieee80211MgmtAdhocAPB::handleBeaconFrame(Ieee80211BeaconFrame *frame)
{
    dropManagementFrame(frame);
}

void Ieee80211MgmtAdhocAPB::handleProbeRequestFrame(Ieee80211ProbeRequestFrame *frame)
{
    dropManagementFrame(frame);
}

void Ieee80211MgmtAdhocAPB::handleProbeResponseFrame(Ieee80211ProbeResponseFrame *frame)
{
    dropManagementFrame(frame);
}

} // namespace ieee80211

} // namespace wapb

