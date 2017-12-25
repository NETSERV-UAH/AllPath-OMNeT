#
# OMNeT++/OMNEST Makefile for liblibgist
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-so -O out -I../inet/src/linklayer/ieee80211/radio -I../inet/src/linklayer/ethernet -I../inet/src/networklayer/common -I../inet/src -I../inet/src/networklayer/icmpv6 -I../inet/src/world/obstacles -I../inet/src/networklayer/contract -I../inet/src/mobility -I../inet/src/networklayer/manetrouting/base -I../inet/src/networklayer/xmipv6 -I../inet/src/linklayer/ieee80211/mgmt -I../inet/src/util -I../inet/src/transport/contract -I../inet/src/linklayer/radio/propagation -I../inet/src/linklayer/ieee80211/radio/errormodel -I../inet/src/world/powercontrol -I../inet/src/linklayer/radio -I../inet/src/util/headerserializers/tcp -I../inet/src/networklayer/ipv4 -I../inet/src/util/headerserializers/ipv4 -I../inet/src/base -I../inet/src/util/headerserializers -I../inet/src/world/radio -I../inet/src/linklayer/ieee80211/mac -I../inet/src/transport/sctp -I../inet/src/networklayer/ipv6 -I../inet/src/util/headerserializers/udp -I../inet/src/networklayer/ipv6tunneling -I../inet/src/battery/models -I../inet/src/applications/pingapp -I../inet/src/linklayer/contract -I../inet/src/util/headerserializers/sctp -I../inet/src/transport/tcp_common -I../inet/src/networklayer/arp -I../inet/src/transport/udp -I../inet/src/applications/tcpapp -L../inet/out/$(CONFIGNAME)/src -linet -DINET_IMPORT -KINET_PROJ=../inet
#

# Name of target to be created (-o option)
TARGET = liblibgist$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../inet/src/linklayer/ieee80211/radio \
    -I../inet/src/linklayer/ethernet \
    -I../inet/src/networklayer/common \
    -I../inet/src \
    -I../inet/src/networklayer/icmpv6 \
    -I../inet/src/world/obstacles \
    -I../inet/src/networklayer/contract \
    -I../inet/src/mobility \
    -I../inet/src/networklayer/manetrouting/base \
    -I../inet/src/networklayer/xmipv6 \
    -I../inet/src/linklayer/ieee80211/mgmt \
    -I../inet/src/util \
    -I../inet/src/transport/contract \
    -I../inet/src/linklayer/radio/propagation \
    -I../inet/src/linklayer/ieee80211/radio/errormodel \
    -I../inet/src/world/powercontrol \
    -I../inet/src/linklayer/radio \
    -I../inet/src/util/headerserializers/tcp \
    -I../inet/src/networklayer/ipv4 \
    -I../inet/src/util/headerserializers/ipv4 \
    -I../inet/src/base \
    -I../inet/src/util/headerserializers \
    -I../inet/src/world/radio \
    -I../inet/src/linklayer/ieee80211/mac \
    -I../inet/src/transport/sctp \
    -I../inet/src/networklayer/ipv6 \
    -I../inet/src/util/headerserializers/udp \
    -I../inet/src/networklayer/ipv6tunneling \
    -I../inet/src/battery/models \
    -I../inet/src/applications/pingapp \
    -I../inet/src/linklayer/contract \
    -I../inet/src/util/headerserializers/sctp \
    -I../inet/src/transport/tcp_common \
    -I../inet/src/networklayer/arp \
    -I../inet/src/transport/udp \
    -I../inet/src/applications/tcpapp \
    -I. \
    -Isrc \
    -Isrc/applications \
    -Isrc/applications/udpapp \
    -Isrc/linklayer \
    -Isrc/linklayer/ARPPathSwitch \
    -Isrc/linklayer/ethernet \
    -Isrc/networklayer \
    -Isrc/networklayer/arp \
    -Isrc/networklayer/ipv4 \
    -Isrc/nodes \
    -Isrc/nodes/inet \
    -Isrc/simulationmodels \
    -Isrc/simulationmodels/flowmodels

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../inet/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/src/applications/udpapp/UDPBasicAppNew.o \
    $O/src/linklayer/ARPPathSwitch/MACRelayUnitRoute.o \
    $O/src/linklayer/ethernet/EtherMACFullDuplexNew.o \
    $O/src/linklayer/ethernet/EtherMACNew.o \
    $O/src/networklayer/arp/ARPNew.o \
    $O/src/networklayer/ipv4/IPv4New.o \
    $O/src/simulationmodels/flowmodels/UDPFlowHost.o \
    $O/src/simulationmodels/flowmodels/TCPFlowHost.o \
    $O/src/simulationmodels/flowmodels/FlowGeneratorBase.o \
    $O/src/simulationmodels/flowmodels/UDPFlowGenerator.o \
    $O/src/simulationmodels/flowmodels/TCPFlowGenerator.o \
    $O/src/networklayer/arp/ARPPacketRoute_m.o

# Message files
MSGFILES = \
    src/networklayer/arp/ARPPacketRoute.msg

