/* Author: Joseph K. Kulikowski*/
/* Sat, Feb 1, 2025*/

#ifndef CONNECTEDNEIGHBORSSETS_H
#define CONNECTEDNEIGHBORSSETS_H

#include <stdlib.h>
#include "allocate.h"

struct pixel {
int m,n; /* m=row, n=col */
};

void ConnectedNeighbors(
/*
Author: Joseph K. Kulikowski
Determine the connected neighbors, c[n] | n<=4, to a pixel p within an
image img of width and height, alongside the number of connected
neighbors n.
*/
struct pixel s,
double T,
unsigned char **img,
int width,
int height,
int *M,
struct pixel c[4]
);

void ConnectedSet(
/* Author: Joseph K. Kulikowski */
struct pixel s,
double T,
unsigned char **img,
int width,
int height,
int ClassLabel,
unsigned int **seg,
int *NumConPixels
);

void ConnectedNeighbors2D(
/*
Author: Joseph K. Kulikowski
Determine the connected neighbors, c[n] | n<=n_neighbors, to a pixel p within an
image img of width and height, alongside the number of connected
neighbors n, using neighbors shifted by offsets nh_offsets[n_neighbors][2].
*/ 
struct pixel s,
double T,
unsigned char **img,
int n_neighbors,
int (*nh_offsets)[2],
int width,
int height,
int *M,
struct pixel c[]);

void ConnectedSet2D(
/* Author: Joseph K. Kulikowski */
struct pixel s,
double T,
unsigned char **img,
int width,
int height,
int ClassLabel,
int n_neighbors,
int (*nh_offsets)[2],
unsigned int **seg,
int *NumConPixels);

#endif /* CONNECTEDNEIGHBORSSETS */
