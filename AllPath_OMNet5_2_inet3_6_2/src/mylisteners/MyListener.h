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

#ifndef SRC_MYLISTENERS_MYLISTENER_H_
#define SRC_MYLISTENERS_MYLISTENER_H_

#include "inet/common/INETDefs.h"
#include <omnetpp/clistener.h>

namespace allpath {

class MyListener: public omnetpp::cListener {
public:
    MyListener();
    virtual ~MyListener();
    void receiveSignal(cComponent *src, simsignal_t id, cObject *value,cObject *details) override;
};

} /* namespace inet */

#endif /* SRC_MYLISTENERS_MYLISTENER_H_ */
