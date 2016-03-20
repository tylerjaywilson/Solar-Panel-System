# This is the make file for lab 2 of Real Time systems
all: main

main: main.o 
	g++-4.9 -std=c++14 main.cpp commreceive.cpp commsend.cpp uart.cpp -o solarPanel
	
clean:
	rm *o solarPanel
