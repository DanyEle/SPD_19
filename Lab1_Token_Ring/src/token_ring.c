
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//forward declarations
int handle_communication(int my_rank, int number_processes, int MAX_MESSAGE_SIZE, int send_integer);


int main(int argc, char ** argv)
{
	int my_rank;

	int number_processes;

	const int MAX_MESSAGE_SIZE = 1;

	int initial_integer = 23;

	MPI_Init(&argc, &argv);
	//basic communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int mpi_error_code = MPI_Comm_size(MPI_COMM_WORLD, &number_processes);

    mpi_error_code = handle_communication(my_rank, number_processes, MAX_MESSAGE_SIZE, initial_integer);

    //send to the next process

	MPI_Finalize();

	return 0;

}



int handle_communication(int my_rank, int number_processes,int MAX_MESSAGE_SIZE, int integer_send)
{
	printf("I am in process %d \n", my_rank);

	//just to received a single integer
	int values[MAX_MESSAGE_SIZE];

	int mpi_error_code;
	int next_process = (my_rank + 1) % number_processes;
	int previous_process = (my_rank - 1) % number_processes;

	while(integer_send < 1000)
	{

		//root, needs to init the communication
		if(my_rank == 0)
		{

			//first one needs to send to the next one
			mpi_error_code = MPI_Send(&integer_send, MAX_MESSAGE_SIZE, MPI_INT, next_process, 789, MPI_COMM_WORLD);

			//then needs to start receiving from the previous one
			mpi_error_code = MPI_Recv(&values, MAX_MESSAGE_SIZE, MPI_INT, previous_process, 789, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			integer_send = values[0] + 1;

			printf("Received message at process %d is %d  \n", my_rank, values[0]);

		}

		//other processes. need to first receive message from previous process, then send to the next process.
		if(my_rank != 0)
		{

			mpi_error_code = MPI_Recv(&values, MAX_MESSAGE_SIZE, MPI_INT, previous_process, 789, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Received message at process %d is %d  \n", my_rank, values[0]);

			integer_send = values[0] + 1;

			mpi_error_code = MPI_Send(&integer_send, MAX_MESSAGE_SIZE, MPI_INT, next_process, 789, MPI_COMM_WORLD);

		}
	}



	return 0;

}
