//
// Copyright (C) 2012 Elisa Rojas (inspired in inet/UDPBasicApp)
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

#ifndef UDPFLOWHOST_H_
#define UDPFLOWHOST_H_

#include <omnetpp.h>
#include <string>
#include "UDPSocket.h"

/**
 * It allows the corresponding UDP traffic generation module to generate traffic at a specific host
 *
 * For more info please see the NED file.
 */

class UDPFlowHost : public cSimpleModule
{
	friend class UDPFlowGenerator;

  protected:
    //To save info about the currently active flows from this source host
    struct UDPFlowInfo {
        UDPSocket socket; //Combination of localPort, destPort y destAddress (srcAddress es siempre la misma)
    	int localPort; //From 1000 to 1999
    	int destPort;  //From 2000 to 2999
    	IPvXAddress destAddress;
        unsigned int transferRate;
        unsigned long long flowSize; //(B), el unsigned int/long sólo llega a 4*10^9 no al 8*10^9 necesario
        unsigned int frameSize;
    };
    typedef std::vector<UDPFlowInfo> FlowInfoVector;

    simtime_t stopTime;

    static int counter; // counter for generating a global number for each packet
    int numSent;
    int numReceived;
    int nHosts; //Number of hosts in the topology

    static simsignal_t sentPkSignal;
    static simsignal_t rcvdPkSignal;

  private:
    FlowInfoVector flowInfo; //Vector that contains the currently active flows info
    std::vector<cMessage*> flowMessages;

  public:
	UDPFlowHost();
	virtual ~UDPFlowHost();

  protected:
    virtual int numInitStages() const  {return 4;} //At least 3 (=4-1) because we need FlatNetworkConfigurator to be initialized (and it does at stage 2)
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

    virtual void startFlow(unsigned int transferRate, unsigned long long flowSize, unsigned int frameSize, const IPvXAddress& destAddr); //Kbps, B(KB*1000), B, address
    virtual cPacket *createPacket(unsigned int frameSize);
    virtual void sendPacket(unsigned int frameSize, unsigned int nFlow);
    virtual void processPacket(cPacket *pk);
    virtual void setSocketOptions(UDPSocket socket);

    virtual void deleteMessageFromVector(cMessage *msg);

  public:
    virtual void updateHostsInfo(unsigned int n, simtime_t stop); //To update the number of hosts in the network and the flowInfo vector size and binding
};



#endif /* UDPFLOWHOST_H_ */
