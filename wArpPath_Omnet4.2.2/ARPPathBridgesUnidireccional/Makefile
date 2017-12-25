#
# OMNeT++/OMNEST Makefile for ARPPathBridgesUnidireccional
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I../inet/src/linklayer/ieee80211/radio -I../inet/src/linklayer/ethernet -I../inet/src/networklayer/common -I../inet/src -I../inet/src/networklayer/icmpv6 -I../inet/src/world/obstacles -I../inet/src/networklayer/xmipv6 -I../inet/src/networklayer/contract -I../inet/src/mobility -I../inet/src/networklayer/manetrouting/base -I../inet/src/linklayer/ieee80211/mgmt -I../inet/src/util -I../inet/src/transport/contract -I../inet/src/linklayer/radio/propagation -I../inet/src/linklayer/ieee80211/radio/errormodel -I../inet/src/world/powercontrol -I../inet/src/linklayer/radio -I../inet/src/util/headerserializers/tcp -I../libgist/src/networklayer/arp -I../inet/src/networklayer/ipv4 -I../inet/src/util/headerserializers/ipv4 -I../inet/src/base -I../inet/src/util/headerserializers -I../inet/src/world/radio -I../inet/src/linklayer/ieee80211/mac -I../inet/src/networklayer/ipv6 -I../inet/src/transport/sctp -I../inet/src/util/headerserializers/udp -I../inet/src/networklayer/ipv6tunneling -I../inet/src/battery/models -I../inet/src/applications/pingapp -I../inet/src/util/headerserializers/sctp -I../inet/src/linklayer/contract -I../libgist/src/linklayer/ARPPathSwitch -I../inet/src/transport/tcp_common -I../inet/src/networklayer/arp -I../inet/src/transport/udp -I../inet/src/linklayer/ethernet/switch -L../libgist/out/$(CONFIGNAME) -L../inet/out/$(CONFIGNAME)/src -llibgist -linet -DINET_IMPORT -KLIBGIST_PROJ=../libgist -KINET_PROJ=../inet
#

