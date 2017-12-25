#
# OMNeT++/OMNEST Makefile for ARPPathBridgesUnidireccional
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I../inet/src/networklayer/ipv4 -I../inet/src/networklayer/common -I../inet/src/networklayer/rsvp_te -I../inet/src/networklayer/icmpv6 -I../inet/src/transport/tcp -I../inet/src/networklayer/mpls -I../inet/src/base -I../inet/src/networklayer/ted -I../inet/src/util/headerserializers -I../inet/src/networklayer/contract -I../inet/src/util -I../inet/src/transport/contract -I../inet/src/linklayer/mfcore -I../inet/src/transport/sctp -I../inet/src/networklayer/ipv6 -I../inet/src/world -I../inet/src/applications/pingapp -I../inet/src/linklayer/contract -I../inet/src/networklayer/arp -I../inet/src/networklayer/ldp -I../inet/src/transport/udp -I../inet/src/linklayer/ethernet -I../inet/src/applications/udpapp -I../inet/src/linklayer/etherswitch -L../inet/out/$(CONFIGNAME)/src -linet -KINET_PROJ=../inet
#

# Name of target to be created (-o option)
TARGET = ARPPathBridgesUnidireccional$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../inet/src/networklayer/ipv4 \
    -I../inet/src/networklayer/common \
    -I../inet/src/networklayer/rsvp_te \
    -I../inet/src/networklayer/icmpv6 \
    -I../inet/src/transport/tcp \
    -I../inet/src/networklayer/mpls \
    -I../inet/src/base \
    -I../inet/src/networklayer/ted \
    -I../inet/src/util/headerserializers \
    -I../inet/src/networklayer/contract \
    -I../inet/src/util \
    -I../inet/src/transport/contract \
    -I../inet/src/linklayer/mfcore \
    -I../inet/src/transport/sctp \
    -I../inet/src/networklayer/ipv6 \
    -I../inet/src/world \
    -I../inet/src/applications/pingapp \
    -I../inet/src/linklayer/contract \
    -I../inet/src/networklayer/arp \
    -I../inet/src/networklayer/ldp \
    -I../inet/src/transport/udp \
    -I../inet/src/linklayer/ethernet \
    -I../inet/src/applications/udpapp \
    -I../inet/src/linklayer/etherswitch \
    -I. \
    -Iconfs \
    -Iresults \
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
    -Isrc/nodes/ethernet \
    -Isrc/nodes/inet \
    -Isrc/nonARPPathTopologies \
    -Isrc/nonARPPathTopologies/OSPFtest \
    -Isrc/nonARPPathTopologies/OSPFtest/results \
    -Isrc/nonARPPathTopologies/OSPFtest10 \
    -Isrc/nonARPPathTopologies/OSPFtest10/results \
    -Isrc/nonARPPathTopologies/OSPFtest11 \
    -Isrc/nonARPPathTopologies/OSPFtest11/results \
    -Isrc/nonARPPathTopologies/OSPFtest12 \
    -Isrc/nonARPPathTopologies/OSPFtest12/results \
    -Isrc/nonARPPathTopologies/OSPFtest13 \
    -Isrc/nonARPPathTopologies/OSPFtest13/results \
    -Isrc/nonARPPathTopologies/OSPFtest2 \
    -Isrc/nonARPPathTopologies/OSPFtest2/results \
    -Isrc/nonARPPathTopologies/OSPFtest3 \
    -Isrc/nonARPPathTopologies/OSPFtest3/results \
    -Isrc/nonARPPathTopologies/OSPFtest4 \
    -Isrc/nonARPPathTopologies/OSPFtest4/results \
    -Isrc/nonARPPathTopologies/OSPFtest5 \
    -Isrc/nonARPPathTopologies/OSPFtest5/results \
    -Isrc/nonARPPathTopologies/OSPFtest6 \
    -Isrc/nonARPPathTopologies/OSPFtest6/results \
    -Isrc/nonARPPathTopologies/OSPFtest7 \
    -Isrc/nonARPPathTopologies/OSPFtest7/results \
    -Isrc/nonARPPathTopologies/OSPFtest8 \
    -Isrc/nonARPPathTopologies/OSPFtest8/results \
    -Isrc/nonARPPathTopologies/OSPFtest9 \
    -Isrc/nonARPPathTopologies/OSPFtest9/results \
    -Isrc/nonARPPathTopologies/Topology3 \
    -Isrc/nonARPPathTopologies/Topology3/results \
    -Isrc/topologies

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
    $O/src/linklayer/ARPPathSwitch/MACRelayUnitAPB.o \
    $O/src/linklayer/ethernet/EtherMACNew.o \
    $O/src/linklayer/ethernet/EtherMACBaseNew.o \
    $O/src/networklayer/arp/ARPnew.o \
    $O/src/networklayer/ipv4/IPNew.o \
    $O/src/linklayer/ARPPathSwitch/PathRepair_m.o

