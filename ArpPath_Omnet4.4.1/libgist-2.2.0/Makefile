#
# OMNeT++/OMNEST Makefile for liblibgist
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-so -O out -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/common -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/icmpv6 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/xmipv6 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/contract -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/autorouting/ipv4 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/transport/contract -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/status -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers/tcp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/ipv4 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/mobility/contract -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers/ipv4 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/base -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/ipv6 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/transport/sctp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers/udp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/ipv6tunneling -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers/ipv6 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/applications/pingapp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/linklayer/contract -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers/sctp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/arp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/transport/tcp_common -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/transport/udp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/linklayer/ethernet -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/linklayer/common -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/applications/tcpapp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/applications/udpapp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/applications/common -L/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/out/$$\(CONFIGNAME\)/src -linet -DINET_IMPORT -KINET_PROJ=/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0
#

# Name of target to be created (-o option)
TARGET = liblibgist$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I$(INET_PROJ)/src/networklayer/common \
    -I$(INET_PROJ)/src/networklayer/icmpv6 \
    -I$(INET_PROJ)/src/networklayer/xmipv6 \
    -I$(INET_PROJ)/src/networklayer/contract \
    -I$(INET_PROJ)/src/networklayer/autorouting/ipv4 \
    -I$(INET_PROJ)/src/util \
    -I$(INET_PROJ)/src/transport/contract \
    -I$(INET_PROJ)/src/status \
    -I$(INET_PROJ)/src/util/headerserializers/tcp \
    -I$(INET_PROJ)/src/networklayer/ipv4 \
    -I$(INET_PROJ)/src/mobility/contract \
    -I$(INET_PROJ)/src/util/headerserializers/ipv4 \
    -I$(INET_PROJ)/src/base \
    -I$(INET_PROJ)/src/util/headerserializers \
    -I$(INET_PROJ)/src/networklayer/ipv6 \
    -I$(INET_PROJ)/src/transport/sctp \
    -I$(INET_PROJ)/src/util/headerserializers/udp \
    -I$(INET_PROJ)/src/networklayer/ipv6tunneling \
    -I$(INET_PROJ)/src/util/headerserializers/ipv6 \
    -I$(INET_PROJ)/src/applications/pingapp \
    -I$(INET_PROJ)/src/linklayer/contract \
    -I$(INET_PROJ)/src/util/headerserializers/sctp \
    -I$(INET_PROJ)/src/networklayer/arp \
    -I$(INET_PROJ)/src/transport/tcp_common \
    -I$(INET_PROJ)/src/transport/udp \
    -I$(INET_PROJ)/src/linklayer/ethernet \
    -I$(INET_PROJ)/src/linklayer/common \
    -I$(INET_PROJ)/src/applications/tcpapp \
    -I$(INET_PROJ)/src/applications/udpapp \
    -I$(INET_PROJ)/src/applications/common \
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
LIBS = -L$(INET_PROJ)/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath $(INET_PROJ)/out/$(CONFIGNAME)/src`

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
    $O/src/simulationmodels/flowmodels/FlowGeneratorBase.o \
    $O/src/simulationmodels/flowmodels/TCPFlowGenerator.o \
    $O/src/simulationmodels/flowmodels/TCPFlowHost.o \
    $O/src/simulationmodels/flowmodels/UDPFlowGenerator.o \
    $O/src/simulationmodels/flowmodels/UDPFlowHost.o \
    $O/src/networklayer/arp/ARPPacketRoute_m.o

# Message files
MSGFILES = \
    src/networklayer/arp/ARPPacketRoute.msg

# Other makefile variables (-K)
INET_PROJ=/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0

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
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating shared library: $@
	$(Q)$(SHLIB_LD) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS) $(LDFLAGS)
	$(Q)$(SHLIB_POSTPROCESS) $O/$(TARGET)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f libgist libgist.exe liblibgist.so liblibgist.a liblibgist.dll liblibgist.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f src/*_m.cc src/*_m.h
	$(Q)-rm -f src/applications/*_m.cc src/applications/*_m.h
	$(Q)-rm -f src/applications/udpapp/*_m.cc src/applications/udpapp/*_m.h
	$(Q)-rm -f src/linklayer/*_m.cc src/linklayer/*_m.h
	$(Q)-rm -f src/linklayer/ARPPathSwitch/*_m.cc src/linklayer/ARPPathSwitch/*_m.h
	$(Q)-rm -f src/linklayer/ethernet/*_m.cc src/linklayer/ethernet/*_m.h
	$(Q)-rm -f src/networklayer/*_m.cc src/networklayer/*_m.h
	$(Q)-rm -f src/networklayer/arp/*_m.cc src/networklayer/arp/*_m.h
	$(Q)-rm -f src/networklayer/ipv4/*_m.cc src/networklayer/ipv4/*_m.h
	$(Q)-rm -f src/nodes/*_m.cc src/nodes/*_m.h
	$(Q)-rm -f src/nodes/inet/*_m.cc src/nodes/inet/*_m.h
	$(Q)-rm -f src/simulationmodels/*_m.cc src/simulationmodels/*_m.h
	$(Q)-rm -f src/simulationmodels/flowmodels/*_m.cc src/simulationmodels/flowmodels/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc src/*.cc src/applications/*.cc src/applications/udpapp/*.cc src/linklayer/*.cc src/linklayer/ARPPathSwitch/*.cc src/linklayer/ethernet/*.cc src/networklayer/*.cc src/networklayer/arp/*.cc src/networklayer/ipv4/*.cc src/nodes/*.cc src/nodes/inet/*.cc src/simulationmodels/*.cc src/simulationmodels/flowmodels/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/applications/udpapp/UDPBasicAppNew.o: src/applications/udpapp/UDPBasicAppNew.cc \
	$(INET_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_PROJ)/src/applications/udpapp/UDPBasicApp.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/ILifecycle.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/LifecycleOperation.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NodeOperations.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/base/OperationalBase.h \
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
	src/applications/udpapp/UDPBasicAppNew.h
$O/src/linklayer/ARPPathSwitch/MACRelayUnitRoute.o: src/linklayer/ARPPathSwitch/MACRelayUnitRoute.cc \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	src/networklayer/arp/ARPPacketRoute_m.h
$O/src/linklayer/ethernet/EtherMACFullDuplexNew.o: src/linklayer/ethernet/EtherMACFullDuplexNew.cc \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/ILifecycle.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/IPassiveQueue.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/common/MACBase.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherMACBase.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherMACFullDuplex.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/status/NodeStatus.h \
	src/linklayer/ethernet/EtherMACFullDuplexNew.h
$O/src/linklayer/ethernet/EtherMACNew.o: src/linklayer/ethernet/EtherMACNew.cc \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/ILifecycle.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/IPassiveQueue.h \
	$(INET_PROJ)/src/linklayer/common/MACBase.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherMAC.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherMACBase.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/status/NodeStatus.h \
	src/linklayer/ethernet/EtherMACNew.h
$O/src/networklayer/arp/ARPNew.o: src/networklayer/arp/ARPNew.cc \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/ILifecycle.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/arp/ARP.h \
	$(INET_PROJ)/src/networklayer/arp/IARPCache.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4ControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4ControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Datagram.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Datagram_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Route.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	src/networklayer/arp/ARPNew.h \
	src/networklayer/arp/ARPPacketRoute_m.h
$O/src/networklayer/arp/ARPPacketRoute_m.o: src/networklayer/arp/ARPPacketRoute_m.cc \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	src/networklayer/arp/ARPPacketRoute_m.h
$O/src/networklayer/ipv4/IPv4New.o: src/networklayer/ipv4/IPv4New.cc \
	$(INET_PROJ)/src/base/AbstractQueue.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/ILifecycle.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/LifecycleOperation.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NodeOperations.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/base/ProtocolMap.h \
	$(INET_PROJ)/src/base/QueueBase.h \
	$(INET_PROJ)/src/base/ReassemblyBuffer.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/arp/IARPCache.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/INetfilter.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPSocket.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4ControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4ControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMP.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPAccess.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Datagram.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Datagram_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4FragBuf.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4Route.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/status/NodeStatus.h \
	src/networklayer/arp/ARPPacketRoute_m.h \
	src/networklayer/ipv4/IPv4New.h
$O/src/simulationmodels/flowmodels/FlowGeneratorBase.o: src/simulationmodels/flowmodels/FlowGeneratorBase.cc \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/ILifecycle.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/arp/ARP.h \
	$(INET_PROJ)/src/networklayer/arp/IARPCache.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddressResolver.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	src/networklayer/arp/ARPNew.h \
	src/networklayer/arp/ARPPacketRoute_m.h \
	src/simulationmodels/flowmodels/FlowGeneratorBase.h \
	src/simulationmodels/flowmodels/UDPFlowHost.h
$O/src/simulationmodels/flowmodels/TCPFlowGenerator.o: src/simulationmodels/flowmodels/TCPFlowGenerator.cc \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/ILifecycle.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/arp/ARP.h \
	$(INET_PROJ)/src/networklayer/arp/IARPCache.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	src/networklayer/arp/ARPNew.h \
	src/networklayer/arp/ARPPacketRoute_m.h \
	src/simulationmodels/flowmodels/FlowGeneratorBase.h \
	src/simulationmodels/flowmodels/TCPFlowGenerator.h \
	src/simulationmodels/flowmodels/UDPFlowHost.h
$O/src/simulationmodels/flowmodels/TCPFlowHost.o: src/simulationmodels/flowmodels/TCPFlowHost.cc \
	$(INET_PROJ)/src/applications/tcpapp/TCPAppBase.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPBasicClientApp.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericSrvApp.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/ILifecycle.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/LifecycleOperation.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/status/NodeStatus.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	src/simulationmodels/flowmodels/TCPFlowHost.h
$O/src/simulationmodels/flowmodels/UDPFlowGenerator.o: src/simulationmodels/flowmodels/UDPFlowGenerator.cc \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/ILifecycle.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/arp/ARP.h \
	$(INET_PROJ)/src/networklayer/arp/IARPCache.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/UDPSocket.h \
	src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	src/networklayer/arp/ARPNew.h \
	src/networklayer/arp/ARPPacketRoute_m.h \
	src/simulationmodels/flowmodels/FlowGeneratorBase.h \
	src/simulationmodels/flowmodels/UDPFlowGenerator.h \
	src/simulationmodels/flowmodels/UDPFlowHost.h
$O/src/simulationmodels/flowmodels/UDPFlowHost.o: src/simulationmodels/flowmodels/UDPFlowHost.cc \
	$(INET_PROJ)/src/base/Compat.h \
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
	src/simulationmodels/flowmodels/UDPFlowHost.h

