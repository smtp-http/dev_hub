include ../env.mak


CXXFLAGS := -Wall -g -std=c++11 -pthread -I../vendor/libfins/include -I../vendor/jsoncpp/include  -I../vendor/curl/include  -I../vendor/libevent-2.0.22-stable/include  -I../transcation -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -O3

TARGET = libconn.a

OBJS += connbaseimp.o eventlooperimp.o TcpServer.o UnixSockServer.o CommServer.o session.o Http.o TcpClient.o 
all : $(TARGET)

$(TARGET): $(OBJS)
	$(AR) cr $@ $^

clean:
	rm -f *.o  $(TARGET)

.PHONY: all clean
sinclude ../rules.mak
