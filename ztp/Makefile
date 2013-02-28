TARGET=netsim_app
OBJECTS=main.o ztpHost.o ztpRouter.o ztpPacket.o app_config.o cookie_class.o
LIBRARY=../netsim/libnetsim.a
SOURCES=main.cpp ztpHost.cpp ztpRouter.cpp ztpPacket.cpp app_config.cpp cookie_class.cpp
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
ztpHost.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
ztpHost.o: /usr/include/alloca.h ../netsim/Node.h ../netsim/FIFONode.h
ztpHost.o: ../netsim/Packet.h ztpPacket.h ../netsim/Timer.h
ztpHost.o: ../netsim/PacketScheduler.h ../netsim/Scheduler.h ztpHost.h
ztpHost.o: new_library.h /usr/include/string.h /usr/include/xlocale.h
ztpHost.o: cookie_class.h
ztpRouter.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
ztpRouter.o: /usr/include/alloca.h ../netsim/Node.h ../netsim/FIFONode.h
ztpRouter.o: ../netsim/Packet.h ztpPacket.h ../netsim/Timer.h
ztpRouter.o: ../netsim/PacketScheduler.h ../netsim/Scheduler.h ztpHost.h
ztpRouter.o: new_library.h ztpRouter.h
ztpPacket.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
ztpPacket.o: /usr/include/alloca.h ../netsim/Node.h ../netsim/FIFONode.h
ztpPacket.o: ../netsim/Packet.h ztpPacket.h ../netsim/Timer.h
ztpPacket.o: ../netsim/PacketScheduler.h ../netsim/Scheduler.h
ztpPacket.o: /usr/include/string.h /usr/include/xlocale.h
app_config.o: /usr/include/string.h /usr/include/features.h
app_config.o: /usr/include/xlocale.h ../netsim/common.h /usr/include/stdlib.h
app_config.o: /usr/include/alloca.h ../netsim/Config.h ../netsim/Node.h
app_config.o: ../netsim/FIFONode.h ../netsim/Packet.h ztpHost.h new_library.h
app_config.o: ../netsim/PacketScheduler.h ztpPacket.h ../netsim/Timer.h
app_config.o: ../netsim/Scheduler.h ztpRouter.h
cookie_class.o: cookie_class.h
