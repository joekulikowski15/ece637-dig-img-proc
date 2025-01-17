#ifndef _CONVOLVE2DFB_H
#define _CONVOLVE2DFB_H

#include "typeutil.h"

/*
Author: Joseph K. Kulikowski
Perform 2D convolution enforcing  free boundary conditions
on an M_rows x N_cols image with a K_rows x L_cols filter.

Filter MUST be odd shaped in both direcitons.
*/

void convolve2DFB(int32_t M_rows, int32_t N_cols, int32_t K_rows, 
                  int32_t L_cols, double **image, double **filter,
                  double **filtered);
#endif /* _CONVOLVE2DFB */