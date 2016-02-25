# This is the make file for lab 2 of Real Time systems
all: main

main: main.o 
	g++ -std=c++11 main.cpp commsend.cpp commreceive.cpp uart.cpp -o solarPanel

clean:
	rm *o solarPanel