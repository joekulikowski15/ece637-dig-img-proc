#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "allocate.h"
#include "tiff.h"
#include "connected_neighbors_sets.h"

void error(char *name);

int main(int argc, char **argv){
    FILE *fp;
    struct TIFF_img input_img, seg_img;
    struct pixel seed;
    int i, j, T, ClassLabel, NumConPixels=0;
    unsigned int **seg;
    char outfile[100];

    /*
    5 args. 1st is this file, 2nd is image to segment,
    3rd is row of seed pixel. 4th is col of seed.
    5th is threshold.
    */
    if(argc != 5) error(argv[0]);

    /* open image file */
    if((fp = fopen(argv[1], "rb")) == NULL) {
    fprintf(stderr, "cannot open file %s\n", argv[1]);
    exit(1);
    }
    /* read image */
    if(read_TIFF(fp, &input_img)){
    fprintf(stderr, "error reading file %s\n", argv[1]);
    exit(1);
    }
    if(input_img.TIFF_type != 'g'){
    fprintf(stderr, "error:  image must be grayscale.\n");
    exit(1);
    }
    /* close image file */
    fclose(fp);

    /* Read in seed pixel and threshold. */
    seed.m = atoi(argv[2]);
    seed.n = atoi(argv[3]);
    T = atoi(argv[4]);

    /* Initialize the connected set to 0, as described in the algorithm */
    seg = multialloc(sizeof(unsigned int), 2, input_img.height, input_img.width);
    for(i=0; i<input_img.height; i++){
        for(j=0; j<input_img.width; j++){
            seg[i][j] = 0;
        }
    }
    ClassLabel = 1;
    ConnectedSet(seed, T, input_img.mono, input_img.width, input_img.height,
                 ClassLabel, seg, &NumConPixels);
    printf("Number of connected pixels: %d\n", NumConPixels);
    get_TIFF(&seg_img, input_img.height, input_img.width, 'g');
    /* Flip-flop and send seg to seg_img as described. */
    for(i=0; i<seg_img.height; i++){
        for(j=0; j<seg_img.width; j++){
            if(seg[i][j]==ClassLabel){
                /* n.b 0=black, 255=white. */
                seg_img.mono[i][j] = 0;
            }else{
                seg_img.mono[i][j] = 255;
            }
        }
    }

    /* Make a sensible output filename. */
    snprintf(outfile, sizeof(outfile), "images/s1_%d_%d_%d.tif", seed.m, seed.n, T);

        /* Try and open the file, error if can't. */
    if((fp = fopen(outfile, "wb")) == NULL){
        fprintf(stderr, "cannot open file %s\n", outfile);
        exit(1);
    }

    /* Try and write the file, error if can't. */
    if(write_TIFF(fp, &seg_img)){
        fprintf(stderr, "error writing TIFF file %s\n", outfile);
        exit(1);
    }
    /* Close the file. */
    fclose(fp);

    multifree(seg, 2);
    free_TIFF(&seg_img);
    return 0;
}

void error(char *name){
    printf("usage:  %s  image.tiff row col threshold\n\n",name);
    printf("this program reads in a grayscale TIFF image.\n");
    printf("It finds the connected sets from a pixel at \n");
    printf("row col for a threshold.\n");
    exit(1);
}
