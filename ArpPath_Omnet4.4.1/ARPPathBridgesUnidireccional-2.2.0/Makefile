#
# OMNeT++/OMNEST Makefile for ARPPathBridgesUnidireccional-2.2.0
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/linklayer/ethernet -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/common -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/icmpv6 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/xmipv6 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/contract -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/autorouting/ipv4 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/transport/contract -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/linklayer/common -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/status -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers/tcp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/ipv4 -I../libgist-2.2.0/src/networklayer/arp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/mobility/contract -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers/ipv4 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/base -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/ipv6 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/transport/sctp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers/udp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/ipv6tunneling -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers/ipv6 -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/applications/pingapp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/util/headerserializers/sctp -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/linklayer/contract -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/networklayer/arp -I../libgist-2.2.0/src/linklayer/ARPPathSwitch -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/transport/tcp_common -I/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/src/transport/udp -L/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0/out/$$\(CONFIGNAME\)/src -L../libgist-2.2.0/out/$$\(CONFIGNAME\) -linet -llibgist -DINET_IMPORT -KINET_PROJ=/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0 -KLIBGIST_PROJ=../libgist-2.2.0
#

# Name of target to be created (-o option)
TARGET = ARPPathBridgesUnidireccional-2.2.0$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I$(INET_PROJ)/src/linklayer/ethernet \
    -I$(INET_PROJ)/src/networklayer/common \
    -I$(INET_PROJ)/src/networklayer/icmpv6 \
    -I$(INET_PROJ)/src/networklayer/xmipv6 \
    -I$(INET_PROJ)/src/networklayer/contract \
    -I$(INET_PROJ)/src/networklayer/autorouting/ipv4 \
    -I$(INET_PROJ)/src/util \
    -I$(INET_PROJ)/src/transport/contract \
    -I$(INET_PROJ)/src/linklayer/common \
    -I$(INET_PROJ)/src/status \
    -I$(INET_PROJ)/src/util/headerserializers/tcp \
    -I$(INET_PROJ)/src/networklayer/ipv4 \
    -I../libgist-2.2.0/src/networklayer/arp \
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
    -I$(INET_PROJ)/src/util/headerserializers/sctp \
    -I$(INET_PROJ)/src/linklayer/contract \
    -I$(INET_PROJ)/src/networklayer/arp \
    -I../libgist-2.2.0/src/linklayer/ARPPathSwitch \
    -I$(INET_PROJ)/src/transport/tcp_common \
    -I$(INET_PROJ)/src/transport/udp \
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
    -Iconfs/test \
    -Iconfs/test/results \
    -Isrc \
    -Isrc/linklayer \
    -Isrc/linklayer/ARPPathSwitch \
    -Isrc/nodes \
    -Isrc/nodes/ethernet \
    -Isrc/topologies \
    -Isrc/topologies/dataCenter2L4CGenerator \
    -Isrc/topologies/dataCenterPortland \
    -Isrc/topologies/gridLoadGenerator \
    -Isrc/topologies/latencyComparative

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L$(INET_PROJ)/out/$(CONFIGNAME)/src -L../libgist-2.2.0/out/$(CONFIGNAME)  -linet -llibgist
LIBS += -Wl,-rpath,`abspath $(INET_PROJ)/out/$(CONFIGNAME)/src` -Wl,-rpath,`abspath ../libgist-2.2.0/out/$(CONFIGNAME)`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = $O/src/linklayer/ARPPathSwitch/MACRelayUnitAPB.o $O/src/linklayer/ARPPathSwitch/PathRepair_m.o

# Message files
MSGFILES = \
    src/linklayer/ARPPathSwitch/PathRepair.msg

