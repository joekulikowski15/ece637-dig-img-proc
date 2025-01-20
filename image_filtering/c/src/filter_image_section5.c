
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
  double color_val;
  double **filt_red, **filt_green, **filt_blue;
  int32_t i, j, c;
  char *out_file = "../images/IIR_filtered.tif";
  double left, up, leftup;
  int8_t left_exists, up_exists, leftup_exists;

  
  /*
  3 input args. the name of the file, the name of the
  input image file, and the lambda value.
  */
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

  /* Name output file based on lambda input. */
  

  /* check the type of image data */
  if ( input_img.TIFF_type != 'c' ) {
    fprintf ( stderr, "error:  image must be 24-bit color\n" );
    exit ( 1 );
  }

  /* Allocate doubles for processing. */
  filt_red = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  filt_green = (double **)get_img(input_img.width,input_img.height,sizeof(double));
  filt_blue = (double **)get_img(input_img.width,input_img.height,sizeof(double));

  /* Work with rgb components while we filter the image! */
  for(c=0; c<3; c++){
    for(i=0; i<input_img.height; i++){
      for(j=0; j<input_img.width; j++){
        color_val = input_img.color[c][i][j];
        /* enforce Free Boundary Conditions. */
        left_exists = j-1>=0;
        up_exists = i-1>=0;
        leftup_exists = (i-1>=0) && (j-1 >=0);
        if(c==0){
          /* Define left, up, and left-up pixels for the red component. */
          left = (left_exists) ? filt_red[i][j-1] : 0.0;
          up = (up_exists) ? filt_red[i-1][j] : 0.0;
          leftup = (leftup_exists) ? filt_red[i-1][j-1] : 0.0;
          filt_red[i][j] = (0.01 * color_val) + (0.9*(left + up)) - (0.81 * leftup);
        }
        if(c==1){
          /* Define left, up, and left-up pixels for the green component. */
          left = (left_exists) ? filt_green[i][j-1] : 0.0;
          up = (up_exists) ? filt_green[i-1][j] : 0.0;
          leftup = (leftup_exists) ? filt_green[i-1][j-1] : 0.0;
          filt_green[i][j] = (0.01 * color_val) + (0.9*(left + up)) - (0.81 * leftup);
        }
        if(c==2){
          /* Define left, up, and left-up pixels for the blue component. */
          left = (left_exists) ? filt_blue[i][j-1] : 0.0;
          up = (up_exists) ? filt_blue[i-1][j] : 0.0;
          leftup = (leftup_exists) ? filt_blue[i-1][j-1] : 0.0;
          filt_blue[i][j] = (0.01 * color_val) + (0.9*(left + up)) - (0.81 * leftup);
        }
      }
    }
  }

  /* Clip all components of the RGB components.*/
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
  free_img((void**)filt_red);
  free_img((void**)filt_green);
  free_img((void**)filt_blue);

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