# Name of target to be created (-o option)
TARGET = ARPPathBridgesUnidireccional$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../inet/src/linklayer/ieee80211/radio \
    -I../inet/src/linklayer/ethernet \
    -I../inet/src/networklayer/common \
    -I../inet/src \
    -I../inet/src/networklayer/icmpv6 \
    -I../inet/src/world/obstacles \
    -I../inet/src/networklayer/xmipv6 \
    -I../inet/src/networklayer/contract \
    -I../inet/src/mobility \
    -I../inet/src/networklayer/manetrouting/base \
    -I../inet/src/linklayer/ieee80211/mgmt \
    -I../inet/src/util \
    -I../inet/src/transport/contract \
    -I../inet/src/linklayer/radio/propagation \
    -I../inet/src/linklayer/ieee80211/radio/errormodel \
    -I../inet/src/world/powercontrol \
    -I../inet/src/linklayer/radio \
    -I../inet/src/util/headerserializers/tcp \
    -I../libgist/src/networklayer/arp \
    -I../inet/src/networklayer/ipv4 \
    -I../inet/src/util/headerserializers/ipv4 \
    -I../inet/src/base \
    -I../inet/src/util/headerserializers \
    -I../inet/src/world/radio \
    -I../inet/src/linklayer/ieee80211/mac \
    -I../inet/src/networklayer/ipv6 \
    -I../inet/src/transport/sctp \
    -I../inet/src/util/headerserializers/udp \
    -I../inet/src/networklayer/ipv6tunneling \
    -I../inet/src/battery/models \
    -I../inet/src/applications/pingapp \
    -I../inet/src/util/headerserializers/sctp \
    -I../inet/src/linklayer/contract \
    -I../libgist/src/linklayer/ARPPathSwitch \
    -I../inet/src/transport/tcp_common \
    -I../inet/src/networklayer/arp \
    -I../inet/src/transport/udp \
    -I../inet/src/linklayer/ethernet/switch \
    -I. \
    -Iconfs \
    -Iconfs/dataCenter2L4CGenerator \
    -Iconfs/dataCenter2L4CGenerator/results \
    -Iconfs/dataCenterPortland \
    -Iconfs/dataCenterPortland/results \
    -Iconfs/gridLoadGenerator \
    -Iconfs/gridLoadGenerator/results \
    -Iconfs/latencyComparative \
    -Iconfs/latencyComparative/results \
    -Iconfs/results \
    -Iconfs/test \
    -Iconfs/test/results \
    -Isrc \
    -Isrc/linklayer \
    -Isrc/linklayer/ARPPathSwitch \
    -Isrc/linklayer/Ieee80211 \
    -Isrc/linklayer/Ieee80211/mgmt \
    -Isrc/nodes \
    -Isrc/nodes/ethernet \
    -Isrc/nodes/wireless \
    -Isrc/topologies \
    -Isrc/topologies/dataCenter2L4CGenerator \
    -Isrc/topologies/dataCenterPortland \
    -Isrc/topologies/gridLoadGenerator \
    -Isrc/topologies/latencyComparative

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../libgist/out/$(CONFIGNAME) -L../inet/out/$(CONFIGNAME)/src  -llibgist -linet
LIBS += -Wl,-rpath,`abspath ../libgist/out/$(CONFIGNAME)` -Wl,-rpath,`abspath ../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/src/linklayer/ARPPathSwitch/MACRelayUnitAPB.o $O/src/linklayer/ARPPathSwitch/MACRelayUnitAPBw.o $O/src/linklayer/Ieee80211/mgmt/Ieee80211MgmtAdhocAPB.o $O/src/linklayer/ARPPathSwitch/PathRepair_m.o

# Message files
MSGFILES = \
    src/linklayer/ARPPathSwitch/PathRepair.msg

# Other makefile variables (-K)
LIBGIST_PROJ=../libgist
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
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

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
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

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
	-rm -f ARPPathBridgesUnidireccional ARPPathBridgesUnidireccional.exe libARPPathBridgesUnidireccional.so libARPPathBridgesUnidireccional.a libARPPathBridgesUnidireccional.dll libARPPathBridgesUnidireccional.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f confs/*_m.cc confs/*_m.h
	-rm -f confs/dataCenter2L4CGenerator/*_m.cc confs/dataCenter2L4CGenerator/*_m.h
	-rm -f confs/dataCenter2L4CGenerator/results/*_m.cc confs/dataCenter2L4CGenerator/results/*_m.h
	-rm -f confs/dataCenterPortland/*_m.cc confs/dataCenterPortland/*_m.h
	-rm -f confs/dataCenterPortland/results/*_m.cc confs/dataCenterPortland/results/*_m.h
	-rm -f confs/gridLoadGenerator/*_m.cc confs/gridLoadGenerator/*_m.h
	-rm -f confs/gridLoadGenerator/results/*_m.cc confs/gridLoadGenerator/results/*_m.h
	-rm -f confs/latencyComparative/*_m.cc confs/latencyComparative/*_m.h
	-rm -f confs/latencyComparative/results/*_m.cc confs/latencyComparative/results/*_m.h
	-rm -f confs/results/*_m.cc confs/results/*_m.h
	-rm -f confs/test/*_m.cc confs/test/*_m.h
	-rm -f confs/test/results/*_m.cc confs/test/results/*_m.h
	-rm -f src/*_m.cc src/*_m.h
	-rm -f src/linklayer/*_m.cc src/linklayer/*_m.h
	-rm -f src/linklayer/ARPPathSwitch/*_m.cc src/linklayer/ARPPathSwitch/*_m.h
	-rm -f src/linklayer/Ieee80211/*_m.cc src/linklayer/Ieee80211/*_m.h
	-rm -f src/linklayer/Ieee80211/mgmt/*_m.cc src/linklayer/Ieee80211/mgmt/*_m.h
	-rm -f src/nodes/*_m.cc src/nodes/*_m.h
	-rm -f src/nodes/ethernet/*_m.cc src/nodes/ethernet/*_m.h
	-rm -f src/nodes/wireless/*_m.cc src/nodes/wireless/*_m.h
	-rm -f src/topologies/*_m.cc src/topologies/*_m.h
	-rm -f src/topologies/dataCenter2L4CGenerator/*_m.cc src/topologies/dataCenter2L4CGenerator/*_m.h
	-rm -f src/topologies/dataCenterPortland/*_m.cc src/topologies/dataCenterPortland/*_m.h
	-rm -f src/topologies/gridLoadGenerator/*_m.cc src/topologies/gridLoadGenerator/*_m.h
	-rm -f src/topologies/latencyComparative/*_m.cc src/topologies/latencyComparative/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc confs/*.cc confs/dataCenter2L4CGenerator/*.cc confs/dataCenter2L4CGenerator/results/*.cc confs/dataCenterPortland/*.cc confs/dataCenterPortland/results/*.cc confs/gridLoadGenerator/*.cc confs/gridLoadGenerator/results/*.cc confs/latencyComparative/*.cc confs/latencyComparative/results/*.cc confs/results/*.cc confs/test/*.cc confs/test/results/*.cc src/*.cc src/linklayer/*.cc src/linklayer/ARPPathSwitch/*.cc src/linklayer/Ieee80211/*.cc src/linklayer/Ieee80211/mgmt/*.cc src/nodes/*.cc src/nodes/ethernet/*.cc src/nodes/wireless/*.cc src/topologies/*.cc src/topologies/dataCenter2L4CGenerator/*.cc src/topologies/dataCenterPortland/*.cc src/topologies/gridLoadGenerator/*.cc src/topologies/latencyComparative/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/linklayer/ARPPathSwitch/MACRelayUnitAPB.o: src/linklayer/ARPPathSwitch/MACRelayUnitAPB.cc \
	src/linklayer/ARPPathSwitch/MACRelayUnitAPB.h \
	src/linklayer/ARPPathSwitch/PathRepair_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/linklayer/ethernet/switch/MACRelayUnitBase.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/util/uint128.h \
	$(LIBGIST_PROJ)/src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	$(LIBGIST_PROJ)/src/networklayer/arp/ARPPacketRoute_m.h
$O/src/linklayer/ARPPathSwitch/MACRelayUnitAPBw.o: src/linklayer/ARPPathSwitch/MACRelayUnitAPBw.cc \
	src/linklayer/ARPPathSwitch/MACRelayUnitAPBw.h \
	src/linklayer/ARPPathSwitch/PathRepair_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/linklayer/ethernet/switch/MACRelayUnitBase.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/util/uint128.h \
	$(LIBGIST_PROJ)/src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	$(LIBGIST_PROJ)/src/networklayer/arp/ARPPacketRoute_m.h
$O/src/linklayer/ARPPathSwitch/PathRepair_m.o: src/linklayer/ARPPathSwitch/PathRepair_m.cc \
	src/linklayer/ARPPathSwitch/PathRepair_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h
$O/src/linklayer/Ieee80211/mgmt/Ieee80211MgmtAdhocAPB.o: src/linklayer/Ieee80211/mgmt/Ieee80211MgmtAdhocAPB.cc \
	src/linklayer/ARPPathSwitch/MACRelayUnitAPBw.h \
	src/linklayer/ARPPathSwitch/PathRepair_m.h \
	src/linklayer/Ieee80211/mgmt/Ieee80211MgmtAdhocAPB.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/IPassiveQueue.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/base/PassiveQueueBase.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/linklayer/ethernet/switch/MACRelayUnitBase.h \
	$(INET_PROJ)/src/linklayer/ieee80211/mac/Ieee80211Consts.h \
	$(INET_PROJ)/src/linklayer/ieee80211/mac/Ieee80211Frame_m.h \
	$(INET_PROJ)/src/linklayer/ieee80211/mgmt/Ieee80211MgmtBase.h \
	$(INET_PROJ)/src/linklayer/ieee80211/mgmt/Ieee80211MgmtFrames_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/util/uint128.h \
	$(LIBGIST_PROJ)/src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	$(LIBGIST_PROJ)/src/networklayer/arp/ARPPacketRoute_m.h

