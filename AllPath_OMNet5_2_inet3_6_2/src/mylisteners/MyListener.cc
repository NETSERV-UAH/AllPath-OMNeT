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

#include "./MyListener.h"

namespace allpath {

MyListener::MyListener() {
    // TODO Auto-generated constructor stub
    getSimulation()->getSystemModule()->subscribe(PRE_MODEL_CHANGE, this);

}

MyListener::~MyListener() {
    // TODO Auto-generated destructor stub
}

void MyListener::receiveSignal(cComponent *src, simsignal_t id, cObject *value,cObject *details)
{
    if (dynamic_cast<cPreGateDisconnectNotification *>(value))
    {
        cPreGateDisconnectNotification *data = (cPreGateDisconnectNotification *)value;
        cPreGateDisconnectNotification *det = (cPreGateDisconnectNotification *)details;
        EV << "data:gate:fullname: " << data->gate->getFullName() << " is about to be deleted.\n";
        EV << "data:gate:base name: " << data->gate->getBaseName()<<"index:"<< data->gate->getIndex()<< "\n";
        EV << "data:gate:name: " << data->gate->getName()<<"index:"<< data->gate->getIndex()<< "\n";
        EV << "data: full name " << data->getFullName() << "\n";
        EV << "data: name " << data->getName() << "\n";
        EV << "data: full path " << data->getFullPath() << "\n";
        EV << "src:fullname : " << src->getFullName() <<"\n";
        EV << "src:full path : " << src->getFullPath() << "\n";
        EV << "src:descriptor : " << src->getDescriptor() << "\n";
        EV << "src:display str : " << src->getDisplayString() << "\n";
        EV << "src:name : " << src->getName() << "\n";



        if (det!=nullptr)
           EV <<"details :"<<det->info()<<"\n";
    }
}

} /* namespace inet */
