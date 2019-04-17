include ../env.mak
#INCLUDE_DIR = -I$(TOPDIR)/device
CXXFLAGS := -Wall -g -std=c++11 -pthread $(INCLUDE_DIR) -I../vendor/jsoncpp/include -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -O3
CFLAGS := -Wall -g  $(INCLUDE_DIR)

TARGET = libtools.a

#OBJS += ut_misc.o ut_log.o ut_signal.o ut_sds.o ut_title.o
OBJS += ThreadPool.o ut_misc.o ut_log.o ut_signal.o ut_sds.o ut_title.o config.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o 
all : $(TARGET)

$(TARGET): $(OBJS)
	$(AR) cr $@ $^

clean:
	rm -f *.o  $(TARGET)

.PHONY: all clean
sinclude ../rules.mak
