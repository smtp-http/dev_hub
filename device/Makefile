include ../env.mak

CXXFLAGS := -Wall -g -std=c++11 -pthread -I../tools -I../equipment -I../Communication  -I../vendor/jsoncpp/include -I../include -I../vendor/libevent-2.0.22-stable/include  -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -O3


TARGET = libdevice.a

OBJS += DeviceMonitor.o Uart.o Usb.o Peripheral.o Device.o
all : $(TARGET)

$(TARGET): $(OBJS)
	$(AR) cr $@ $^

clean:
	rm -f *.o  $(TARGET)

.PHONY: all clean
sinclude ../rules.mak
