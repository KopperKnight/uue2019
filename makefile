
all: uuencode uudecode

uuencode:
	g++ -o uuencode -Wall uuencode.cpp



uudecode:
	g++ -o uudecode -Wall uudecode.cpp


clean:
	rm uuencode.exe
	rm uudecode.exe	
