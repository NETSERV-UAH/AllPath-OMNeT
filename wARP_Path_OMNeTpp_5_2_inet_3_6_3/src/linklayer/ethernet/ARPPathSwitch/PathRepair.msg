// PathRepair Message is a special frame based on EtherFrameWithLLC (from INET framework)
// but adding another field (type), allowing the switch to know what kind of pathRepair frame
// arrives.
// Authors: Diego Rivera (2010)
//		   	Elisa Rojas (2013)

cplusplus {{
#include <vector>
//#include <sys/types.h>
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/linklayer/common/MACAddress.h"
//typedef std::vector<MACAddress> MACAddressVector;
}}

//class noncobject MACAddress;
//class noncobject MACAddressVector;

namespace inet;
class noncobject ::MACAddress;
cplusplus {{
typedef std::vector<MACAddress> MACAddressVector;
}}
class noncobject MACAddressVector;

message EtherFrame;
//EtherFrameWithLLC;

//Types of repair messages for ARP-Path
enum RepairMessageType 
{ 
    Hello = 1; 			//Hello message (from switch to switch)		--Repair 1/2
    PathFail = 2; 		//PathFail message (broadcast - BT)			--Repair 1/2
    PathRequest = 3; 	//PathRequest message (broadcast - LT)		--Repair 1/2
    PathReply = 4;		//PathReply message (unicast - LT)			--Repair 1/2
    LinkFail = 5;		//LinkFail message, which can contain several addresses ~ several PathFail messages (broadcast - LT)	--Repair 3
    LinkReply = 6;		//LinkReply message, which can contain several addresses ~ several PathReply messages (unicast - LT)	--Repair 3
}

//PathRepair message fields:
//	- Type (RepairMessageType)
//	- Source MAC address
//	- Destination MAC address
message PathRepair extends EtherFrame //EtherFrameWithLLC en la version antigua
{
    int type enum(RepairMessageType);		//Message type
    MACAddress srcMACAddress;				//Address to be learnt (repairType == 1 || 2)
    MACAddress destMACAddress;				//Address to be learnt in the following step (if applicable, i.e. repairType == 1)
	simtime_t repairTime;					//Contains the repairTime of any previous repair message (repairTime is always zero for Hello/PathFail messages)
	MACAddress repairSwitch;				//Address to which forward all LinkReply messages (address of the switch that originated the repair)
	MACAddressVector repairMACAddresses;	//Group of addresses to be learnt/repaired (repairType == 3)
}
