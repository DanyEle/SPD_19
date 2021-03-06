#include "kmeans.h"
#include <time.h>

// macro to print the centroids to stdout at each iteration
#define PRINTCENTR(x)  x

//macro to store a set of centroids once convergence is reached
#define STORECENTR(x)  x

/* prototypes of functions from this file */
void computemindist(row * data, c_row * centroids, int n_cen);
void printcentroids(c_row * centroidSet, int n_cen);
void fprintcentroids(FILE *f, c_row * centroidSet, int n_cen);

/******************************************/

row data [CHUNK_SIZE];

int n_cen = 5;
c_row centroids [MAX_CENTR];

c_row new_centroids [MAX_CENTR];



int main(int argc, char ** argv )
{
  int i,j, data_point;
  int convergence =0; //not used yet
  int iteration = 0;

  /* !!! add here a function to read in the data
     (or generate randomly)*/
  // read input data *****************************************
  for (i=0;i<CHUNK_SIZE; i++){
    readcoords(data[i].coords);
    data[i].mindist=0;    // clear distances
    data[i].centroid=-1;  // assign to no centroid
    //
  }

  fprintf(stderr, "Read in %d rows\n",i);

  /* outer cycle, generates a centroid set, computes iterations, at
     convergence exits or generates a new one */
  do {

    /* we should initialize the MSE to a large value here, we skip
   this for now */



    /* random init of centroids */
    srandom(time(NULL));
    generateNewCentroids(centroids, n_cen);
    PRINTCENTR( printf("# Iteration 0\n");printcentroids(centroids, n_cen));


    do {
      //deal with convergence condition, we skip it

      // init new centroids to zero
      generateClearCentroids(new_centroids, n_cen);

      //iterate over all the data points
      for(data_point=0; data_point <CHUNK_SIZE; data_point++)
	{
	  computemindist(&data[data_point], centroids, n_cen);

	  /* update the new centroids which will be valid at next
	     iteration */
	  int closest = data[data_point].centroid;

	  sumcoords( new_centroids[closest].center,
		     data[data_point].coords);

	  new_centroids[closest].point_count++;

	  // we should compute the convergence measure,
	  // skip for now

	}

      // compute the new centroids by average
      for(i=0;i<n_cen;i++)
	{
	  //divide centroid coords by number of points
	  if (new_centroids[i].point_count > 0)
	    {
	      for(j=0;j<SPACE_DIM;j++)
		{
		  new_centroids[i].center[j] /=
		    new_centroids[i].point_count;
		}
	    }
	}

      // copy new centroids onto old ones

      for (i=0;i<n_cen;i++)
	{
	  centroids[i] = new_centroids[i];
	}


      PRINTCENTR( printf("# Iteration %d\n",iteration+1);
		  printcentroids(centroids, n_cen)) ;

      // evaluate convergence (not yet)

    }while(!convergence && ++iteration < MAX_ITER);
      //until convergence

      //store results (local optimum) and decide if we try again

  } while(0); //in this test program we just make one run

  //choose best results if multiple runs

  // print results (hopefully valid gnuplot input)
  STORECENTR( printf("# Final result\n"); printcentroids(centroids, n_cen));

  return 0;


}


void  computemindist(row * data, c_row * centroids, int n_cen)
{
  int i,j;
  double mindist = 1e32;
  double dist = 0;
  int closest = -1;
  for (i=0; i<n_cen; i++)
    {
      dist = sdistance (data->coords, centroids[i].center);
      if (dist < mindist)
	{
	  mindist = dist;
	  closest = i;
	}
    }
  if (closest == -1 || closest >= n_cen) exit (-128);
  //  printf ("<%d>",closest);
  data->mindist = mindist;
  data->centroid = closest;
}

// print the set of centroids which is passed as argument
void printcentroids(c_row * centroidSet, int n_cen)
{
  fprintcentroids(stdout, centroidSet, n_cen);
}

// print the set of centroids which is passed as argument
void fprintcentroids(FILE *f, c_row * centroidSet, int n_cen)
{
  int i;
  fprintf(f, "# centroids \n");
  for (i=0;i<n_cen;i++)
    {
      fprintcoords(f, centroidSet[i].center);
    }
}
