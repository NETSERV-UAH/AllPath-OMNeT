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
    previous_resolution_address = previous_resolution_address.UNSPECIFIED_ADDRESS;
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
    MACAddress next_hop;
    inet::ieee80211::Ieee80211DataFrame *frame = encapsulate(msg);

    EV << "Ieee80211DataFrame received from upper layer, frame.ReceiverAddress = " << frame->getReceiverAddress() << ", TransmitterAddress = " << frame->getTransmitterAddress() << ", frame.Address3 = " << frame->getAddress3() << ", frame.Address4 = "  << frame->getAddress4() << endl;
    EtherFrame *frame_eth=convertToEtherFrame(frame->dup());
    EV << "EtherFrame is Generated to send to macRelay Unit, frame.src = " << frame_eth->getSrc() << "frame.dest = " << frame_eth->getDest() << endl;
    MACRelayUnitWAPB *pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getParentModule()->getSubmodule("relayUnit"));
    next_hop=pRelayUnitWAPB->handleAndDispatchFrameV2Route(frame_eth);
    EV << "Next hop received from macRelay = " << next_hop << endl;

    frame->setReceiverAddress(next_hop);
    EV << "Next hop address (" << frame->getReceiverAddress() <<  ") is added to the field of ReceiverAddress" << endl;
    EV << "Ieee80211DataFrame completed to send to lower layer." << endl;
    EV << "ReceiverAddress (Physical Receiver) = " << frame->getReceiverAddress() << ", TransmitterAddress (Physical Transmitter) = " << frame->getTransmitterAddress() << ", frame.Address3 (Logical Receiver) = " << frame->getAddress3() << ", frame.Address4 (Logical Transmitter) = "  << frame->getAddress4() << endl;
    sendDown(frame);
    EV << "<- Ieee80211MgmtAdhocAPB::handleUpperMessage()" << endl;

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
    EV << "->Ieee80211MgmtAdhocAPB::handleDataFrame()" << endl;
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
          /* if(previous_resolution_address==frame->getAddress4())       this part was eliminated because of increasing reliability of unsuccessful broadcasts. in layer 2, broadcast service is an unreliable service
           {
               EV << "It is the previous ARP Resolution, is not necessary forwarding this frame" << endl;
               delete (frame);
           }else
           {
               previous_resolution_address=frame->getAddress4(); //estaba en 4
         */
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

       /*    }*/
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

