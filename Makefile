CXX	= g++
CXXFLAGS	= -Wall -g
OFILES	= heap.o huffmanNode.o huffmanenc.o 
OFILESx = huffmanNode.o huffmandec.o
.SUFFIXES: .o .cpp

all:A B
	g++ -o lz77-string CircBuffer.cpp lz77.cpp -I .
	g++ -o lz77-string-decompress CircBuffer.cpp lz77-decompress.cpp -I .

A:	$(OFILES)
	$(CXX) -o huffmanEncode $(OFILES)
B:	$(OFILESx)
	$(CXX) -o huffmanDecode $(OFILESx)
clean:
	/bin/rm -f *.o *~
heap.o: heap.h dsexceptions.h