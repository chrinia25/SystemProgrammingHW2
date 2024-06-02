all: qr_decode

qr_decode: qrCode_C.o qrheader_test.o
	g++ qrheader_test.o qrCode_C.o -o qr_decode `pkg-config --libs opencv4`

qrCode_C.o: qrCode_C.h qrCode_C.cpp
	g++ -c qrCode_C.cpp -o qrCode_C.o `pkg-config --cflags opencv4`


qrheader_test.o: qrheader_test.c qrCode_C.h
	gcc -o qrheader_test.o -c qrheader_test.c

clean:
	rm -f qr_decode *.o