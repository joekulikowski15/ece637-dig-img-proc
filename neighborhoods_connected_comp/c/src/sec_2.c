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
    int i, j, T, ClassLabel, NumConPixels=0, NumConPixels2=0;
    unsigned int **seg;
    char outfile[100];

    /*
    3 args. 
    */
    if(argc != 3) error(argv[0]);

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

    /* Read in threshold. */
    T = atoi(argv[2]);

    /* allocate seg img populated with zeros. */
    seg = multialloc(sizeof(unsigned int), 2, input_img.height, input_img.width);
    get_TIFF(&seg_img, input_img.height, input_img.width, 'g');
    /* Initialize the connected sets to 0, as described in the algorithm */
    for(i=0; i<input_img.height; i++){
        for(j=0; j<input_img.width; j++){
            seg_img.mono[i][j] = 0;
            seg[i][j] = 0;
        }
    }
    ClassLabel = 1;
    for(i=0; i<input_img.height; i++){
        for(j=0; j<input_img.width; j++){
            /* Only if the pixel of the segmented image does not belong to a connected set */
            if(seg[i][j]==0){
                /* Set the seed in raster order. */
                seed.m = i; seed.n = j;
                NumConPixels=0;
                ConnectedSet(seed, T, input_img.mono, input_img.width, input_img.height,
                             ClassLabel, seg, &NumConPixels);
                if(NumConPixels >100){
                    /* Increment class label. */
                    ClassLabel++;
                }else{
                     NumConPixels2 = 0;
                     /* Label the small set as 0. */
                     ConnectedSet(seed, T, input_img.mono, input_img.width, input_img.height,
                                  0, seg, &NumConPixels2);
                    if(NumConPixels!=NumConPixels2){
                        error("The ConnectedSet function does not work.");
                    }
                }
            }
        }
    }
    printf("Threshold %d: %d Labels.\n",T,ClassLabel);
    for(i=0; i<input_img.height; i++){
        for(j=0; j<input_img.width; j++){
            seg_img.mono[i][j] = seg[i][j];
        }
    }

    /* Make a sensible output filename. */
    snprintf(outfile, sizeof(outfile), "images/segmentation_%d.tif", T);

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
