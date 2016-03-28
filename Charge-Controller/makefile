# This is the make file for lab 2 of Real Time systems
all: main

main: main.o 
	g++ -std=c++11 main.cpp cccomm.cpp uart.cpp i2c.cpp -o solarPanel
	
clean:
	rm *o solarPanel
