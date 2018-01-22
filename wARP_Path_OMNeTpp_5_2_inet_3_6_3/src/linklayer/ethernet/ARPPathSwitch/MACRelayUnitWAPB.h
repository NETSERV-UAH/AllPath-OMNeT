/*
 * Copyright (C) 2012/2013 Elisa Rojas; GIST, University of Alcala, Spain
 *              based on work of (C) 2010 Diego Rivera (for previous versions of Omnet and the inet framework.
 * MACRelayUnitAPB module defines the ARP-Path switch behaviour (SAu version). It is based on the MACRelayUnitNP module from the Inet Framework.
 * Since the MACRelayUnitAPB couldn't be written as an extension of MACRelayUnitNP (due to the characteristics of it),
 * but as an extended copy of that module, any update of the inet framework MACRelayUnitNP should be made in this module as well.
 * We've tried to keep this module as organized as possible, so that future updates are easy.
 * LAST UPDATE OF THE INET FRAMEWORK: inet2.0 @ 14/09/2012
 *
 * Copyright (C) 2014 Andres Beato: Adaptation MACRelayUnitAPB for wired-networks to MACRelayUnitAPBw for wireless-networks
*/

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

#ifndef WAPB_SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_MACRELAYUNITWAPB_H
#define WAPB_SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_MACRELAYUNITWAPB_H

#include "inet/common/INETDefs.h"

#include "inet/common/lifecycle/ILifecycle.h"
#include "LearningTableNewWAPB.h"
#include "src/linklayer/ethernet/ARPPathSwitch/BlockingTableWAPB.h"
#include "src/linklayer/ethernet/ARPPathSwitch/LearningTableWAPB.h"
#include "src/linklayer/ieee80211/mgmt/Ieee80211MgmtAdhocAPB.h"
#include "src/linklayer/ethernet/ARPPathswitch/LearningTableNewWAPB.h"
#include "src/linklayer/ethernet/ARPPathswitch/BlockingTableNewWAPB.h"
#include "PathRepair_m.h"

/*
#include <omnetpp.h>
#include "inet/networklayer/contract/ipv4/IPv4Address.h"
#include "EtherFrame_m.h"
#include "Ethernet.h"
#include "inet/linklayer/common/MACAddress.h"
//#include "PathRepair_m.h"
#include <sstream>
#include "ARPPacketRoute_m.h"
#include "MACRelayUnitRoute.h"
*/
#define ARPPATH_MCAST_ADD "FE:FF:FF:FF:FF:FF" //TODO: No es una dirección multicast, sino unicast. Lo hemos dejado así porque falla con mcast y bcast (bug de OMNeT++? se llama a una función de EtherEncap y da error...)
#define MAX_EVENTS 10 //Maximum number of events for a link = 10 (this is to save memory, but vectors could be bigger if needed)
#define NPORTS 64 //Number of ports in the switch (for multicast table)

namespace inet{

class EtherFrame;

}

namespace wapb {
using namespace inet;

class MACRelayUnitWAPB : public cSimpleModule, public ILifecycle
{
    /*MACRelayUnitAPB extension from here...*/
    friend class ieee80211::Ieee80211MgmtAdhocAPB;
    //friend class BlockingTableWAPB;

  protected:
    //new implementation
    BlockingTableNewWAPB *BT = nullptr;
    LearningTableNewWAPB *LT = nullptr;
    const char *functionality;
    const char *implementation;


    //old implementation
      unsigned int protocolVersion;   //Two versions of ARP-Path are implemented (2 -unidirectional- and 3 -conditional-, not 1 because it didn't work properly)
      unsigned int repairType;        //Type of repair to be applied (1,2,3: PF+PRq+PRp; PF+PRq; PF+PRp) -- 0:NoRepair (saves events at executing if we will not use any repair method)

      // Configuration of ARP-Path
      simtime_t agingTime; //in LearningTableWAPB defines the "Learning Time" for LT
      simtime_t blockingTime;     //"Locking Time" for BT(Broadcast/Blocking Table) and LT(Lookup/Learning Table). blockingTime is defined in BlockingTableWAPB module
      simtime_t helloTime;        //"Hello" frequency time and "Locking Time" for HeT = helloTime+lockingTime (not used)
      simtime_t repairingTime;    //"Repairing Time" for RT(Repair Table)

      /** LT(Lookup/Learning Table) **/
      enum Status { none = 0, locked = 1, learnt = 2}; //Default: 0,1,2
      LearningTableWAPB *addressTable = nullptr;
      typedef LearningTableWAPB::AddressEntry AddressEntry;