# Message files
MSGFILES = \
    src/linklayer/ARPPathSwitch/PathRepair.msg

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
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f ARPPathBridgesUnidireccional ARPPathBridgesUnidireccional.exe libARPPathBridgesUnidireccional.so libARPPathBridgesUnidireccional.a libARPPathBridgesUnidireccional.dll libARPPathBridgesUnidireccional.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f confs/*_m.cc confs/*_m.h
	-rm -f results/*_m.cc results/*_m.h
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
	-rm -f src/nodes/ethernet/*_m.cc src/nodes/ethernet/*_m.h
	-rm -f src/nodes/inet/*_m.cc src/nodes/inet/*_m.h
	-rm -f src/nonARPPathTopologies/*_m.cc src/nonARPPathTopologies/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest/*_m.cc src/nonARPPathTopologies/OSPFtest/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest/results/*_m.cc src/nonARPPathTopologies/OSPFtest/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest10/*_m.cc src/nonARPPathTopologies/OSPFtest10/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest10/results/*_m.cc src/nonARPPathTopologies/OSPFtest10/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest11/*_m.cc src/nonARPPathTopologies/OSPFtest11/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest11/results/*_m.cc src/nonARPPathTopologies/OSPFtest11/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest12/*_m.cc src/nonARPPathTopologies/OSPFtest12/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest12/results/*_m.cc src/nonARPPathTopologies/OSPFtest12/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest13/*_m.cc src/nonARPPathTopologies/OSPFtest13/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest13/results/*_m.cc src/nonARPPathTopologies/OSPFtest13/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest2/*_m.cc src/nonARPPathTopologies/OSPFtest2/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest2/results/*_m.cc src/nonARPPathTopologies/OSPFtest2/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest3/*_m.cc src/nonARPPathTopologies/OSPFtest3/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest3/results/*_m.cc src/nonARPPathTopologies/OSPFtest3/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest4/*_m.cc src/nonARPPathTopologies/OSPFtest4/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest4/results/*_m.cc src/nonARPPathTopologies/OSPFtest4/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest5/*_m.cc src/nonARPPathTopologies/OSPFtest5/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest5/results/*_m.cc src/nonARPPathTopologies/OSPFtest5/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest6/*_m.cc src/nonARPPathTopologies/OSPFtest6/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest6/results/*_m.cc src/nonARPPathTopologies/OSPFtest6/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest7/*_m.cc src/nonARPPathTopologies/OSPFtest7/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest7/results/*_m.cc src/nonARPPathTopologies/OSPFtest7/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest8/*_m.cc src/nonARPPathTopologies/OSPFtest8/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest8/results/*_m.cc src/nonARPPathTopologies/OSPFtest8/results/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest9/*_m.cc src/nonARPPathTopologies/OSPFtest9/*_m.h
	-rm -f src/nonARPPathTopologies/OSPFtest9/results/*_m.cc src/nonARPPathTopologies/OSPFtest9/results/*_m.h
	-rm -f src/nonARPPathTopologies/Topology3/*_m.cc src/nonARPPathTopologies/Topology3/*_m.h
	-rm -f src/nonARPPathTopologies/Topology3/results/*_m.cc src/nonARPPathTopologies/Topology3/results/*_m.h
	-rm -f src/topologies/*_m.cc src/topologies/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc confs/*.cc results/*.cc src/*.cc src/applications/*.cc src/applications/udpapp/*.cc src/linklayer/*.cc src/linklayer/ARPPathSwitch/*.cc src/linklayer/ethernet/*.cc src/networklayer/*.cc src/networklayer/arp/*.cc src/networklayer/ipv4/*.cc src/nodes/*.cc src/nodes/ethernet/*.cc src/nodes/inet/*.cc src/nonARPPathTopologies/*.cc src/nonARPPathTopologies/OSPFtest/*.cc src/nonARPPathTopologies/OSPFtest/results/*.cc src/nonARPPathTopologies/OSPFtest10/*.cc src/nonARPPathTopologies/OSPFtest10/results/*.cc src/nonARPPathTopologies/OSPFtest11/*.cc src/nonARPPathTopologies/OSPFtest11/results/*.cc src/nonARPPathTopologies/OSPFtest12/*.cc src/nonARPPathTopologies/OSPFtest12/results/*.cc src/nonARPPathTopologies/OSPFtest13/*.cc src/nonARPPathTopologies/OSPFtest13/results/*.cc src/nonARPPathTopologies/OSPFtest2/*.cc src/nonARPPathTopologies/OSPFtest2/results/*.cc src/nonARPPathTopologies/OSPFtest3/*.cc src/nonARPPathTopologies/OSPFtest3/results/*.cc src/nonARPPathTopologies/OSPFtest4/*.cc src/nonARPPathTopologies/OSPFtest4/results/*.cc src/nonARPPathTopologies/OSPFtest5/*.cc src/nonARPPathTopologies/OSPFtest5/results/*.cc src/nonARPPathTopologies/OSPFtest6/*.cc src/nonARPPathTopologies/OSPFtest6/results/*.cc src/nonARPPathTopologies/OSPFtest7/*.cc src/nonARPPathTopologies/OSPFtest7/results/*.cc src/nonARPPathTopologies/OSPFtest8/*.cc src/nonARPPathTopologies/OSPFtest8/results/*.cc src/nonARPPathTopologies/OSPFtest9/*.cc src/nonARPPathTopologies/OSPFtest9/results/*.cc src/nonARPPathTopologies/Topology3/*.cc src/nonARPPathTopologies/Topology3/results/*.cc src/topologies/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/applications/udpapp/UDPBasicAppNew.o: src/applications/udpapp/UDPBasicAppNew.cc \
	$(INET_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	src/applications/udpapp/UDPBasicAppNew.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/applications/udpapp/UDPAppBase.h
$O/src/linklayer/ARPPathSwitch/PathRepair_m.o: src/linklayer/ARPPathSwitch/PathRepair_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	src/linklayer/ARPPathSwitch/PathRepair_m.h
$O/src/linklayer/ARPPathSwitch/MACRelayUnitAPB.o: src/linklayer/ARPPathSwitch/MACRelayUnitAPB.cc \
	src/linklayer/ARPPathSwitch/MACRelayUnitAPB.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/etherswitch/MACRelayUnitBase.h \
	src/linklayer/ARPPathSwitch/PathRepair_m.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h
$O/src/linklayer/ethernet/EtherMACNew.o: src/linklayer/ethernet/EtherMACNew.cc \
	$(INET_PROJ)/src/base/IPassiveQueue.h \
	$(INET_PROJ)/src/linklayer/contract/TxNotifDetails.h \
	src/linklayer/ethernet/EtherMACNew.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	src/linklayer/ethernet/EtherMACBaseNew.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h
$O/src/linklayer/ethernet/EtherMACBaseNew.o: src/linklayer/ethernet/EtherMACBaseNew.cc \
	$(INET_PROJ)/src/base/IPassiveQueue.h \
	$(INET_PROJ)/src/linklayer/contract/TxNotifDetails.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	src/linklayer/ethernet/EtherMACBaseNew.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h
$O/src/networklayer/arp/ARPnew.o: src/networklayer/arp/ARPnew.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/arp/ARPPacket_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	src/networklayer/arp/ARPnew.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h
$O/src/networklayer/ipv4/IPNew.o: src/networklayer/ipv4/IPNew.cc \
	$(INET_PROJ)/src/networklayer/ipv4/IPFragBuf.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	src/networklayer/ipv4/IPNew.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/base/QueueBase.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram_m.h \
	$(INET_PROJ)/src/base/ProtocolMap.h \
	$(INET_PROJ)/src/base/AbstractQueue.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/arp/ARPPacket_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMP.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPAccess.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/base/ReassemblyBuffer.h

