
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N               10        /* number of rows and columns in matrix */
#define K

//forward declarations
int handle_communication(int my_rank, int number_processes, int send_integer);


int main(int argc, char ** argv)
{
	int my_rank;

	int number_processes;

	int initial_integer = 23;

	MPI_Init(&argc, &argv);
	//basic communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int mpi_error_code = MPI_Comm_size(MPI_COMM_WORLD, &number_processes);

    mpi_error_code = handle_communication(my_rank, number_processes, initial_integer);

    //send to the next process

	MPI_Finalize();

	return 0;

}



int handle_communication(int my_rank, int number_processes, int integer_send)
{
	const int MAX_MESSAGE_SIZE = N;
	const int BUFFER_SIZE = N;
	const int DUMMY_TAG = 789;

	//let's try defining the array over here
	int a[BUFFER_SIZE];
	printf("I am in process %d.  \n", my_rank);

	//receive all elements of the matrix
	int buffer_receiver[BUFFER_SIZE];

	int next_process = (my_rank + 1) % number_processes;
	int previous_process = (my_rank - 1) % number_processes;

	//root, needs to init the communication
	if(my_rank == 0)
	{
		//initialize elements of an array
		for(int i = 0; i < N; i++)
		{
			a[i] = 1;
			printf("%d|", a[i]);
		}


		for(int i = 0; i < N; i++)
		{
			MPI_Request mpi_request;
			MPI_Isend(&a, MAX_MESSAGE_SIZE, MPI_INT, next_process, DUMMY_TAG, MPI_COMM_WORLD, &mpi_request);

		}



		printf("\n");
	}

	//other processes. need to first receive message from previous process, then send to the next process.
	if(my_rank != 0)
	{
		printf("Received array:\n");


		for(int i = 0; i < N; i++)
		{
			MPI_Request mpi_request_receive;
			MPI_Recv(&buffer_receiver, MAX_MESSAGE_SIZE, MPI_INT, previous_process,  DUMMY_TAG, MPI_COMM_WORLD, &mpi_request_receive);

		}

	}

	return 0;

}