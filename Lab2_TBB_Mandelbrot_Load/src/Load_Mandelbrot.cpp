#include <iostream>
#include <string>
#include <algorithm>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/task_arena.h"
#include "tbb/task_group.h"




#include "Save_Array_as_PPM.c"

#define AMOUNT_ROWS 1200
#define AMOUNT_COLUMNS 1200

using namespace tbb;
using namespace std;

// square area, lower left angle and size
#define DEFAULT_X 2.0
#define DEFAULT_Y 2.0
#define DEFAULT_SIZE 4.0
//#define DEFAULT_PIXELS 10
#define DEFAULT_ITERATIONS 10000

#define AMOUNT_THREADS 16

// we assume a point diverges if its squared modulus exceeds this value
#define MAX_SMODULUS 4

// a variable holding the number of iterations limit
static int maxIter = DEFAULT_ITERATIONS;

int saveimg(int dimx, int dimy, const char * filename, int * matrix, int max_value);

//function computing the mandelbrot in a single point
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
	int a [AMOUNT_ROWS][AMOUNT_COLUMNS] = {{0}};
	//int * b [AMOUNT_ROWS];

	int(*b) [AMOUNT_ROWS];
	b = a;
	tbb::task_arena limited(AMOUNT_THREADS);

	limited.execute([b, &a]{
	tbb::task_group tg;

	  tg.run([b, &a]{ // run in task group
			  //loop over the matrix and compute the amount of iterations it takes
			  	//for the mandelbrot function to converge
			  	tbb::parallel_for(0, AMOUNT_ROWS, [b, &a](int row){
			  				tbb::parallel_for(0, AMOUNT_COLUMNS, [b, &a, row](int col){
			  				    //tbb::task_scheduler_init init(1);
			  					//rows = y axis
			  					//columns = x axis
			  					//compute the complex number
			  					double c_re = (col - AMOUNT_COLUMNS/DEFAULT_X)*4.0/AMOUNT_COLUMNS;
			  					double c_im = (row - AMOUNT_ROWS/DEFAULT_Y)*4.0/AMOUNT_ROWS;

			  					//let's compute mandelbrot based on it.
			  					int iters = mand_compute(c_re, c_im) ;

			  					//printf("%d \n", iters);

			  					b[row][col] = iters;
			  				});
			  			});
	  	  	  	  });
	  tg.wait();
	});

    //int n = tbb::task_scheduler_init::default_num_threads();

		for(int i = 0; i < AMOUNT_ROWS; i++)
		{
			for(int j = 0; j < AMOUNT_COLUMNS; j++)
			{
				printf("%d | " , b[i][j]);
			}
			printf("\n");
		}
		//let's try printing the final matrix, shall we?

	    const int MaxColorComponentValue = 255;
		saveimg(AMOUNT_COLUMNS, AMOUNT_ROWS, "mymatrix.ppm",* b, MaxColorComponentValue);

}