# Other makefile variables (-K)
INET_PROJ=../inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(SHLIB_LD) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS) $(LDFLAGS)
	$(SHLIB_POSTPROCESS) $O/$(TARGET)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f libgist libgist.exe liblibgist.so liblibgist.a liblibgist.dll liblibgist.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f src/*_m.cc src/*_m.h
	-rm -f src/applications/*_m.cc src/applications/*_m.h
	-rm -f src/applications/udpapp/*_m.cc src/applications/udpapp/*_m.h
	-rm -f src/linklayer/*_m.cc src/linklayer/*_m.h
	-rm -f src/linklayer/ARPPathSwitch/*_m.cc src/linklayer/ARPPathSwitch/*_m.h
	-rm -f src/linklayer/ethernet/*_m.cc src/linklayer/ethernet/*_m.h
	-rm -f src/networklayer/*_m.cc src/networklayer/*_m.h
	-rm -f src/networklayer/arp/*_m.cc src/networklayer/arp/*_m.h
	-rm -f src/networklayer/ipv4/*_m.cc src/networklayer/ipv4/*_m.h
	-rm -f src/nodes/*_m.cc src/nodes/*_m.h
	-rm -f src/nodes/inet/*_m.cc src/nodes/inet/*_m.h
	-rm -f src/simulationmodels/*_m.cc src/simulationmodels/*_m.h
	-rm -f src/simulationmodels/flowmodels/*_m.cc src/simulationmodels/flowmodels/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc src/*.cc src/applications/*.cc src/applications/udpapp/*.cc src/linklayer/*.cc src/linklayer/ARPPathSwitch/*.cc src/linklayer/ethernet/*.cc src/networklayer/*.cc src/networklayer/arp/*.cc src/networklayer/ipv4/*.cc src/nodes/*.cc src/nodes/inet/*.cc src/simulationmodels/*.cc src/simulationmodels/flowmodels/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/applications/udpapp/UDPBasicAppNew.o: src/applications/udpapp/UDPBasicAppNew.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddressResolver.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	$(INET_PROJ)/src/util/uint128.h \
	src/applications/udpapp/UDPBasicAppNew.h
$O/src/linklayer/ARPPathSwitch/MACRelayUnitRoute.o: src/linklayer/ARPPathSwitch/MACRelayUnitRoute.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	src/networklayer/arp/ARPPacketRoute_m.h
$O/src/linklayer/ethernet/EtherMACFullDuplexNew.o: src/linklayer/ethernet/EtherMACFullDuplexNew.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/IPassiveQueue.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherMACBase.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	src/linklayer/ethernet/EtherMACFullDuplexNew.h
$O/src/linklayer/ethernet/EtherMACNew.o: src/linklayer/ethernet/EtherMACNew.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/IPassiveQueue.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherMACBase.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	src/linklayer/ethernet/EtherMACNew.h
$O/src/networklayer/arp/ARPNew.o: src/networklayer/arp/ARPNew.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4ControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4ControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Datagram.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Datagram_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Route.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	src/networklayer/arp/ARPNew.h \
	src/networklayer/arp/ARPPacketRoute_m.h
$O/src/networklayer/arp/ARPPacketRoute_m.o: src/networklayer/arp/ARPPacketRoute_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	src/networklayer/arp/ARPPacketRoute_m.h
$O/src/networklayer/ipv4/IPv4New.o: src/networklayer/ipv4/IPv4New.cc \
	$(INET_PROJ)/src/base/AbstractQueue.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/base/ProtocolMap.h \
	$(INET_PROJ)/src/base/QueueBase.h \
	$(INET_PROJ)/src/base/ReassemblyBuffer.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4ControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4ControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMP.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPAccess.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Datagram.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Datagram_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4FragBuf.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Route.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	src/networklayer/arp/ARPPacketRoute_m.h \
	src/networklayer/ipv4/IPv4New.h
$O/src/simulationmodels/flowmodels/FlowGeneratorBase.o: src/simulationmodels/flowmodels/FlowGeneratorBase.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddressResolver.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	$(INET_PROJ)/src/util/uint128.h \
	src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	src/networklayer/arp/ARPNew.h \
	src/networklayer/arp/ARPPacketRoute_m.h \
	src/simulationmodels/flowmodels/FlowGeneratorBase.h \
	src/simulationmodels/flowmodels/UDPFlowHost.h
$O/src/simulationmodels/flowmodels/TCPFlowGenerator.o: src/simulationmodels/flowmodels/TCPFlowGenerator.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	$(INET_PROJ)/src/util/uint128.h \
	src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	src/networklayer/arp/ARPNew.h \
	src/networklayer/arp/ARPPacketRoute_m.h \
	src/simulationmodels/flowmodels/FlowGeneratorBase.h \
	src/simulationmodels/flowmodels/TCPFlowGenerator.h \
	src/simulationmodels/flowmodels/UDPFlowHost.h
$O/src/simulationmodels/flowmodels/TCPFlowHost.o: src/simulationmodels/flowmodels/TCPFlowHost.cc \
	$(INET_PROJ)/src/applications/tcpapp/TCPBasicClientApp.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericCliAppBase.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericSrvApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/util/uint128.h \
	src/simulationmodels/flowmodels/TCPFlowHost.h
$O/src/simulationmodels/flowmodels/UDPFlowGenerator.o: src/simulationmodels/flowmodels/UDPFlowGenerator.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	$(INET_PROJ)/src/util/uint128.h \
	src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	src/networklayer/arp/ARPNew.h \
	src/networklayer/arp/ARPPacketRoute_m.h \
	src/simulationmodels/flowmodels/FlowGeneratorBase.h \
	src/simulationmodels/flowmodels/UDPFlowGenerator.h \
	src/simulationmodels/flowmodels/UDPFlowHost.h
$O/src/simulationmodels/flowmodels/UDPFlowHost.o: src/simulationmodels/flowmodels/UDPFlowHost.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	$(INET_PROJ)/src/util/uint128.h \
	src/simulationmodels/flowmodels/UDPFlowHost.h

