all:
	g++ -o lz77-string CircBuffer.cpp lz77.cpp -I .
	g++ -o lz77-string-decompress CircBuffer.cpp lz77-decompress.cpp -I .