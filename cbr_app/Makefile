TARGET=netsim_app
OBJECTS=main.o CbrSender.o CbrReceiver.o CbrPacket.o app_config.o 
LIBRARY=../netsim/libnetsim.a
SOURCES=main.cpp CbrSender.cpp CbrReceiver.cpp CbrPacket.cpp app_config.cpp
CCFLAGS=-g

all: netsim $(TARGET)

netsim:
	(cd ../netsim; make)

$(TARGET): $(OBJECTS) $(LIBRARY)
	g++ $(CCFLAGS) -o $(TARGET) $(OBJECTS) $(LIBRARY)

%.o: %.cpp
	g++ -c $(CCFLAGS) $*.cpp

%.o: %.c
	gcc -c $(CCFLAGS) $*.c

depend: $(SOURCES)
	(cd ../netsim; make depend)
	makedepend $(SOURCES)

clean:
	(cd ../netsim; make clean)
	rm *.o

# DO NOT DELETE

main.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
main.o: /usr/include/alloca.h ../netsim/Config.h ../netsim/Packet.h
main.o: ../netsim/Timer.h ../netsim/PacketScheduler.h ../netsim/Scheduler.h
main.o: ../netsim/Topology.h /usr/include/unistd.h /usr/include/getopt.h
CbrSender.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
CbrSender.o: /usr/include/alloca.h ../netsim/Node.h ../netsim/FIFONode.h
CbrSender.o: ../netsim/Packet.h CbrPacket.h ../netsim/Timer.h
CbrSender.o: ../netsim/PacketScheduler.h ../netsim/Scheduler.h CbrSender.h
CbrSender.o: /usr/include/string.h /usr/include/xlocale.h
CbrReceiver.o: ../netsim/common.h /usr/include/stdlib.h
CbrReceiver.o: /usr/include/features.h /usr/include/alloca.h ../netsim/Node.h
CbrReceiver.o: ../netsim/FIFONode.h ../netsim/Packet.h CbrPacket.h
CbrReceiver.o: ../netsim/PacketScheduler.h ../netsim/Timer.h
CbrReceiver.o: ../netsim/Scheduler.h CbrReceiver.h
CbrPacket.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
CbrPacket.o: /usr/include/alloca.h ../netsim/Node.h ../netsim/FIFONode.h
CbrPacket.o: ../netsim/Packet.h CbrPacket.h ../netsim/Timer.h
CbrPacket.o: ../netsim/PacketScheduler.h ../netsim/Scheduler.h
app_config.o: /usr/include/string.h /usr/include/features.h
app_config.o: /usr/include/xlocale.h ../netsim/common.h /usr/include/stdlib.h
app_config.o: /usr/include/alloca.h ../netsim/Config.h ../netsim/Node.h
app_config.o: ../netsim/FIFONode.h ../netsim/Packet.h CbrSender.h
app_config.o: CbrReceiver.h
