include ../env.mak
CXXFLAGS := -Wall -g -std=c++11 -pthread -I../tools -I../Communication -I../device -I../equipment -I../include -I../vendor/jsoncpp/include -I../vendor/libevent-2.0.22-stable/include -I../vendor/log4cxx/include -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -O3

TARGET = libtranscation.a

OBJS += Transcation.o task.o FunctionManager.o PlcFins.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o station.o machine.o 
all : $(TARGET)

$(TARGET): $(OBJS)
	$(AR) cr $@ $^

clean:
	rm -f *.o  $(TARGET)

.PHONY: all clean
sinclude ../rules.mak
