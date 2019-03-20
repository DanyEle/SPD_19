
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//forward declarations
void emitter(int number_processes);
void collector();
void worker(int my_rank);

//global variables for handling the communication
#define ID_EMITTER_RANK 0
#define ID_COLLECTOR_RANK 1

#define EMITTER_TAG 500
#define COLLECTOR_TAG 501

//N = amount of values to send in the data stream.
#define N 100000
//M = value being sent
#define M 950



int main(int argc, char ** argv)
{
	//my current process rank
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

int custom_function(int inputI)
{
	return inputI * inputI;
}

void emitter(int number_processes)
{
	int buffer_sender[N];

	for(int i = 0; i < N; i++)
	{
		buffer_sender[i] = M;
	}

	//to every single process, send every single piece of data.
	for(int i = 2; i < number_processes; i++)
	{
		MPI_Send(&buffer_sender, N, MPI_INT, i, EMITTER_TAG, MPI_COMM_WORLD);
		printf("E: Sending Data to Worker with PID: %d \n", i);
	}
}

void worker(int my_rank)
{
	//receive data.
	printf("W: In the worker with PID %d \n", my_rank);
	int buffer_receive[N];
	MPI_Recv(&buffer_receive, N, MPI_INT, ID_EMITTER_RANK, EMITTER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	int buffer_send[N];

	//process the received data.
	for(int i = 0; i < N; i++)
	{
		buffer_send[i] = custom_function(buffer_receive[i]);
	}

	//send the processed data to the collector.
	MPI_Send(buffer_send, N, MPI_INT, ID_COLLECTOR_RANK,COLLECTOR_TAG, MPI_COMM_WORLD);
}



void collector(int number_processes)
{
	//receive from every single worker
	printf("C: In the collector \n");

	int buffer_all_workers_vals_recv[N * (number_processes - 2)];

	int j = 0;
	for(int i = 2; i < number_processes; i++)
	{
		int buffer_recv_single_worker[N];
		MPI_Recv(&buffer_recv_single_worker, N, MPI_INT, i, COLLECTOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//finally, print the output of all values received.
		for(int i = 0; i < N; i++)
		{
			buffer_all_workers_vals_recv[j] = buffer_recv_single_worker[i];
			j++;
		}
	}

	for(int i = 0; i < N * (number_processes - 2); i++)
	{
		printf("C: Received value %d \n", buffer_all_workers_vals_recv[i]);

	}


}
