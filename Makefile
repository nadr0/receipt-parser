CC = gcc
INC = -I.
FLAGS = -W -Wall -g -std=c99 -Wno-unused-variable -Wno-unused-parameter
LDFLAGS = -pthread

all: m

m: meijer.o parsePDF.o
	$(CC) meijer.o parsePDF.o -o m $(LDFLAGS)

meijer.o: meijer.c 
	$(CC) $(INC) $(FLAGS) -c meijer.c $(LDFLAGS)

parsePDF.o: parsePDF.c parsePDF.h
	$(CC) $(INC) $(FLAGS)  -c parsePDF.c $(LDFLAGS)
clean:
	rm -rf m