      /** BT(Blocking/Broadcast Table) **/
      BlockingTableWAPB *addressTableBasic = nullptr;
      typedef BlockingTableWAPB::AddressEntryBasic AddressEntryBasic;

      /** MT(Multicast Table) **/
      struct AddressEntryMulticast //Entry of MT - key is multicast group MAC address
      {
          bool port[NPORTS];             // Port that belongs to the multicast group (e.g. up to 64 ports -> 64 bits/flags in real tables)
          simtime_t inTime;              // Creation time of the entry (timer)
      };

      struct MAC_compare
      {
          bool operator()(const MACAddress& u1, const MACAddress& u2) const
          {return u1.compareTo(u2) < 0;}
      };

      typedef std::map<MACAddress, AddressEntryMulticast, MAC_compare> AddressTableMulticast;
      AddressTableMulticast addressTableMulticast;

      /** HoT(Hosts Table) **/ //Esta tabla actualmente ya está fusionada con LT, para simplificar (la dejamos comentada por si es necesario recurrir a ella)
      /*struct ConnectedHostEntry
      {
          int portno;
      };
      typedef std::map<MACAddress, ConnectedHostEntry, MAC_compare> ConnectedHostTable;
      ConnectedHostTable connectedHostTable;*/

      /** HeT(Hello/Switches Table) **/
      std::vector<int> notHostList;

      /** RT(Repair Table) **/
      struct RepairEntry
      {
          simtime_t inTime;
      };
      typedef std::map<MACAddress,RepairEntry,MAC_compare> RepairTable;
      RepairTable repairTable;

      // Parameters to simulate link failure
      std::string portLinkDown; //Ports which links are down and times (port number ':' init time '-' end time, and separated by ';')
      struct InitEndTimeEntry //TODO: Sólo permite enteros como valores de segundos (podria cambiarse a que fuera ms o directamente generar un simtime, más complicado)
      {
          unsigned int nEvents; //Number of events (up to 10)
          unsigned int initTime[MAX_EVENTS]; //Init time (index matches endTime) for a link to be down
          unsigned int endTime[MAX_EVENTS]; //End time (index matches initTime) for a link to be up
      };
      typedef std::map<int, InitEndTimeEntry> PortEventTable; //(Port) link down and the init and end times of that event
      PortEventTable linkDownTable; //A table for each port and its failure links events (times)

      std::string switchDown; //Times at which the switch will be completely down (init time '-' end time, and separated by ';') - This parameter deletes all tables and, combined with the portLinkDown, it can simulate a switch that failed (and later was restarted)
      InitEndTimeEntry switchDownStruct; //A structure that contains the failure events (times) in which the switch will be down (it needs to be combined with portLinkDown events)
      bool switchIsDown; //'true' when the switch is down (is order to avoid sending 'Hello' messages, etc)

      MACAddress myAddress;           //MAC address that represents the switch
      MACAddress repairingAddress;    //Multicast MAC address used for the repair special messages

      // Parameters for statistics collection
      long ARPReqRcvd;                //Number of ARP Request messages received
      long ARPRepRcvd;                //Number of ARP Reply messages received
      long nRepairStarted;            //Number of path repairs started (from PathFail)
      long HelloRcvd;                 //Number of Hello messages received
      long PathFailRcvd;              //Number of PathFail messages received
      long PathRequestRcvd;           //Number of PathRequest messages received
      long PathReplyRcvd;             //Number of PathRequest messages received
      long LinkFailRcvd;              //Number of LinkFail messages received
      long LinkReplyRcvd;             //Number of LinkReply messages received
      long lostPackets;               //Number of lost packets (because no path was found and they were discarded or used for repair)

