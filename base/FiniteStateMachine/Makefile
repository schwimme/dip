COMPILER=g++ --std=c++14 -I../..
OUTPUT_DIR=../../Build/

BINARIES= $(OUTPUT_DIR)bin/
OBJECTS= $(OUTPUT_DIR)obj/


all: libfsm

walker:
	$(COMPILER) -c fsmwalker.cpp

builder:
	$(COMPILER) -c fsmbuilder.cpp

state:
	$(COMPILER) -c state.cpp

generator:
	$(COMPILER) -c stateidgenerator.cpp

fsm: walker builder state generator
	$(COMPILER) -c fsm.cpp $(FSM_OBJECTS)

libfsm: fsm
	ar rcs libfsm.a *.o
	
