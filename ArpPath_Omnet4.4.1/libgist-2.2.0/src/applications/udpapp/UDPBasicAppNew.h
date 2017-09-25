//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
// Copyright (C) 2004,2011 Andras Varga
// Copyright (C) 2010 Diego Rivera (added startTime, stopTime, numberOfMessages)
// Copyright (C) 2012 Elisa Rojas (updated to inet2.0.0, which already has startTime, stopTime implemented... messageFreq is now sendInterval)
// Copyright (C) 2014 Isaias Martinez Yelmo (Inet 2.2.0 and Inet 2.3.0 adaptation)
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

#ifndef __INET_UDPBASICAPP_NEW_H
#define __INET_UDPBASICAPP_NEW_H

#include <vector>
#include <omnetpp.h>

#include "UDPBasicApp.h"


/**
 * UDP application. See NED for more info.
 */
class INET_API UDPBasicAppNew : public UDPBasicApp
{
  protected:

    int numberOfMessages; // EXTRA-IMY

  protected:
    virtual void initialize(int stage);
    virtual void processSend();
};

#endif


