
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//forward declarations
void emitter(int number_processes);
void collector();
void worker(int my_rank);

int handle_communication(int my_rank, int number_processes, int MAX_MESSAGE_SIZE, int send_integer);

#define ID_EMITTER_RANK 0
#define ID_COLLECTOR_RANK 1

#define EMITTER_TAG 500
#define COLLECTOR_TAG 501

#define value_send 20



int main(int argc, char ** argv)
{
	//my current rank
	int my_rank;
	//total amount of processes
	int number_processes;


	MPI_Init(&argc, &argv);
	//basic communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int mpi_error_code = MPI_Comm_size(MPI_COMM_WORLD, &number_processes);

	if(number_processes <= 2)
	{
		printf("Cannot start a farm with %d processes. Required: 1 processor for emitter; 1 processor for collector; at least 1 worker", number_processes);
		return -1;
	}

    //emitter
    if(my_rank == ID_EMITTER_RANK)
    {
    	emitter(number_processes);
    }
    //collector
    else if(my_rank == ID_COLLECTOR_RANK)
    {
    	collector(number_processes);
    }
    else
    {
    	worker(my_rank);
    }


	MPI_Finalize();
	return 0;
}

void emitter(int number_processes)
{

	int value_emitter = value_send;


	for(int i = 2; i < number_processes; i++)
	{
		MPI_Send(&value_emitter, 1, MPI_INT, i, EMITTER_TAG, MPI_COMM_WORLD);
		printf("Sending Data to Worker with PID: %d \n", i);
	}

	printf("E: In the emitter \n");
	//need to send function and data to all the workers with a broadcast operation.
}


void worker(int my_rank)
{
	printf("W: In the worker with PID %d \n", my_rank);
	int buffer_receive[1];

	MPI_Recv(&buffer_receive, 1, MPI_INT, ID_EMITTER_RANK, EMITTER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	int buffer_send[1];
	//square operation
	buffer_send[0] = buffer_receive[0] * buffer_receive[0];

	//send the the new value to the emitter now
	MPI_Send(buffer_send, 1, MPI_INT, ID_COLLECTOR_RANK,COLLECTOR_TAG, MPI_COMM_WORLD);
}


void collector(int number_processes)
{
	printf("C: In the collector \n");

	//need to receive from every single worker
	int buffer_receive[1];

	for(int i = 2; i < number_processes; i++)
	{
		MPI_Recv(&buffer_receive, 1, MPI_INT, i, COLLECTOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("C: Received value %d \n", buffer_receive[0]);

	}



}
