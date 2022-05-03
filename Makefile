CC=gcc

all: main.o event.o calendar.o util.o
	$(CC) -o planter main.o event.o calendar.o util.o
