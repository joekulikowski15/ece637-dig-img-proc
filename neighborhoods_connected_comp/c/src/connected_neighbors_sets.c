#include <stdlib.h>
#include "allocate.h"
struct pixel{
    int m,n;
    /* m rows, n columns */
};

void ConnectedNeighbors(struct pixel s, double T, unsigned char **img,
                        int width, int height, int *M, struct pixel c[4]){
    /*
    Author: Joseph K. Kulikowski
    Determine the connected neighbors, c[n] | n<=4, to a pixel p within an
    image img of width and height, alongside the number of connected
    neighbors n.
    */

    int i, fb_conds, abs_diff; /* iterator, pixel in bounds, absolute difference*/
    struct pixel neighbor;
    int nh_offsets[4][2] = {
        {-1, 0},  /* above pixel. */
        {1, 0}, /* below pixel. */
        {0, -1}, /* left pixel. */
        {0, 1}, /* right pixel. */
    };
    /* Initialize number of connected neighborhoods to zero. */
    *M = 0;
    for(i=0; i<4; i++){
        /* find position of neighbor given offset. */
        neighbor.m = s.m + nh_offsets[i][0];
        neighbor.n = s.n + nh_offsets[i][1];
        /* Determine if the neighbor exists in the image */
        fb_conds = (neighbor.m < height) &&
                   (neighbor.m >=0) &&
                   (neighbor.n < width) &&
                   (neighbor.n >= 0);
        if(fb_conds){
            abs_diff = abs(img[s.m][s.n] - img[neighbor.m][neighbor.n]);
            /* Do we meet the threshold? */
            if(abs_diff<=T){
                /* If so, neighbor is connected. */
                c[*M].m = neighbor.m;
                c[*M].n = neighbor.n;
                /* Increment number of neighbors connected. */
                (*M)++;
            }
        }
    }

}

void ConnectedSet(struct pixel s, double T, unsigned char **img,
                  int width, int height, int ClassLabel,
                  unsigned int **seg, int *NumConPixels){
    /* Author: Joseph K. Kulikowski */
    /* a list of pixels B which are known to be connected to s0, but have not yet been searched */
    /* n.b. ANY pixel in the image COULD BE connected to s0. */
    int i, B_count=0;
    int M; /* connected neighbors */
    struct pixel loop_pixel, neighbor, neighbors[4], *B;
    B = (struct pixel *)multialloc(sizeof(struct pixel), 1, width * height);
    
    /* Initialize seed pixel to ClassLabel */
    seg[s.m][s.n] = ClassLabel;
    /* Put our seed pixel into the boundary set. (seed is connected to itself.) */
    B[B_count] = s;
    /* We now have one element in B, and 1 connected pixel. */
    B_count += 1;
    (*NumConPixels)++;

    while (B_count > 0){
        /* Decrement until we run out of pixels. */
        B_count -= 1;
        /* We expect to add ConnectedNeighbors to boundary set. */
        loop_pixel = B[B_count];
        M = 0; /* No connected neighbors yet! */
        /* Determine M neighbors, locations in neighbors */
        ConnectedNeighbors(loop_pixel, T, img, width, height, &M, neighbors);
        /* Loop through connected neighbors, add to boundary set */
        /* set seg[i][j] = ClassLabel where connected */
        for(i=0; i<M; i++){
            neighbor = neighbors[i];
            /* If neighbor hasn't been marked, it hasn't been added. */
            if(seg[neighbor.m][neighbor.n]!=ClassLabel){
                /* Mark seg as ClassLabel */
                seg[neighbor.m][neighbor.n] = ClassLabel;
                /* Add neighbor to boundary set! */
                B[B_count] = neighbor;
                /* Add to our B_count. */
                B_count += 1;
                /* increment our NumConPixels */
                (*NumConPixels)++;
            }
        }
    }
    multifree(B, 1);
}

void ConnectedNeighbors2D(struct pixel s, double T, unsigned char **img,
                          int n_neighbors, int **nh_offsets,
                          int width, int height, int *M, struct pixel c[]){
    /*
    Author: Joseph K. Kulikowski
    Determine the connected neighbors, c[n] | n<=n_neighbors, to a pixel p within an
    image img of width and height, alongside the number of connected
    neighbors n, using neighbors shifted by offsets nh_offsets[n_neighbors][2].
    */ 
    int i, fb_conds, abs_diff; /* iterator, pixel in bounds, absolute difference*/
    struct pixel neighbor;
    *M = 0;
    for(i=0; i<n_neighbors; i++){
        /* find position of neighbor given offset. */
        neighbor.m = s.m + nh_offsets[i][0];
        neighbor.n = s.n + nh_offsets[i][1];
        /* Determine if the neighbor exists in the image */
        fb_conds = (neighbor.m < height) &&
                  (neighbor.m >=0) &&
                  (neighbor.n < width) &&
                  (neighbor.n >= 0);
        if(fb_conds){
            abs_diff = abs(img[s.m][s.n] - img[neighbor.m][neighbor.n]);
            /* Do we meet the threshold? */
            if(abs_diff<=T){
                /* If so, neighbor is connected. */
                c[*M].m = neighbor.m;
                c[*M].n = neighbor.n;
                /* Increment number of neighbors connected. */
                (*M)++;
            }
        }
    }
}

void ConnectedSet2D(struct pixel s, double T, unsigned char **img,
                  int width, int height, int ClassLabel,
                  int n_neighbors, int **nh_offsets,
                  unsigned int **seg, int *NumConPixels){
    /* Author: Joseph K. Kulikowski */
    /* a list of pixels B which are known to be connected to s0, but have not yet been searched */
    /* n.b. ANY pixel in the image COULD BE connected to s0. */
    struct pixel loop_pixel, neighbor, *neighbors, *B;
    int i, B_count=0;
    int M; /* connected neighbors */
    neighbors = (struct pixel *)multialloc(sizeof(struct pixel), 1, n_neighbors);
    B = (struct pixel *)multialloc(sizeof(struct pixel), 1, width * height);

    /* Initialize seed pixel to ClassLabel */
    seg[s.m][s.n] = ClassLabel;
    /* Put our seed pixel into the boundary set. (seed is connected to itself.) */
    B[B_count] = s;
    /* We now have one element in B, and 1 connected pixel. */
    B_count += 1;
    (*NumConPixels)++;

    while (B_count > 0){
        /* Decrement until we run out of pixels. */
        B_count -= 1;
        /* The loop pixel should be removed from boundary set after loop. */
        /* We expect to add ConnectedNeighbors to boundary set. */
        loop_pixel = B[B_count];
        M = 0; /* No connected neighbors yet! */
        /* Determine M neighbors, locations in neighbors */
        ConnectedNeighbors2D(loop_pixel, T, img, n_neighbors, nh_offsets, width, height, &M, neighbors);
        /* Loop through connected neighbors, add to boundary set */
        /* set seg[i][j] = ClassLabel where connected */
        for(i=0; i<M; i++){
            neighbor = neighbors[i];
            /* If neighbor hasn't been marked, it hasn't been added. */
            if(seg[neighbor.m][neighbor.n]!=ClassLabel){
                /* Mark seg as ClassLabel */
                seg[neighbor.m][neighbor.n] = ClassLabel;
                /* Add neighbor to boundary set! */
                B[B_count] = neighbor;
                /* Add to our B_count. */
                B_count += 1;
                /* increment our NumConPixels */
                (*NumConPixels)++;
            }
        }
    }
    multifree(neighbors, 1);
    multifree(B, 1);
}
