all: qr_decode

qr_decode: qrCode_C.o qrheader_test.o
	gcc -o qr_decode qrCode_C.o qrheader_test.o 

qrCode_C.o: qrCode_C.h qrCode_C.cpp
	g++ -c -o 'pkg-config --cflags opencv' qrCode_C.o qrCode_C.cpp 'pkg-config --libs opencv'

qrheader_test.o: qrheader_test.c qrCode_C.h
	gcc -o qrheader_test.o qrheader_test.c

clean:
	rm -f qr_decode *.o