      simtime_t averagePFTime;        //PathFail message times (only measured for PathFail arriving at its destination)
      simtime_t maxPFTime;
      simtime_t minPFTime;
      unsigned long nPF;
      simtime_t averagePRqTime;       //PathRequest message times (only measured for PathRequest arriving at its destination)
      simtime_t maxPRqTime;
      simtime_t minPRqTime;
      unsigned long nPRq;
      simtime_t averagePRyTime;       //PathReply message times (only measured for PathReply arriving at its destination)
      simtime_t maxPRyTime;
      simtime_t minPRyTime;
      unsigned long nPRy;
      simtime_t averageLFTime;       //LinkFail message times (only measured for LinkFail arriving at its destination) - if there's more than one address at its destination time's divided by the group
      simtime_t maxLFTime;
      simtime_t minLFTime;
      unsigned long nLF;
      unsigned long nLFi;
      simtime_t averageLRTime;       //LinkReply message times (only measured for LinkReply arriving at its destination) - if there's more than one address at its destination time's divided by the group
      simtime_t maxLRTime;
      simtime_t minLRTime;
      unsigned long nLR;
      unsigned long nLRi;
      simtime_t averageRepairTime;    //Total repair times (only measured for completed/finished repairs)
      simtime_t maxRepairTime;
      simtime_t minRepairTime;
      unsigned long nRepairFinished;

      //Other parameters
      unsigned int broadcastSeed;     //Seed for random broadcasting (to be configured at the *.ini file)
      bool multicastActive;           //Indicates whether multicast optimization is active or not (active by default)


      /*MACRelayUnit.h (start)*/
  protected:
    //IMACAddressTable *addressTable = nullptr;
    int numPorts = 0;

    // Parameters for statistics collection
    long numProcessedFrames = 0;
    long numDiscardedFrames = 0;

    bool isOperational = false;    // for lifecycle

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

    /**
     * Calls handleIncomingFrame() for frames arrived from outside,
     * and processFrame() for self messages.
     */
    virtual void handleMessage(cMessage *msg) override;

    /**
     * Writes statistics.
     */
    virtual void finish() override;

    // for lifecycle:

  public:
    virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback) override;

  protected:
    virtual void start();
    virtual void stop();
    /*MACRelayUnit.h (end)*/

  protected:
        virtual void handleAndDispatchFrameV2(EtherFrame* frame, int inputport); //ARP-Path unidireccional (v2)
        virtual void handleAndDispatchFrameV3(EtherFrame* frame, int inputport); //ARP-Path unidireccional condicionado (v3)

        virtual void handleAndDispatchFrameV2Learning(EtherFrame* frame, MACAddress next_h);
        virtual MACAddress handleAndDispatchFrameV2Route(EtherFrame* frame);
        virtual MACAddress handleAndDispatchV2WArpPath(EtherFrame* frame, MACAddress inputHop);


        virtual void generateSwitchAddress();
        virtual void generateParametersTablesAndEvents();
        virtual void updateTables();
        virtual void updateAndRemoveAgedEntriesFromHeT();
        virtual void updateAndRemoveAgedEntriesFromRT();
        //TODO update... para MT
        virtual void removeAllEntriesFromTables();


        virtual void addEntryMulticast(AddressEntryMulticast entry, MACAddress address, int inputport, bool isNew = false);
        virtual bool findEntryMulticast(MACAddress address, AddressEntryMulticast& entryMulticast);

        virtual bool linkDownForPort(int port);
        virtual void sendHello();
        virtual bool inHeT(int port);
        virtual bool inHoT(int port);
        virtual bool inRepairTable(MACAddress address);
        virtual int portOfHost(MACAddress address);

        virtual EtherFrame* repairFrame(RepairMessageType type, MACAddress srcAddress = MACAddress(), MACAddress dstAddress = MACAddress(), simtime_t repairTime = 0); //Si no se indica
        virtual EtherFrame* groupRepairFrame(RepairMessageType type, MACAddress repairSwitch, MACAddressVector addressVector, simtime_t repairTime = 0);
        virtual void startPathRepair(MACAddress srcAddress = MACAddress(), MACAddress dstAddress = MACAddress());
        virtual void handlePathRepairFrame(EtherFrame* frame, int inputport);
        virtual void handlePathRepairFrameV2(EtherFrame* frame, int inputport);
        virtual void handlePathRepairFrameV3(EtherFrame* frame, int inputport);
        virtual void updateRepairStatistics(PathRepair* frame, unsigned int type, unsigned int nRepair = 1);

        virtual void multicastFrame(EtherFrame *frame, int inputport, AddressEntryMulticast entry);
        virtual void broadcastFrame(EtherFrame *frame, int inputport, bool repairMessage = false);
        virtual void generateSendSequence(std::vector<int>& sequence);

        virtual void printTables();
        //TODO printMT
        virtual void printHeT();
        virtual void printRT();
        virtual void printLinkDownTable();
        virtual void printSwitchDownStruct();
        virtual void printRouteStatistics();


};

} // namespace wapb

#endif // ifndef WAPB_SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_MACRELAYUNITWAPB_H

