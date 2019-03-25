#include "kmeans.h"

/* utility functions for the Kmeans code  */

//generate random array in [0..1]^N scaled by factor s
void randcoords (rNcoords a, double s)
{
  int i;
  for (i=0;i<SPACE_DIM; i++)
    a[i]= ((double) random())/(pow(2, 31)) * s;
};

// sum the array b to the array a
void sumcoords (rNcoords a, rNcoords b)
{
  int i;
  for (i=0;i<SPACE_DIM; i++)
    a[i]+= b[i];
}

// generate a set of random centroids
void  generateNewCentroids(c_row * centroids, int n_cen)
{ int i;
  for (i=0; i<n_cen; i++)
    {
      randcoords(centroids[i].center, 1);
      centroids[i].point_count=0;
    }
};

// generate a set of clear centroids
void  generateClearCentroids(c_row * centroids, int n_cen)
{ int i,j;
  for (i=0; i<n_cen; i++)
    {
      for (j=0;j<SPACE_DIM; j++)
	{centroids[i].center[j]=0.0;}
      centroids[i].point_count=0;
    }
};

// compute the distance between two sets of coordinates
// compute the squared distance of two coordinates
double sdistance(rNcoords a, rNcoords b)
{
  int i; 
  double sum = 0;
  for (i=0;i<SPACE_DIM;i++)
    {
      // we don't use pow here
      sum += (a[i]-b[i])*(a[i]-b[i]);
    }
  //return sqrt(sum); // if we wanted the distance
  return sum;
}

//output an array row as a tab-separated string
void printcoords (rNcoords a)
{
  int i;
  for (i=0;i<SPACE_DIM; i++)
    printf("%10e\t", a[i]); 
  printf("\n");
};

/* function to read data in a set of coords array */
//input an array row as a tab-separated string
void readcoords (rNcoords a)
{
  int i;
  for (i=0;i<SPACE_DIM; i++)
    {   
      if (scanf("%le", &a[i]) <1) exit(-127); 
    }
  //debug  printcoords (a);
};

//output an array row as a tab-separated string
void fprintcoords (FILE* f, rNcoords a)
{
  int i;
  for (i=0;i<SPACE_DIM; i++)
    fprintf(f,"%10e\t", a[i]); 
  fprintf(f,"\n");
};

/* function to read data in a set of coords array */
//input an array row as a tab-separated string
void freadcoords (FILE *f, rNcoords a)
{
  int i;
  for (i=0;i<SPACE_DIM; i++)
    {   
      if (fscanf(f, "%le", &a[i]) <1) exit(-127); 
    }
  //debug  printcoords (a);
};
