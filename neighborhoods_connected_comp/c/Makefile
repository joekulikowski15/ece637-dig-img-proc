# For Linux or any machines with gcc compiler
CC = gcc 
LICENSED = ../../licensed
CFLAGS = -ansi -Wall -pedantic -g -O0 -I $(LICENSED)
BIN = bin
IMG = ../images
SRC = src

all: sec_1 sec_2

clean:
	/bin/rm *.o $(BIN)/*

OBJ = $(LICENSED)/tiff.o $(LICENSED)/allocate.o $(LICENSED)/randlib.o $(LICENSED)/qGGMRF.o $(LICENSED)/solve.o src/connected_neighbors_sets.o

sec_1: $(SRC)/sec_1.o $(OBJ) 
	$(CC) $(CFLAGS) -o $(SRC)/sec_1.out $(SRC)/sec_1.o $(OBJ) -lm
	mv $(SRC)/sec_1.out $(BIN)
sec_2: $(SRC)/sec_2.o $(OBJ) 
	$(CC) $(CFLAGS) -o $(SRC)/sec_2.out $(SRC)/sec_2.o $(OBJ) -lm
	mv $(SRC)/sec_2.out $(BIN)