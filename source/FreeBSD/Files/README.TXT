This file contains instructions on how to compile a linux version from the 
windows source code.

The follwing files are needed from the windows source code:

calcthread.cpp 
calcthread.h 
dest.h  
engine.cpp  
engine.h  
general.cpp  
general.h  
globalvariables.h  
linkedlist.cpp  
linkedlist.h  
loadsavedata.cpp  
loadsavedata.h  
objectstructdef.h  
parser.cpp  
parser.h 

The following file is needed from the linux-specific source code:

sarsim2.cc

For easier compilation a batch file and a make file has been written:

com - compiles all source files and produces executable sarsim2.

(Here is a copy of the file:
gcc -c engine.cpp -O2 -Wall
gcc -c general.cpp -O2 -Wall
gcc -c linkedlist.cpp -O2 -Wall
gcc -c parser.cpp -O2 -Wall
gcc -c loadsavedata.cpp -O2 -Wall
make -k
rm *.o
rm core
rm *.bak
rm *.~h
rm *.~cp
rm *.txt
rm *.cc~
cp sarsim2 ..
cd ..
chmod 750 sarsim2
cd src
chmod 720 *
)

makefile - file needed for compiling 

(Here is a copy of the file:
CC = gcc
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
)


Now to get a linux executable:

1) copy above mentioned files into a directory
2) change dest.h as described in that file (usually only necessary to 
   un-comment '#define COMPILE_FOR_WINDOWS32' line and change version name
3) run com.

A file called sarsim2 should be created.

