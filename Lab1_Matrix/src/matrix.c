
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N               10        /* number of rows and columns in matrix */

//forward declarations
int handle_communication(int my_rank, int number_processes);


int main(int argc, char ** argv)
{
	int my_rank;

	int number_processes;
	MPI_Init(&argc, &argv);
	//basic communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int mpi_error_code = MPI_Comm_size(MPI_COMM_WORLD, &number_processes);

    mpi_error_code = handle_communication(my_rank, number_processes);

	MPI_Finalize();

	return 0;

}



int handle_communication(int my_rank, int number_processes)
{
	//each message is
	const int MAX_MESSAGE_SIZE = N * N;

	const int DUMMY_TAG = 789;

	//let's try defining the array over here
	int a[N][N];
	printf("I am in process %d.  \n", my_rank);

	//receive all elements of the matrix
	int buffer_receiver[N][N];

	int next_process = (my_rank + 1) % number_processes;
	int previous_process = (my_rank - 1) % number_processes;


	//root, needs to init the communication
	if(my_rank == 0)
	{
		printf("Sending the following matrix:\n");
		//initialize thr matrix' values
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < N; j++)
			{
				a[i][j] = 1;
				printf("%d|", a[i][j]);
			}
			printf("\n");
		}
		//let's send the matrix
		MPI_Send(&a, MAX_MESSAGE_SIZE, MPI_INT, next_process, DUMMY_TAG, MPI_COMM_WORLD);


	}

	//other processes. need to first receive message from previous process, then send to the next process.
	if(my_rank != 0)
	{
		printf("Received matrix:\n");
		//mpi_error_code = MPI_Recv(&a, N, MPI_INT, previous_process,  DUMMY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&buffer_receiver, MAX_MESSAGE_SIZE, MPI_INT, previous_process, DUMMY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//let's perform some operations on the received matrix, e.g: increase all of its elements
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < N; j++)
			{
				buffer_receiver[i][j] += 3;
			}
		}

		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < N; j++)
			{
				printf("%d|", buffer_receiver[i][j]);
			}
			printf("\n");
		}
		printf("\n");


	}




	return 0;

}
