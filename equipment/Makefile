include ../env.mak
CXXFLAGS := -Wall -g -std=c++11 -pthread -I../tools -I../Communication -I../include -I../vendor/jsoncpp/include -I../vendor/libevent-2.0.22-stable/include -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -O3

TARGET = libequipment.a

#OBJS += scan.o Scanner.o CodePairing.o
OBJS += Scanner.o CodePairing.o ElecBalance.o SolderCtrl.o Sequence.o equipment.o
all : $(TARGET)

$(TARGET): $(OBJS)
	$(AR) cr $@ $^

clean:
	rm -f *.o  $(TARGET)

.PHONY: all clean
sinclude ../rules.mak
