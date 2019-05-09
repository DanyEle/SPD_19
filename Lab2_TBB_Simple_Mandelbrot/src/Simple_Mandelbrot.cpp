#include <iostream>
#include <string>
#include <algorithm>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

#include "Save_Array_as_PPM.c"

#define AMOUNT_ROWS 10
#define AMOUNT_COLUMNS 10

using namespace tbb;
using namespace std;

// square area, lower left angle and size
#define DEFAULT_X -2.0
#define DEFAULT_Y -2.0
#define DEFAULT_SIZE 4.0
#define DEFAULT_PIXELS 10
#define DEFAULT_ITERATIONS 1000

// we assume a point diverges if its squared modulus exceeds this value
#define MAX_SMODULUS 4

// a variable holding the number of iterations limit
static int maxIter = DEFAULT_ITERATIONS;

int saveimg(int dimx, int dimy, const char * filename, int * matrix, int max_value);


//function computing the mandelbrot in asingle point
//returns the number of iteration until divergence
int mand_compute( double cx, double cy)
{
 int i=0;
    double x = cx; double y = cy;
    double nx, ny;
    for (i=0; i<maxIter; i++)
    {
       // (x,y)^2 + c
       nx = x*x - y*y + cx;
       ny = 2*x*y + cy;

       if ( nx*nx + ny*ny > MAX_SMODULUS ) break;
       x=nx; y=ny;
    }
    return i;
}


//main
int main () {


	// initialization of matrix
	int a [AMOUNT_ROWS][AMOUNT_COLUMNS] = {};

		for(int i = 0; i < AMOUNT_ROWS; i++)
		{
			for(int j = 0; j < AMOUNT_COLUMNS; j++)
			{
				a[i][j] = i;
			}
		}


	//loop over the matrix and compute the amount of iterations it takes
	tbb::parallel_for(0, AMOUNT_ROWS, [a](int i){
				tbb::parallel_for(0, AMOUNT_COLUMNS, [i, a](int j){

					//double cur_value = (double) a[i][j];
					//rows = y axis
					//columns = x axis
					double y = (double)i;
					double x = (double)j;

					//let's compute mandelbrot based on it.
					int iters = mand_compute(x, y);

					printf("%d \n", iters);

					//b[i][j] = iters;

				});
			});

		for(int i = 0; i < AMOUNT_ROWS; i++)
		{
			for(int j = 0; j < AMOUNT_COLUMNS; j++)
			{
				printf("%d \n ", a[i][j]);
			}
		}




}

