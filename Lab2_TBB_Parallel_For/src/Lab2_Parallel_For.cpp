#include "tbb/tbb.h"
#include <iostream>
#include <cstdio>

float square_float(float a);

#define ARRAY_SIZE 10

using namespace std;


using namespace tbb;
class ApplyFoo
{
	float *const my_a;
	public:

	//Workaround to store a value.
	float * my_b;

	void operator()( const blocked_range<size_t>& r )

	const
	{
		float *a = my_a;
		for( size_t i=r.begin(); i!=r.end(); ++i )
		{
			//std::cout <<  FooSquare(a[i]);
			my_b[i] = square_float(a[i]);
		}
	}

	//Constructor?
	ApplyFoo( float a[] ) :
	my_a(a)
	{
		my_b = a;

	}
};

float square_float(float a)
{
	return a * a;

}

void ParallelApplyFoo( float a[], size_t n ) {
	parallel_for(blocked_range<size_t>(0,n), ApplyFoo(a));
}

int main()
{
	//insert values into the array
	float testArray [ARRAY_SIZE] = {};

	for(int i = 0; i < ARRAY_SIZE; i++)
	{
		testArray[i] = i;
	}

	ApplyFoo applFoo(testArray);

	ParallelApplyFoo(testArray, ARRAY_SIZE);

	//read values from the array
	for(int i = 0; i < ARRAY_SIZE; i++)
	{
		cout << testArray[i] << endl;
	}


}


