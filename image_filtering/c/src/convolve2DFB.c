#include "typeutil.h"

void convolve2DFB(int32_t M_rows, int32_t N_cols, int32_t K_rows, 
                  int32_t L_cols, double **image, double **filter,
                  double **filtered) {
  /*
  Author: Joseph K. Kulikowski
  Perform 2D convolution enforcing  free boundary conditions
  on an M_rows x N_cols image with a K_rows x L_cols filter.
  
  Filter MUST be odd shaped in both direcitons.
  */
  int32_t m, n, k, l, row_offset, col_offset;
  int32_t filtered_row, filtered_col, filter_row, filter_col;
  double sum;
  row_offset = K_rows/2;
  col_offset = L_cols/2;
  /* 2 outer for loops to iterate through the image. */
  for (m = 0; m < M_rows; m++){
    for(n = 0; n < N_cols; n++){
      /* 
        Write inner loops to enforce Free Boundary Conditions, that is,
        if a pixel is outside the image, we mark it 0.
       */
      
      sum = 0;
      /* 
      This set of inner loops gives us an easy way to compute if we are
      outside the image!   
       */
      for(k = -row_offset; k <= row_offset; k++){
        for(l = -col_offset; l <= col_offset; l++){
          /* Position of pixels inside our filtered img */
          filtered_row = m + k;
          filtered_col = n + l;
          /* The actual filter indices are offset by K_rows/2 due to loop vars. */
          filter_row = k + row_offset;
          filter_col = l + col_offset;

          if (filtered_row >= 0 && filtered_row < M_rows &&
              filtered_col >= 0 && filtered_col < N_cols){
                /*
                We are within our bounds! contibute to the sum.
                */ 
                sum += filter[filter_row][filter_col] * image[filtered_row][filtered_col];
              }

        }
      }
      filtered[m][n] = sum;
    }
  }
}
