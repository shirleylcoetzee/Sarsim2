CC = g++
FLAGS = -O2 -Wall
LIBS = -lm
EXECUTABLES = sarsim2
RM = rm -f 

all : sarsim2

sarsim2 :  sarsim2.o loadsavedata.o engine.o general.o linkedlist.o parser.o 
	$(CC) -o sarsim2 sarsim2.o loadsavedata.o engine.o general.o linkedlist.o parser.o $(LIBS)

sarsim2.o : sarsim2.cc objectstructdef.h globalvariables.h 
	$(CC) -c sarsim2.cc $(FLAGS) $(INCLUDE)

engine.o : engine.cpp engine.h objectstructdef.h globalvariables.h
	$(CC) -c engine.cpp $(FLAGS) $(INCLUDE)

general.o : general.cpp general.h objectstructdef.h globalvariables.h
	$(CC) -c general.cpp $(FLAGS) $(INCLUDE)

linkedlist.o : linkedlist.cpp linkedlist.h objectstructdef.h globalvariables.h
	$(CC) -c linkedlist.cpp $(FLAGS) $(INCLUDE)

parser.o : parser.cpp parser.h objectstructdef.h globalvariables.h
	$(CC) -c parser.cpp $(FLAGS) $(INCLUDE)

loadsavedata.o : loadsavedata.cpp loadsavedata.h objectstructdef.h globalvariables.h
	$(CC) -c loadsavedata.cpp $(FLAGS) $(INCLUDE)

clean:
	$(RM) *.o $(EXECUTABLES) core

