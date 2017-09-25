/*
 * Copyright (C) 2012/2013 Elisa Rojas; GIST, University of Alcala, Spain
 *              based on work of 2010 Diego Rivera (for previous versions of Omnet and the inet framework.
 *
 * MACRelayUnitAPB module defines the ARP-Path switch behaviour (SAu version). It is based on the MACRelayUnitNP module from the Inet Framework.
 * Since the MACRelayUnitAPB couldn't be written as an extension of MACRelayUnitNP (due to the characteristics of it),
 * but as an extended copy of that module, any update of the inet framework MACRelayUnitNP should be made in this module as well.
 * We've tried to keep this module as organized as possible, so that future updates are easy.
 * LAST UPDATE OF THE INET FRAMEWORK: inet2.0 @ 14/09/2012
*/

#ifndef __ARPPATHBRIDGESUNIDIRECCIONAL_MACRELAYUNITAPB_H_
#define __ARPPATHBRIDGESUNIDIRECCIONAL_MACRELAYUNITAPB_H_

#include <omnetpp.h>
#include "MACRelayUnitNP.h"
#include "IPvXAddress.h"
#include "EtherFrame_m.h"
#include "Ethernet.h"
#include "MACAddress.h"
#include "PathRepair_m.h"
#include <sstream>
#include "ARPPacketRoute_m.h"
#include "MACRelayUnitRoute.h"

#define ARPPATH_MCAST_ADD "FE:FF:FF:FF:FF:FF" //TODO: No es una dirección multicast, sino unicast. Lo hemos dejado así porque falla con mcast y bcast (bug de OMNeT++? se llama a una función de EtherEncap y da error...)
#define MAX_EVENTS 10 //Maximum number of events for a link = 10 (this is to save memory, but vectors could be bigger if needed)
#define NPORTS 64 //Number of ports in the switch (for multicast table)

class MACRelayUnitAPB : public MACRelayUnitNP
{

    /* EXTRA-IMY functions modified by ARPPAY */

     public:
       MACRelayUnitAPB();
       virtual ~MACRelayUnitAPB();

     protected:

        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);
        virtual void finish();
        virtual void handleIncomingFrame(EtherFrame *msg);
        virtual void processFrame(cMessage *msg);
        /*MACRelayUnitNP.h (end)*/

    /* EXTRA-IMY */

/*MACRelayUnitAPB extension from here...*/
     protected:
        unsigned int protocolVersion;   //Two versions of ARP-Path are implemented (2 -unidirectional- and 3 -conditional-, not 1 because it didn't work properly)
        unsigned int repairType;        //Type of repair to be applied (1,2,3: PF+PRq+PRp; PF+PRq; PF+PRp) -- 0:NoRepair (saves events at executing if we will not use any repair method)

        // Configuration of ARP-Path
        //simtime_t agingTime; in MACRelayUnitBase defines the "Learning Time" for LT
        simtime_t blockingTime;     //"Locking Time" for BT(Broadcast/Blocking Table) and LT(Lookup/Learning Table)
        simtime_t helloTime;        //"Hello" frequency time and "Locking Time" for HeT = helloTime+lockingTime (not used)
        simtime_t repairingTime;    //"Repairing Time" for RT(Repair Table)
        int relayQueueLimit;        //Queue Limit

            /** LT(Lookup/Learning Table) **/
        enum Status { none = 0, locked = 1, learnt = 2}; //Default: 0,1,2
        struct AddressEntry //Entry of LT - key is MAC address to forward to
        {
            int port;                   // Input port, used to learn/send (v2) and only to learn (v3)
            int portToSend;             // Original (prioritary) input port, used to send (only v3)
            simtime_t inTime;           // Creation time of the entry (timer)
            int status;                 // Entry Status (locked, learnt)
            IPvXAddress ip;             // host IP address (for EtherProxy)
        };
        struct MAC_compare
        {
            bool operator()(const MACAddress& u1, const MACAddress& u2) const
            {return u1.compareTo(u2) < 0;}
        };
        typedef std::map<MACAddress, AddressEntry, MAC_compare> AddressTable;
        AddressTable addressTable;

            /** BT(Blocking/Broadcast Table) **/
        struct AddressEntryBasic //Entry of BT - key is MAC address to lock (to avoid loops)
        {
            int port;                   // Input port
            simtime_t inTime;           // Creation time of the entry (timer)
        };

        typedef std::map<MACAddress, AddressEntryBasic, MAC_compare> AddressTableBasic;
        AddressTableBasic addressTableBasic;

            /** MT(Multicast Table) **/
        struct AddressEntryMulticast //Entry of MT - key is multicast group MAC address
        {
            bool port[NPORTS];             // Port that belongs to the multicast group (e.g. up to 64 ports -> 64 bits/flags in real tables)
            simtime_t inTime;              // Creation time of the entry (timer)
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
        cOutVector queueLength;         //Vector that saves the evolution of the queue
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

     protected:
        virtual void handleAndDispatchFrame(EtherFrame *frame, int inputport);
        virtual void handleAndDispatchFrameV2(EtherFrame* frame, int inputport); //ARP-Path unidireccional (v2)
        virtual void handleAndDispatchFrameV3(EtherFrame* frame, int inputport); //ARP-Path unidireccional condicionado (v3)

        virtual void generateSwitchAddress();
        virtual void generateParametersTablesAndEvents();
        virtual void updateTables();
        virtual void updateAndRemoveAgedEntriesFromLT();
        virtual void updateAndRemoveAgedEntriesFromBT();
        virtual void updateAndRemoveAgedEntriesFromHeT();
        virtual void updateAndRemoveAgedEntriesFromRT();
        //TODO update... para MT
        virtual void removeAllEntriesFromTables();

        virtual void addEntry(AddressEntry entry, MACAddress address, int inputport);
        virtual void addEntryBasic(AddressEntryBasic entry, MACAddress address, int inputport);
        virtual void addEntryMulticast(AddressEntryMulticast entry, MACAddress address, int inputport, bool isNew = false);
        virtual bool findEntry(MACAddress address, AddressEntry& entry);
        virtual bool findEntryBasic(MACAddress address, AddressEntryBasic& entryBasic);
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

        virtual int getNumberOfEntriesForPort(int port);
        virtual void printTables();
        virtual void printLT();
        virtual void printBT();
        //TODO printMT
        virtual void printHeT();
        virtual void printRT();
        virtual void printLinkDownTable();
        virtual void printSwitchDownStruct();
        virtual void printRouteStatistics();
};

#endif
