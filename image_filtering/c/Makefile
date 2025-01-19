# For Linux or any machines with gcc compiler
CC = gcc 
LICENSED = ../../licensed
CFLAGS = -ansi -Wall -pedantic -g -O0 -I $(LICENSED)
BIN = bin
IMG = ../images
SRC = src

all: filter_image_section3 filter_image_section4

clean:
	/bin/rm *.o $(BIN)/*

OBJ = $(LICENSED)/tiff.o $(LICENSED)/allocate.o $(LICENSED)/randlib.o $(LICENSED)/qGGMRF.o $(LICENSED)/solve.o src/convolve2DFB.o

filter_image_section3: $(SRC)/filter_image_section3.o $(OBJ) 
	$(CC) $(CFLAGS) -o $(SRC)/filter_image_section3.out $(SRC)/filter_image_section3.o $(OBJ) -lm 

filter_image_section4: $(SRC)/filter_image_section4.o $(OBJ) 
	$(CC) $(CFLAGS) -o $(SRC)/filter_image_section4.out $(SRC)/filter_image_section4.o $(OBJ) -lm 