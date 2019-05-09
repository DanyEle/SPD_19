#include "tbb/tbb.h"
#include <iostream>
#include <cstdio>

#define AMOUNT_ROWS 10
#define AMOUNT_COLUMNS 10

using namespace std;


using namespace tbb;




int main()
{
	//insert values into the array
	int a [AMOUNT_ROWS][AMOUNT_COLUMNS] = {};

	for(int i = 0; i < AMOUNT_ROWS; i++)
	{
		for(int j = 0; j < AMOUNT_COLUMNS; j++)
		{
			a[i][j] = i;
		}
	}

	tbb::parallel_for(0, AMOUNT_ROWS, [a](int i){
	        tbb::parallel_for(0, AMOUNT_COLUMNS, [i, a](int j){

	            printf("Hello World %d\n", a[i][j]);

	        });
	    });




}


