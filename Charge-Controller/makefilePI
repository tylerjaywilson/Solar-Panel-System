# This is the make file for the Solar Panel Project
all: main

main: main.o 
	g++-4.9 -std=c++14 main.cpp cccomm.cpp psoccomm.cpp i2c.cpp uart.cpp -o solarPanel
	
clean:
	rm *o solarPanel
