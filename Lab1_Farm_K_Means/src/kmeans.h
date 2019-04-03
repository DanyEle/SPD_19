#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SPACE_DIM 2
#define MAX_CENTR 20
#define CHUNK_SIZE 8000
// input is chunk size times number of processors == it is the same
// for the sequential algorithm
#define INPUT_SIZE CHUNK_SIZE
#define MAX_ITER 10

typedef double rNcoords [SPACE_DIM];

struct data_row{
  rNcoords coords;
  double mindist;
  int    centroid;
}; 

typedef struct data_row row; 

struct centroid_row {
  rNcoords center;
  int point_count;
};

typedef struct centroid_row c_row; 

/************ prototypes of functions from util.c */
void randcoords (rNcoords a, double s);
void sumcoords (rNcoords a, rNcoords b);
void printcoords (rNcoords a);
void readcoords (rNcoords a);
void fprintcoords (FILE* f, rNcoords a);
void freadcoords (FILE *f, rNcoords a);

void  generateNewCentroids(c_row * centroids, int n_cen);
void  generateClearCentroids(c_row * centroids, int n_cen);
double sdistance(rNcoords a, rNcoords b);

/**************************************************/
