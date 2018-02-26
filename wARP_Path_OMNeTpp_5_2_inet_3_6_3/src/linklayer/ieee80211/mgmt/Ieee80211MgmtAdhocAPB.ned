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


package wAPB.src.linklayer.ieee80211.mgmt;

import inet.linklayer.ieee80211.mgmt.IIeee80211Mgmt;

//
// 802.11 management module used for ad-hoc mode.
// Relies on the MAC layer (~Ieee80211Mac) for reception and transmission of frames.
//
// This implementation never sends control or management frames, and discards
// any such frame received. Distributed beacon generation is not modelled.
// Only data frames are understood or sent at any time.
//
// This module never switches channels, that is, it will operate on the channel
// the physical layer is configured for (see channelNumber in ~Ieee80211Radio).
//
// @author Andras Varga
//
simple Ieee80211MgmtAdhocAPB like IIeee80211Mgmt
{
    parameters:
        @class(wapb::ieee80211::Ieee80211MgmtAdhocAPB);
        string macModule;               // The path to the MAC module
        @display("i=block/cogwheel");
        //EXTRA
        // RFC 5148:
        // need more revise: As well as the decision as to whether
        // to use jitter being dependent on the medium access control and
        // lower layers, the selection of the MAXJITTER parameter SHOULD
        // be appropriate to those mechanisms.
        double maxJitter @unit("s") = default(5ms);
        volatile double jitter @unit("s") = default(uniform(0ms, maxJitter)); // jitter for broadcasts
        bool isSlottedJitter = default(false);
        
        string implementation = default("new"); //EXTRA NEW : there are 2 implementation of wARP-Path (old and new)
    gates:
        input upperLayerIn;
        output upperLayerOut;
        input macIn @labels(Ieee80211Frame);
        output macOut @labels(Ieee80211Frame);
        input agentIn @loose;   // unused dummy to statisfy the IIeee80211Mgmt interface
        output agentOut @loose; // unused dummy to statisfy the IIeee80211Mgmt interface
        //EXTRA New
        input upperLayerRelayIn @labels(EtherFrame);
        output upperLayerRelayOut @labels(EtherFrame);
        //EXTRA New End
}
