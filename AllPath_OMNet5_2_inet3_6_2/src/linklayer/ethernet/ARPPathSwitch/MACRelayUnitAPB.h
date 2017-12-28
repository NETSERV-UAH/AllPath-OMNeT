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

#ifndef SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_MACRELAYUNITAPB_H_
#define SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_MACRELAYUNITAPB_H_

#include "inet/common/INETDefs.h"

#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/linklayer/ethernet/switch/IMACAddressTable.h"
#include "inet/linklayer/ethernet/switch/MACAddressTable.h"
#include "./ArpPathMacAddressTable.h"
#include "./ArpPathIPMacAddressTable.h"
#include "ArpPathIPandMacAddressTable.h"
#include "ArpPathMulticastMacAddressTable.h"
#include "inet/networklayer/contract/ipv4/IPv4Address.h"

//#include "../../../mylisteners/MyListener.h"
#include <omnetpp/clistener.h>
using namespace inet;
namespace inet{

class EtherFrame;

}

using inet::EtherFrame;

namespace allpath {



class  MACRelayUnitAPB : public cSimpleModule, public ILifecycle ,omnetpp::cListener
{
  protected:
    IMACAddressTable *addressTable = nullptr;
    int numPorts = 0;
    //our change
    //MACAddressTable *BT = new(MACAddressTable);
    //IMACAddressTable *BT = new(MACAddressTable);
    ArpPathMacAddressTable *BT = nullptr,*LT = nullptr;
    ArpPathIPMacAddressTable *BT2 = nullptr,*LT2 = nullptr;  // for UsingL3inL2
    ArpPathIPandMacAddressTable *BT3 = nullptr,*LT3 = nullptr; // for OurFlowPath
    ArpPathMulticastMacAddressTable *BT4 = nullptr,*LT4 = nullptr; // for Multicast ARP-Path


    bool isArpPath = false;
    bool isRerouteArpPath = false;
    bool isArpPathLinkFail = false;
    bool isIndependentPath = false;
    bool isLowResArpPath = false;
    bool isUsingL3inL2 = false;
    bool isOurFlowPath = false;
    bool isMulticastPath = false;
    bool isEdgeSwitch = false;
    simtime_t learningTableAgingTime = 120;
    simtime_t blockingTableAgingTime = 120;
    //MyListener *listener = nullptr;


    // Parameters for statistics collection
    long numProcessedFrames = 0;
    long numDiscardedFrames = 0;
    long numARP_PathBroadcastFrames = 0;  // our change
    long numARP_PathMulticastFrames = 0;  // our change
    long numARP_PathUnicastFrames = 0;  // our change


    bool isOperational = false;    // for lifecycle
    int  forwaringState=0;

  protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    /**
     * Updates address table with source address, determines output port
     * and sends out (or broadcasts) frame on ports. Includes calls to
     * updateTableWithAddress() and getPortForAddress().
     *
     * The message pointer should not be referenced any more after this call.
     */
    virtual void handleAndDispatchFrame(EtherFrame *frame);

    //Standard ARP-Path
    virtual void handleArpPath(EtherFrame *frame, int inputport);

    //Standard ARP-Path with Link Fail property
    virtual void handleLinkFailArpPath(EtherFrame *frame, int inputport);

    //this is not a new approach because of oscillation.
    virtual void handleReroutableARPPath(EtherFrame *frame, int inputport);

    //Single table ARP-Path
    virtual void handleLowResArpPath(EtherFrame *frame, int inputport);

    //Independent-Path
    virtual void handleIndependentPath(EtherFrame *frame, int inputport);

    //Using L3 addresses beside L2 addresses in ARP-Path as source address
    virtual void handleUsingL3inL2Path(EtherFrame *frame, int inputport);

    //Using L3 addresses beside L2 addresses in ARP-Path as destination address to use lower granularity
    virtual void handleOurFlowPath(EtherFrame *frame, int inputport);

    //this approach accumulates late frames and learns port of late frames and creates a multicast group based on this information.
    virtual void handleMulticastPath(EtherFrame *frame, int inputport);


    /**
     * Utility function: sends the frame on all ports except inputport.
     * The message pointer should not be referenced any more after this call.
     */
    virtual void broadcastFrame(EtherFrame *frame, int inputport);

    //this method is used for multicast approach in handleMulticastPath()
    void arpPathMulticastFrame(EtherFrame *frame, int inputport);

    /**
     * Calls handleIncomingFrame() for frames arrived from outside,
     * and processFrame() for self messages.
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * Writes statistics.
     */
    virtual void finish() override;

    //our change
    virtual void receiveSignal(cComponent *src, simsignal_t id, cObject *value,cObject *details) override;
    virtual MACAddress getPortMACAddress(int portnumber);

    bool isEncapsulatedMacDirectlyConnected(EtherFrame *frame);
    void sendToEncapsulatedMacDirectlyConnected(EtherFrame *frame, int inputport);
    IPv4Address getSourceIPAddress(EtherFrame *frame);
    IPv4Address getDestinationIPAddress(EtherFrame *frame);



    // for lifecycle:

  public:
    virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback) override;

  protected:
    virtual void start();
    virtual void stop();
};

} // namespace inet

#endif // ifndef SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_MACRELAYUNITAPB_H_

