#ifndef _CONVOLVE2DFB_H
#define _CONVOLVE2DFB_H

#include "typeutil.h"

/*
Author: Joseph K. Kulikowski
Perform 2D convolution enforcing  free boundary conditions
on an M_rows x N_cols image with a K_rows x L_cols filter.
*/

void convolve2DFB(int32_t M_rows, /* Height of the input image. */
                  int32_t N_cols, /* Width of the input image. */
                  int32_t K_rows, /* Height of the filter to be applied. */
                  int32_t L_cols, /* Width of the filter to be applied. */
                  double **image, /* Image to filter. */
                  double **filter, /* Filter to apply. */
                  double **filtered /* Result of the 2D convolution. */
                  );
#endif /* _CONVOLVE2DFB */
