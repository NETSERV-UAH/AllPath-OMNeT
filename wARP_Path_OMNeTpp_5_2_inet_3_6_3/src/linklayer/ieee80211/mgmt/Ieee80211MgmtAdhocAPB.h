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

#ifndef WAPB_SRC_LINKLAYER_IEEE80211_MGMT_IEEE80211MGMTADHOC_H
#define WAPB_SRC_LINKLAYER_IEEE80211_MGMT_IEEE80211MGMTADHOC_H

#include "inet/common/INETDefs.h"

#include "inet/linklayer/ieee80211/mgmt/Ieee80211MgmtBase.h"

namespace inet{

class EtherFrame;

}
namespace wapb {
namespace ieee80211 {

using namespace inet;
using namespace inet::ieee80211;

/**
 * Used in 802.11 ad-hoc mode. See corresponding NED file for a detailed description.
 * This implementation ignores many details.
 *
 * @author Andras Varga
 */
class Ieee80211MgmtAdhocAPB : public Ieee80211MgmtBase
{
  protected:
    //EXTRA begin
    MACAddress previous_resolution_address;
    const char *implementation;
    cPar *jitterPar = nullptr;
    double maxJitter;
    bool isSlottedJitter = false;
    //EXTRA end

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int) override;

    /** Implements abstract Ieee80211MgmtBase method */
    virtual void handleTimer(cMessage *msg) override;

    /** Implements abstract Ieee80211MgmtBase method */
    virtual void handleUpperMessage(cPacket *msg) override;
    // wARP-Path, new version
    virtual void handleUpperMessageNewWAPB(cPacket *msg);
    // wARP-Path, new version
    virtual void handleUpperMessageOldWAPB(cPacket *msg);



    /** Implements abstract Ieee80211MgmtBase method -- throws an error (no commands supported) */
    virtual void handleCommand(int msgkind, cObject *ctrl) override;

    /** Utility function for handleUpperMessage() */
    virtual Ieee80211DataFrame *encapsulate(cPacket *msg);

    /** Utility method to decapsulate a data frame */
    virtual cPacket *decapsulate(Ieee80211DataFrame *frame);
    // wARP-Path
    virtual EtherFrame *convertToEtherFrame(Ieee80211DataFrame *frame_);
    // wARP-Path, new version
    virtual void handleDataFrameNewVersionWAPB(Ieee80211DataFrame *frame);
    // wARP-Path, old version
    virtual void handleDataFrameOldVersionWAPB(Ieee80211DataFrame *frame);
    // wARP-Path, to handle RFC 5148 (jitter)
    virtual void sendDownDelayed(cPacket *frame, double delay);
    // wARP-Path, to handle slotted jitter)
    virtual void sendDownSlottedDelayed(cPacket *frame, double delay);

    /** @name Processing of different frame types */
    //@{
    virtual void handleDataFrame(Ieee80211DataFrame *frame) override;
    virtual void handleAuthenticationFrame(Ieee80211AuthenticationFrame *frame) override;
    virtual void handleDeauthenticationFrame(Ieee80211DeauthenticationFrame *frame) override;
    virtual void handleAssociationRequestFrame(Ieee80211AssociationRequestFrame *frame) override;
    virtual void handleAssociationResponseFrame(Ieee80211AssociationResponseFrame *frame) override;
    virtual void handleReassociationRequestFrame(Ieee80211ReassociationRequestFrame *frame) override;
    virtual void handleReassociationResponseFrame(Ieee80211ReassociationResponseFrame *frame) override;
    virtual void handleDisassociationFrame(Ieee80211DisassociationFrame *frame) override;
    virtual void handleBeaconFrame(Ieee80211BeaconFrame *frame) override;
    virtual void handleProbeRequestFrame(Ieee80211ProbeRequestFrame *frame) override;
    virtual void handleProbeResponseFrame(Ieee80211ProbeResponseFrame *frame) override;
    //@}
};

} // namespace ieee80211

} // namespace wapb

#endif // ifndef WAPB_SRC_LINKLAYER_IEEE80211_MGMT_IEEE80211MGMTADHOC_H

