
#include <math.h>
#include "convolve2DFB.h"
#include "tiff.h"
#include "allocate.h"
#include "typeutil.h"

void error(char *name);
void print_2D_double(int32_t rows, int32_t cols, double **arr);
void print_2D_int(int32_t rows, int32_t cols, int32_t **arr);
void clip(int32_t M_rows, int32_t N_cols, double **img);

int32_t main (int32_t argc, char **argv) 
{


  FILE *fp;
  struct TIFF_img input_img, filtered_img;
  double **red_arr, **green_arr, **blue_arr, **filter;
  double **filt_red, **filt_green, **filt_blue;
  int32_t i, j;
  int8_t filter_size = 9;
  char *out_file = "../images/FIR_LPF_filtered.tif";

  if ( argc != 2 ) error( argv[0] );

  /* open image file */
  if ( ( fp = fopen ( argv[1], "rb" ) ) == NULL ) {
    fprintf ( stderr, "cannot open file %s\n", argv[1] );
    exit ( 1 );
  }

  /* read image */
  if ( read_TIFF ( fp, &input_img ) ) {
    fprintf ( stderr, "error reading file %s\n", argv[1] );
    exit ( 1 );
  }

  /* close image file */
  fclose ( fp );

  /* check the type of image data */
  if ( input_img.TIFF_type != 'c' ) {
    fprintf ( stderr, "error:  image must be 24-bit color\n" );
    exit ( 1 );
  }

  /* Allocate doubles for processing. */
  filter = (double **) get_img(filter_size, filter_size, sizeof(double));
  red_arr = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  green_arr = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  blue_arr = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  filt_red = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  filt_green = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  filt_blue = (double **)get_img(input_img.width,input_img.height,sizeof(double));


  /* copy RGB components to double arrays */
  for ( i = 0; i < input_img.height; i++ )
  for ( j = 0; j < input_img.width; j++ ) {
    red_arr[i][j] = input_img.color[0][i][j];
    green_arr[i][j] = input_img.color[1][i][j];
    blue_arr[i][j] = input_img.color[2][i][j];
  }

  /* Filter the image! */
  /* Generate the filter in the problem statement */
  for (i=0; i<filter_size; i++){
    for (j=0; j<filter_size; j++){
      filter[i][j] = 1./81.;
    }
  }

  /* Convolve the filter with each color, perform clipping.*/
  convolve2DFB(input_img.height, input_img.width, filter_size,
               filter_size, red_arr, filter, filt_red);
  convolve2DFB(input_img.height, input_img.width, filter_size,
               filter_size, green_arr, filter, filt_green);
  convolve2DFB(input_img.height, input_img.width, filter_size,
               filter_size, blue_arr, filter, filt_blue);

  /* Clip */
  clip(input_img.height, input_img.width, filt_red);
  clip(input_img.height, input_img.width, filt_green);
  clip(input_img.height, input_img.width, filt_blue);
  
  /* Convert the int arrays to TIFF structs. */
  get_TIFF(&filtered_img, input_img.height, input_img.width, 'c' );
  for(i=0; i<input_img.height; i++){
    for(j=0; j<input_img.width; j++){
      filtered_img.color[0][i][j] = (int32_t)filt_red[i][j];
      filtered_img.color[1][i][j] = (int32_t)filt_green[i][j];
      filtered_img.color[2][i][j] = (int32_t)filt_blue[i][j];
    }
  }
  
  /* Try and open the file, error if can't. */
  if ((fp = fopen(out_file, "wb")) == NULL){
    fprintf(stderr, "cannot open file %s\n", out_file);
    exit(1);
  }

  /* Try and write the file, error if can't. */
  if(write_TIFF(fp, &filtered_img)){
    fprintf(stderr, "error writing TIFF file %s\n", out_file );
    exit(1);
  }
  /* Close the file. */
  fclose(fp);

  /* de-allocate space which was used for the images / arrays. */
  free_TIFF(&(input_img));
  free_TIFF(&(filtered_img));
  
  free_img((void**)red_arr);
  free_img((void**)green_arr);  
  free_img((void**)blue_arr);
  free_img((void**)filt_red);
  free_img((void**)filt_green);
  free_img((void**)filt_blue);
  free_img((void**)filter);

  return(0);
}

void error(char *name)
{
    printf("usage:  %s  image.tiff \n\n",name);
    printf("this program reads in a 24-bit color TIFF image.\n");
    printf("It then horizontally filters the RGB components\n");
    printf("with the FIR filter described in section 3 and writes\n");
    printf("out the result as a 24 bit color image\n");
    printf("with the name 'FIR_LPF_filtered.tiff'.\n");
    exit(1);
}

void clip(int32_t M_rows, int32_t N_cols, double **img){
  /*
  Perform clipping of image.
  */
  int32_t i, j;
  for (i = 0; i < M_rows; i++){
    for (j = 0; j < N_cols; j++){
     if (img[i][j] < 0.){
      img[i][j] = 0.;
     }
     if (img[i][j] > 255.){
      img[i][j] = 255.;
     }
    } 
  }
}

void print_2D_double(int32_t rows, int32_t cols, double **arr) {
  int32_t i, j;
  printf("[\n");
  for (i = 0; i < rows; i++) {
    printf("[");
    for (j = 0; j < cols; j++) {
        printf("%6.3f ", arr[i][j]);
    }
    printf("]\n");
  }
  printf("]\n");
}

void print_2D_int(int32_t rows, int32_t cols, int32_t **arr) {
  int32_t i, j;
  printf("[\n");
  for (i = 0; i < rows; i++) {
    printf("[");
    for (j = 0; j < cols; j++) {
        printf("%6d ", arr[i][j]);
    }
    printf("]\n");
  }
  printf("]\n");
}
