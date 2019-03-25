
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// number of rows and columns in matrix (square matrix)
#define N 3
//value set in matrix A
#define A_VALUE 5
//value set in matrix B
#define B_VALUE 5

#define EMITTER_TAG 750
#define COLLECTOR_TAG 751

#define EMITTER_RANK 0
#define COLLECTOR_RANK 1

#define EOS_VALUE  -1



//forward declarations

void emitter(int number_processes, int my_rank);
void worker(int number_processes, int my_rank);



int main(int argc, char ** argv)
{
	int my_rank;
	int number_processes;

	MPI_Init(&argc, &argv);
	//basic communicator	int B[N][N];

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &number_processes);

    if(number_processes <= 2)
    {
    	printf("You have provided %d processes. At least 3 processes are required though (scatter, worker, gather).\n", number_processes);
    	return - 1;
    }

    //emitter - distribute data among workers
    if(my_rank == EMITTER_RANK)
    {
    	emitter(number_processes, my_rank);
     }
    //gather. gather the data from the different workers
    else if(my_rank == COLLECTOR_RANK)
    {

    }
    //worker. actually do the work
    else if(my_rank != 0 && my_rank != 1)
    {
    	worker(number_processes, my_rank);
    }

	MPI_Finalize();

	return 0;

}

void emitter(int number_processes, int my_rank)
{
	//in the scatter process
	int A[N][N];
	int B[N][N];


	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			A[i][j] = A_VALUE;
			printf("%d ", A[i][j]);
		}
		printf("\n");
	}

	printf("\n");


	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			B[i][j] = B_VALUE;
			printf("%d ", B[i][j]);
		}
		printf("\n");
	}

	printf("C: Preparting work in the emitter\n");

	//send row and column to one single process.

	//loop through all the columns of the matrix A
	//loop through all the rows of the matrix B
	for(int i = 0; i < N; i++)
	{
		//columnA is one single column of the matrix A
		int columnA[N];
		for(int j = 0; j < N; j++)
		{
			columnA[j] = A[i][j];
		}

		//rowB is one single row of the matrix B
		int rowB[N];
		for(int j = 0; j < N; j++)
		{
			rowB[N] = B[j][i];
		}

		//now need to send columnA and rowB to one single worker
		int worker_destination = (i % (number_processes - 2)) + 2;

		//who cares about data structures, when you can simply use two sends
		MPI_Send(&columnA, N, MPI_INT, worker_destination, EMITTER_TAG, MPI_COMM_WORLD);
		MPI_Send(&rowB, N, MPI_INT, worker_destination, EMITTER_TAG, MPI_COMM_WORLD);
	}

	//COMMUNICATION OF MATRIX ROWS AND COLUMNS TERMINATED. NOW SEND EOS VALUES.
	int EOS_Array[N];

	for(int i = 0; i < N; i++)
	{
		EOS_Array[i] = EOS_VALUE;
	}

	//send an EOS array to all the workers
	for(int j = 2; j < number_processes; j++)
	{
		MPI_Send(&EOS_Array, N, MPI_INT, j, EMITTER_TAG, MPI_COMM_WORLD);
	}

}


void worker(int number_processes, int my_rank)
{


	//only works for one single worker!!!!
	while(1 == 1)
	{

		int columnA[N];
		MPI_Recv(&columnA, N, MPI_INT, EMITTER_RANK, EMITTER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//check if an EOS was received
		if(columnA[0] == EOS_VALUE)
		{
			printf("W: Received EOS array A\n");
			//neeed to propagate the EOS to the collector too, actually
			break;
		}
		else
		{
			printf("W: Received column A\n");
		}

		int rowB[N];
		MPI_Recv(&rowB, N, MPI_INT, EMITTER_RANK, EMITTER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


		//check if an EOS was received
		printf("W: Received row B\n");

	}


}


