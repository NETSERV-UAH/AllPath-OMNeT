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


using namespace inet;
using namespace inet::ieee80211;

namespace wapb {

namespace ieee80211 {

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

    MACAddress next_hop;
    Ieee80211DataFrame *frame = encapsulate(msg);
    EtherFrame *frame_eth=convertToEtherFrame(frame->dup());
    MACRelayUnitWAPB *pRelayUnitWAPB = check_and_cast<MACRelayUnitWAPB *>(this->getParentModule()->getParentModule()->getSubmodule("relayUnit"));
    next_hop=pRelayUnitWAPB->handleAndDispatchFrameV2Route(frame_eth);
    frame->setReceiverAddress(next_hop);
    sendDown(frame);

}

void Ieee80211MgmtAdhocAPB::handleCommand(int msgkind, cObject *ctrl)
{
    throw cRuntimeError("handleCommand(): no commands supported");
}

Ieee80211DataFrame *Ieee80211MgmtAdhocAPB::encapsulate(cPacket *msg)
{
    Ieee80211DataFrameWithSNAP *frame = new Ieee80211DataFrameWithSNAP(msg->getName());

    // copy receiver address from the control info (sender address will be set in MAC)
    Ieee802Ctrl *ctrl = check_and_cast<Ieee802Ctrl *>(msg->removeControlInfo());
    frame->setAddress3(ctrl->getSrc());
    frame->setAddress4(ctrl->getDest());
    //frame->setReceiverAddress(ctrl->getDest());
    frame->setEtherType(ctrl->getEtherType());
    int up = ctrl->getUserPriority();
    if (up >= 0) {
        // make it a QoS frame, and set TID
        frame->setType(ST_DATA_WITH_QOS);
        frame->addBitLength(QOSCONTROL_BITS);
        frame->setTid(up);
    }
    delete ctrl;

    frame->encapsulate(msg);
    return frame;
}

EtherFrame *Ieee80211MgmtAdhocAPB::convertToEtherFrame(Ieee80211DataFrame *frame_)
{
    Ieee80211DataFrameWithSNAP *frame = check_and_cast<Ieee80211DataFrameWithSNAP *>(frame_);

    // create a matching ethernet frame
    EthernetIIFrame *ethframe = new EthernetIIFrame(frame->getName()); //TODO option to use EtherFrameWithSNAP instead
    ethframe->setDest(frame->getAddress4());
    ethframe->setSrc(frame->getAddress3());
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
    ctrl->setSrc(frame->getAddress3());
    ctrl->setDest(frame->getAddress4()); //modified for ARP-PATH, original is : frame->getReceiverAddress()
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
    if(frame->getAddress3()==myAddress)
    {
        EV << "  Delete packect, src address is my own address " << frame->getAddress3() << endl;
        delete (frame);
    }
    else
    {
      //Realizamos el aprendizaje ya que los paquetes llegan desde abajo
       MACAddress previous_MAC=frame->getTransmitterAddress();

       //EV << "  Original ARP-Resolution ADDRESS: " << orig_ARPResolution << endl;

       EV << "  Previous ADDRESS: " << previous_MAC << endl;


       if((!strcmp("arpREQ",frame->getFullName())) || (!strcmp("arpREPLY",frame->getFullName())))
          {
           if(previous_resolution_address==frame->getAddress3())
           {
               EV << "It is the previous ARP Resolution, is not necessary forwarding this frame" << endl;
               delete (frame);
           }
           else
           {
               previous_resolution_address=frame->getAddress3(); //estaba en 4
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
               sendUp(decapsulate(frame));
           }
           //}
          }
       else if((frame->getAddress4())==myAddress)
       {
           sendUp(decapsulate(frame));
       }
       else
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

