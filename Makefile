CXX	= g++
OFILES	= heap.o huffmanNode.o huffmanenc.o 
OFILESx = huffmanNode.o huffmandec.o
.SUFFIXES: .o .cpp

all: lz77 lz78 A B

CircBuffer.o: CircBuffer.cpp CircBuffer.h
	$(CXX) -c CircBuffer.cpp

lz77: lz77.cpp lz77-decompress.cpp lz77-sort.cpp CircBuffer.o
	$(CXX) -o lz77-compress CircBuffer.o lz77.cpp
	$(CXX) -o lz77-decompress CircBuffer.o lz77-decompress.cpp
	$(CXX) -o lz77-sort CircBuffer.o lz77-sort.cpp

lz78: lz78.cpp lz78-decompress.cpp lz78-sort.cpp
	$(CXX) -o lz78-compress lz78.cpp
	$(CXX) -o lz78-decompress lz78-decompress.cpp
	$(CXX) -o lz78-sort lz78-sort.cpp

A:	$(OFILES)
	$(CXX) -o huffmanEncode $(OFILES)
B:	$(OFILESx)
	$(CXX) -o huffmanDecode $(OFILESx)
clean:
	/bin/rm -f *.o *~
heap.o: heap.h dsexceptions.h