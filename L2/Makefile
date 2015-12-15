
CC=g++
CFLAGS=-c -Wall -Wno-enum-compare -std=c++11
LDFLAGS=-lmraa -pthread

all: Motor

Motor.o: Motor.cpp
	$(CC) $(CFLAGS) ${LDFLAGS} Motor.cpp

clean:
	rm *o Motor

