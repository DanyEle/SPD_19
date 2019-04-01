#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kmeans.h"

#define REALLYRANDOM srandom(time(NULL));
#ifdef REALLYRANDOM
#include <time.h>
#endif

/************************************************************************/
void usage ()
{
  printf("Usage: generatadata <num_centroids> <num_chunks, must be 1>");
  printf("Space Dimension is %d, chunk size is %d\n", SPACE_DIM, CHUNK_SIZE);
}

/************************************************************************/

int main (int argc, char ** argv)
{
  int n_cen;
  int n_chu;
  int iter;
  rNcoords centroids[MAX_CENTR];
  double clust_size;
  rNcoords temp_row;
  int i,j;

  if (argc!= 3) {
    usage();
    return -1;
  }

  n_cen = atof(argv[1]);
  n_chu = atof(argv[2]);

  if (n_cen <=0 || n_chu <=0 || n_chu>1) {
    usage();
    return -1;
  }

  REALLYRANDOM;
  //generate centroids
  for (i=0;i<n_cen;i++)
    {
      randcoords(centroids[i], 1);
    }

  // we'd better compute average distance among centroids
  // but for now we just use a fixed parameter instead
  clust_size = 0.4;

  j=0; // centroid to choose

  for (i=0;i<CHUNK_SIZE;i++)
   {
      randcoords(temp_row, clust_size);
      sumcoords(temp_row, centroids[j]); //assign to centroid j
      printcoords(temp_row);

      j += 1; j = j % n_cen; 

   }

  return 0;
}
