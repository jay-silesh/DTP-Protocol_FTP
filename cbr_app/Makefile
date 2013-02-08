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

main.o: ../netsim/common.h /usr/include/sys/types.h
main.o: /usr/include/sys/appleapiopts.h /usr/include/sys/cdefs.h
main.o: /usr/include/machine/types.h /usr/include/i386/types.h
main.o: /usr/include/i386/_types.h /usr/include/sys/_types.h
main.o: /usr/include/machine/_types.h /usr/include/machine/endian.h
main.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
main.o: /usr/include/libkern/_OSByteOrder.h
main.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/sys/_structs.h
main.o: /usr/include/stdlib.h /usr/include/Availability.h
main.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
main.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
main.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
main.o: /usr/include/i386/_structs.h /usr/include/sys/resource.h
main.o: /usr/include/alloca.h ../netsim/Config.h ../netsim/Packet.h
main.o: ../netsim/Timer.h ../netsim/PacketScheduler.h ../netsim/Scheduler.h
main.o: ../netsim/Topology.h /usr/include/unistd.h /usr/include/sys/unistd.h
main.o: /usr/include/sys/select.h /usr/include/sys/_select.h
CbrSender.o: ../netsim/common.h /usr/include/sys/types.h
CbrSender.o: /usr/include/sys/appleapiopts.h /usr/include/sys/cdefs.h
CbrSender.o: /usr/include/machine/types.h /usr/include/i386/types.h
CbrSender.o: /usr/include/i386/_types.h /usr/include/sys/_types.h
CbrSender.o: /usr/include/machine/_types.h /usr/include/machine/endian.h
CbrSender.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
CbrSender.o: /usr/include/libkern/_OSByteOrder.h
CbrSender.o: /usr/include/libkern/i386/_OSByteOrder.h
CbrSender.o: /usr/include/sys/_structs.h /usr/include/stdlib.h
CbrSender.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
CbrSender.o: /usr/include/_types.h /usr/include/sys/wait.h
CbrSender.o: /usr/include/sys/signal.h /usr/include/machine/signal.h
CbrSender.o: /usr/include/i386/signal.h /usr/include/i386/_structs.h
CbrSender.o: /usr/include/sys/resource.h /usr/include/alloca.h
CbrSender.o: ../netsim/Node.h ../netsim/FIFONode.h ../netsim/Packet.h
CbrSender.o: CbrPacket.h ../netsim/Timer.h ../netsim/PacketScheduler.h
CbrSender.o: ../netsim/Scheduler.h CbrSender.h
CbrReceiver.o: ../netsim/common.h /usr/include/sys/types.h
CbrReceiver.o: /usr/include/sys/appleapiopts.h /usr/include/sys/cdefs.h
CbrReceiver.o: /usr/include/machine/types.h /usr/include/i386/types.h
CbrReceiver.o: /usr/include/i386/_types.h /usr/include/sys/_types.h
CbrReceiver.o: /usr/include/machine/_types.h /usr/include/machine/endian.h
CbrReceiver.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
CbrReceiver.o: /usr/include/libkern/_OSByteOrder.h
CbrReceiver.o: /usr/include/libkern/i386/_OSByteOrder.h
CbrReceiver.o: /usr/include/sys/_structs.h /usr/include/stdlib.h
CbrReceiver.o: /usr/include/Availability.h
CbrReceiver.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
CbrReceiver.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
CbrReceiver.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
CbrReceiver.o: /usr/include/i386/_structs.h /usr/include/sys/resource.h
CbrReceiver.o: /usr/include/alloca.h ../netsim/Node.h ../netsim/FIFONode.h
CbrReceiver.o: ../netsim/Packet.h CbrPacket.h ../netsim/PacketScheduler.h
CbrReceiver.o: ../netsim/Timer.h ../netsim/Scheduler.h CbrReceiver.h
CbrPacket.o: ../netsim/common.h /usr/include/sys/types.h
CbrPacket.o: /usr/include/sys/appleapiopts.h /usr/include/sys/cdefs.h
CbrPacket.o: /usr/include/machine/types.h /usr/include/i386/types.h
CbrPacket.o: /usr/include/i386/_types.h /usr/include/sys/_types.h
CbrPacket.o: /usr/include/machine/_types.h /usr/include/machine/endian.h
CbrPacket.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
CbrPacket.o: /usr/include/libkern/_OSByteOrder.h
CbrPacket.o: /usr/include/libkern/i386/_OSByteOrder.h
CbrPacket.o: /usr/include/sys/_structs.h /usr/include/stdlib.h
CbrPacket.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
CbrPacket.o: /usr/include/_types.h /usr/include/sys/wait.h
CbrPacket.o: /usr/include/sys/signal.h /usr/include/machine/signal.h
CbrPacket.o: /usr/include/i386/signal.h /usr/include/i386/_structs.h
CbrPacket.o: /usr/include/sys/resource.h /usr/include/alloca.h
CbrPacket.o: ../netsim/Node.h ../netsim/FIFONode.h ../netsim/Packet.h
CbrPacket.o: CbrPacket.h ../netsim/Timer.h ../netsim/PacketScheduler.h
CbrPacket.o: ../netsim/Scheduler.h
app_config.o: /usr/include/string.h /usr/include/_types.h
app_config.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
app_config.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
app_config.o: /usr/include/secure/_string.h /usr/include/secure/_common.h
app_config.o: ../netsim/common.h /usr/include/sys/types.h
app_config.o: /usr/include/sys/appleapiopts.h /usr/include/machine/types.h
app_config.o: /usr/include/i386/types.h /usr/include/machine/endian.h
app_config.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
app_config.o: /usr/include/libkern/_OSByteOrder.h
app_config.o: /usr/include/libkern/i386/_OSByteOrder.h
app_config.o: /usr/include/sys/_structs.h /usr/include/stdlib.h
app_config.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
app_config.o: /usr/include/sys/wait.h /usr/include/sys/signal.h
app_config.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
app_config.o: /usr/include/i386/_structs.h /usr/include/sys/resource.h
app_config.o: /usr/include/alloca.h ../netsim/Config.h ../netsim/Node.h
app_config.o: ../netsim/FIFONode.h ../netsim/Packet.h CbrSender.h
app_config.o: CbrReceiver.h
