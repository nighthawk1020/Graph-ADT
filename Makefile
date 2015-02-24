COMPILER = g++
FLAGS = -g -Wall -std=c++0x -O2
EXECUTABLE = GraphLab
all: Driver.o mmciner1Graph.a Edges.o
	$(COMPILER) $(FLAGS) Driver.o Edges.o -o $(EXECUTABLE) mmciner1Graph.a

Driver.o: Driver.cpp
	$(COMPILER) $(FLAGS) -c Driver.cpp

mmciner1Graph.o: mmciner1Graph.cpp
	$(COMPILER) $(FLAGS) -c mmciner1Graph.cpp

Edges.o: Edges.cpp
	$(COMPILER) $(FLAGS) -c Edges.cpp

mmciner1Graph.a: mmciner1Graph.o
	ar rcs mmciner1Graph.a mmciner1Graph.o
	rm mmciner1Graph.o

clean:
	rm -f *.o $(EXECUTABLE) mmciner1Graph.a BFTTestg1.txt BFTTestg2.txt DFTTestg21.txt DFTTestg22.txt DFTTestg1.txt MSTTestg1.txt MSTTestg2.txt StepAwayg11.txt StepAwayg12.txt StepAwayg13.txt StepAwayg2.txt testWrite.txt testWrite2.txt
