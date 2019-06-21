include env.mak

TOPDIR := $(shell pwd)

LDFLAGS +=  $(TOPDIR)/vendor/$(LIBSDIR)/libevent.a  $(TOPDIR)/vendor/$(LIBSDIR)/libcurl.a $(TOPDIR)/vendor/$(LIBSDIR)/libjsoncpp.a  $(TOPDIR)/vendor/$(LIBSDIR)/libfins.a  #$(TOPDIR)/vendor/$(LIBSDIR)/libssl.a $(TOPDIR)/vendor/$(LIBSDIR)/libcrypto.a $(TOPDIR)/vendor/$(LIBSDIR)/libcares.a $(TOPDIR)/vendor/$(LIBSDIR)/libaprutil-1.a $(TOPDIR)/vendor/$(LIBSDIR)/libapr-1.a  $(TOPDIR)/vendor/$(LIBSDIR)/libmosquitto.a
ifdef TOOLCHAIN_NAME
	#LDFLAGS += $(TOPDIR)/vendor/$(LIBSDIR)/libexpat.a 
else
	LDFLAGS += #-lexpat 
endif

LDFLAGS +=  -lpthread -ldl  -lstdc++ -lrt #,-Bstatic   -lcurl #-lz#-lexpat 
# -std=c++11 -pthread -Wl,--no-as-needed



INCLUDE_DIR = -I$(TOPDIR)/device -I$(TOPDIR)/equipment  -I$(TOPDIR)/include -I$(TOPDIR)/tools -I$(TOPDIR)/transcation -I$(TOPDIR)/Communication -I$(TOPDIR)/command -I$(TOPDIR)/vendor/libevent-2.0.22-stable/include -I$(TOPDIR)/vendor/jsoncpp/include -I$(TOPDIR)/vendor/log4cxx/include -I$(TOPDIR)/vendor/apr/include -I$(TOPDIR)/vendor/apr-util/include #-I$(TOPDIR)/mqtt

################  COMPILE FLAG ########################
#export CFLAGS := -Wall -g -std=c++11 -Wl,--no-as-needed $(INCLUDE_DIR)
export CXXFLAGS := -Wall -g -std=c++11 -D_GLIBCXX_USE_NANOSLEEP -Wl,--no-as-needed $(INCLUDE_DIR) -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -O3

TARGET = dev_collecter
OBJS = Main.o#AppSimulator.o

LOCAL_LIBS :=  ./tools/libtools.a ./Communication/libconn.a ./device/libdevice.a  ./equipment/libequipment.a ./transcation/libtranscation.a $(TOPDIR)/vendor/$(LIBSDIR)/libcurl.a

#./command/libcommand.a  ./mqtt/libmqtt.a 

LIBSCAN = libscanner.a

MODCLI = mod_client
OBCLI = unit-test-client.o

MODSVR = mod_server
OBSVR = unit-test-server.o

all : device  equipment tools Communication mqtt transcation $(TARGET)
modcli : device  equipment tools Communication mqtt transcation $(MODCLI)
modsvr : device  equipment tools Communication mqtt transcation $(MODSVR)
#command
#all : session  inireader $(TARGET)

$(LIBSCAN):$(LOCAL_LIBS)
	$(AR) cr $@ $^

$(TARGET): $(OBJS) $(LIBSCAN)
#$(TARGET): $(LIBSCAN)
	@echo "==== target ======"
	@echo "Linking $@"
#	@$(CXX) -o $@ $(LOCAL_LIBS) $(LDFLAGS)
	@$(CXX) -o $@ $(OBJS) $(LOCAL_LIBS) $(LDFLAGS)

$(MODCLI): $(OBCLI) $(LIBSCAN)
#$(TARGET): $(LIBSCAN)
	@echo "==== target ======"
	@echo "Linking $@"
#	@$(CXX) -o $@ $(LOCAL_LIBS) $(LDFLAGS)
	@$(CC) -o $@ $(OBCLI) $(LOCAL_LIBS) $(LDFLAGS)

$(MODSVR): $(OBSVR) $(LIBSCAN)
#$(TARGET): $(LIBSCAN)
	@echo "==== target ======"
	@echo "Linking $@"
#	@$(CXX) -o $@ $(LOCAL_LIBS) $(LDFLAGS)
	@$(CXX) -o $@ $(OBSVR) $(LOCAL_LIBS) $(LDFLAGS)

device:
	@echo "==== device ======"
	@$(MAKE) -sC $@

#mqtt:
#	@echo "==== mqtt ======"
#	@$(MAKE) -sC $@

equipment:
	@echo "==== equipment ===="
	@$(MAKE) -sC $@

#SolderCtrl:
#	@echo "==== SolderCtrl ===="
#	@$(MAKE) -sC $@

tools:
	@echo "==== tools ===="
	@$(MAKE) -sC $@

Communication:
	@echo "==== Communication ===="
	@$(MAKE) -sC $@

transcation:
	@echo "==== transcation ===="
	@$(MAKE) -sC $@

#command:
#	@echo "==== command ===="
#	@$(MAKE) -sC $@

	
clean:
	$(MAKE) -sC device clean
	$(MAKE) -sC tools clean
#	$(MAKE) -sC mqtt clean
	$(MAKE) -sC Communication clean
	$(MAKE) -sC equipment clean
	$(MAKE) -sC transcation clean
#	$(MAKE) -sC command clean
	
	rm -f *.o $(TARGET) $(LIBSCAN)

distclean: clean
	find -name *.d | xargs rm -f
	
.PHONY: all clean device equipment tools  Communication transcation #command mqtt
include rules.mak
