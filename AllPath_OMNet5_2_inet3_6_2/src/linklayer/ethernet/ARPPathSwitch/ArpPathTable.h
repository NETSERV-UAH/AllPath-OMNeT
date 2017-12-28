#ifndef SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_ARPPATHTABLE_H_
#define SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_ARPPATHTABLE_H_

#include <iostream>
#include "inet/common/INETDefs.h"

#include "inet/linklayer/common/MACAddress.h"
using namespace inet;

namespace allpath {

class ArpPathTable //:public cSimpleModule
{
    private:
        class tableEntry
        {

            public:
               int portNumber;
               MACAddress MAC_Address;
               simtime_t last_Update;
               tableEntry* leftChild;
               tableEntry* rightChild;
               tableEntry(void);
               ~tableEntry(void);
         };


         tableEntry *Root=NULL;


    public:
         ArpPathTable(void);
        ~ArpPathTable(void);
        void insert(MACAddress MAC_Address,int portNumber,simtime_t now);
        int  find(MACAddress MAC_Address);
        void update(MACAddress MAC_Address,int portNumber,simtime_t now);
    private:
        void refresh(MACAddress MAC_Address,int portNumber,simtime_t now);

    protected:



};

} // namespace inet
#endif // SRC_LINKLAYER_ETHERNET_ARPPATHSWITCH_ARPPATHTABLE_H_
