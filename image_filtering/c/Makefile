# For Linux or any machines with gcc compiler
CC = gcc 
LICENSED = ../../licensed
CFLAGS = -ansi -Wall -pedantic -g -O0 -I $(LICENSED)
BIN = bin
IMG = ../images
SRC = src

all: filter_image_section3

clean:
	/bin/rm *.o $(BIN)/*

OBJ = $(LICENSED)/tiff.o $(LICENSED)/allocate.o $(LICENSED)/randlib.o $(LICENSED)/qGGMRF.o $(LICENSED)/solve.o

filter_image_section3: $(SRC)/filter_image_section3.o $(OBJ) 
	$(CC) $(CFLAGS) -o $(SRC)/filter_image_section3.out $(SRC)/filter_image_section3.o $(OBJ) -lm 
