
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
	int my_rank;

	int number_of_processes;


	char * my_message_0_1 = "Ping";
	char * my_message_1_0 = "Pong";

	const int MAX_MESSAGE_SIZE = 100;
	//buffer for receiving messages
	char received_message[MAX_MESSAGE_SIZE];


	MPI_Init(&argc, &argv);
	//basic communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int mpi_error_code = MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

    while( 1 == 1)
    {
		if(my_rank == 0)
		{
			//after sending, start receiving

			mpi_error_code = MPI_Send(my_message_0_1, strlen(my_message_0_1)+1,MPI_CHAR,1, 789, MPI_COMM_WORLD);
			mpi_error_code = MPI_Recv(received_message, MAX_MESSAGE_SIZE, MPI_CHAR, 1, 789, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Received message at process %d is: %s\n", my_rank, received_message);

		}

		if(my_rank == 1)
		{
			//after receiving, start sending.
			mpi_error_code = MPI_Recv(received_message, MAX_MESSAGE_SIZE, MPI_CHAR, 0, 789, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Received message at process %d is: %s\n", my_rank, received_message);
			mpi_error_code = MPI_Send(my_message_1_0, strlen(my_message_1_0)+1,MPI_CHAR,0, 789, MPI_COMM_WORLD);
		}

    }

	MPI_Finalize();

	return 0;

}
