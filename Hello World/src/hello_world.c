
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	int Wsize;
	int myrank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &Wsize);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	printf("Starting process %d of %d", myrank, Wsize);

	MPI_Finalize();

	return 0;

}