# Other makefile variables (-K)
INET_PROJ=/Volumes/MAC_HDD_Data/imyelmo/Documents/svn/inet/inet-2.3.0
LIBGIST_PROJ=../libgist-2.2.0

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
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

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
	$(Q)-rm -f ARPPathBridgesUnidireccional-2.2.0 ARPPathBridgesUnidireccional-2.2.0.exe libARPPathBridgesUnidireccional-2.2.0.so libARPPathBridgesUnidireccional-2.2.0.a libARPPathBridgesUnidireccional-2.2.0.dll libARPPathBridgesUnidireccional-2.2.0.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f confs/*_m.cc confs/*_m.h
	$(Q)-rm -f confs/dataCenter2L4CGenerator/*_m.cc confs/dataCenter2L4CGenerator/*_m.h
	$(Q)-rm -f confs/dataCenter2L4CGenerator/results/*_m.cc confs/dataCenter2L4CGenerator/results/*_m.h
	$(Q)-rm -f confs/dataCenterPortland/*_m.cc confs/dataCenterPortland/*_m.h
	$(Q)-rm -f confs/dataCenterPortland/results/*_m.cc confs/dataCenterPortland/results/*_m.h
	$(Q)-rm -f confs/gridLoadGenerator/*_m.cc confs/gridLoadGenerator/*_m.h
	$(Q)-rm -f confs/gridLoadGenerator/results/*_m.cc confs/gridLoadGenerator/results/*_m.h
	$(Q)-rm -f confs/latencyComparative/*_m.cc confs/latencyComparative/*_m.h
	$(Q)-rm -f confs/latencyComparative/results/*_m.cc confs/latencyComparative/results/*_m.h
	$(Q)-rm -f confs/test/*_m.cc confs/test/*_m.h
	$(Q)-rm -f confs/test/results/*_m.cc confs/test/results/*_m.h
	$(Q)-rm -f src/*_m.cc src/*_m.h
	$(Q)-rm -f src/linklayer/*_m.cc src/linklayer/*_m.h
	$(Q)-rm -f src/linklayer/ARPPathSwitch/*_m.cc src/linklayer/ARPPathSwitch/*_m.h
	$(Q)-rm -f src/nodes/*_m.cc src/nodes/*_m.h
	$(Q)-rm -f src/nodes/ethernet/*_m.cc src/nodes/ethernet/*_m.h
	$(Q)-rm -f src/topologies/*_m.cc src/topologies/*_m.h
	$(Q)-rm -f src/topologies/dataCenter2L4CGenerator/*_m.cc src/topologies/dataCenter2L4CGenerator/*_m.h
	$(Q)-rm -f src/topologies/dataCenterPortland/*_m.cc src/topologies/dataCenterPortland/*_m.h
	$(Q)-rm -f src/topologies/gridLoadGenerator/*_m.cc src/topologies/gridLoadGenerator/*_m.h
	$(Q)-rm -f src/topologies/latencyComparative/*_m.cc src/topologies/latencyComparative/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc confs/*.cc confs/dataCenter2L4CGenerator/*.cc confs/dataCenter2L4CGenerator/results/*.cc confs/dataCenterPortland/*.cc confs/dataCenterPortland/results/*.cc confs/gridLoadGenerator/*.cc confs/gridLoadGenerator/results/*.cc confs/latencyComparative/*.cc confs/latencyComparative/results/*.cc confs/test/*.cc confs/test/results/*.cc src/*.cc src/linklayer/*.cc src/linklayer/ARPPathSwitch/*.cc src/nodes/*.cc src/nodes/ethernet/*.cc src/topologies/*.cc src/topologies/dataCenter2L4CGenerator/*.cc src/topologies/dataCenterPortland/*.cc src/topologies/gridLoadGenerator/*.cc src/topologies/latencyComparative/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/linklayer/ARPPathSwitch/MACRelayUnitAPB.o: src/linklayer/ARPPathSwitch/MACRelayUnitAPB.cc \
	src/linklayer/ARPPathSwitch/MACRelayUnitAPB.h \
	src/linklayer/ARPPathSwitch/PathRepair_m.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(LIBGIST_PROJ)/src/linklayer/ARPPathSwitch/MACRelayUnitRoute.h \
	$(LIBGIST_PROJ)/src/networklayer/arp/ARPPacketRoute_m.h
$O/src/linklayer/ARPPathSwitch/PathRepair_m.o: src/linklayer/ARPPathSwitch/PathRepair_m.cc \
	src/linklayer/ARPPathSwitch/PathRepair_m.h \
	$(INET_PROJ)/src/base/Compat.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_PROJ)/src/linklayer/ethernet/Ethernet.h

