all:
	g++ -o lz77 CircBuffer.cpp lz77.cpp -I .
	g++ -o lz77-decompress CircBuffer.cpp lz77-decompress.cpp -I .