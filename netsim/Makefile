TARGET=libnetsim.a
OBJECTS=Node.o FIFONode.o Scheduler.o PacketScheduler.o Timer.o Topology.o lex.yy.o parser.o Config.o Packet.o
SOURCES=Node.cpp FIFONode.cpp Scheduler.cpp PacketScheduler.cpp Timer.cpp Topology.cpp Packet.cpp
CCFLAGS=-g

all: $(TARGET)

$(TARGET): $(OBJECTS)
	ar cru $(TARGET) $(OBJECTS)

%.o: %.cpp %.h
	g++ -c $(CCFLAGS) $*.cpp

%.o: %.cpp 
	g++ -c $(CCFLAGS) $*.cpp

%.o: %.c
	gcc -c $(CCFLAGS) $*.c

depend: $(SOURCES)
	makedepend $(SOURCES)

parser.cpp: parser.y
	yacc -d parser.y -o parser.cpp

lex.yy.c: lexer.l
	flex lexer.l

clean:
	rm *.o libnetsim.a

# DO NOT DELETE
