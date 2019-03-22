
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//forward declarations
void emitter(int number_processes, MPI_Datatype input_type);
void collector(int number_processes, MPI_Datatype input_type);
void worker(int my_rank, MPI_Datatype input_type);

//struct car custom_function(struct car input_struct);


//global variables for handling the communication
#define ID_EMITTER_RANK 0
#define ID_COLLECTOR_RANK 1

#define EMITTER_TAG 500
#define COLLECTOR_TAG 501

//N = amount of values to send in the data stream.
#define N 10

//our very nice struct

typedef struct car_s {
        int shifts;
        int topSpeed;
} car;



int main(int argc, char ** argv)
{
	//my current process rank
	int my_rank;
	//total amount of processes
	int number_processes;

	MPI_Init(&argc, &argv);
	//basic communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &number_processes);

	if(number_processes <= 2)
	{
		printf("Cannot start a farm with %d processes. Required: 1 processor for emitter; 1 processor for collector; at least 1 worker", number_processes);
		return -1;
	}

	/* create a type for struct car */
	const int nitems=2;
	int          blocklengths[2] = {1,1};
	MPI_Datatype types[2] = {MPI_INT, MPI_INT};
	MPI_Datatype mpi_car_type;
	MPI_Aint     offsets[2];

	offsets[0] = offsetof(struct car_s, shifts);

	offsets[1] = offsetof(struct car_s, topSpeed);

	MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_car_type);
	MPI_Type_commit(&mpi_car_type);


    //emitter
    if(my_rank == ID_EMITTER_RANK)
    {
    	emitter(number_processes, mpi_car_type);
    }
    //collector
    else if(my_rank == ID_COLLECTOR_RANK)
    {
    	collector(number_processes, mpi_car_type);
    }
    else if(my_rank != ID_EMITTER_RANK && my_rank != ID_COLLECTOR_RANK)
    {
    	worker(my_rank, mpi_car_type);
    }

	MPI_Finalize();
	return 0;
}

car custom_function(car input_car)
{
	input_car.shifts += 1;
	input_car.topSpeed += 1;

	return input_car;
}


void emitter(int number_processes, MPI_Datatype input_type)
{
	//let's create an array of structs
	car buffer_cars_send[N];

	//init the struct
	car car_send;
	car_send.shifts = 4;
	car_send.topSpeed = 100;

	for(int i = 0; i < N; i++)
	{
		buffer_cars_send[i] = car_send;
	}


	//to every single process, send every single piece of data.
	for(int i = 1; i < number_processes; i++)
	{
		MPI_Send(&buffer_cars_send, N, input_type, i, EMITTER_TAG, MPI_COMM_WORLD);
		printf("E: Sending Data to Worker with PID: %d \n", i);
	}
}

void worker(int my_rank, MPI_Datatype input_type)
{
	//receive data.
	printf("W: In the worker with PID %d \n", my_rank);

	//receive the newly created struct
	car buffer_cars_received[N];
	car buffer_cars_to_send[N];

	MPI_Recv(&buffer_cars_received, N, input_type, ID_EMITTER_RANK, EMITTER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	//process the received data.
	for(int i = 0; i < N; i++)
	{
		buffer_cars_to_send[i] = custom_function(buffer_cars_received[i]);
		//printf("W: Shifts to send %d", buffer_cars_to_send[i].shifts);
	}

	//send the processed data to the collector.
	MPI_Send(&buffer_cars_to_send, N, input_type, ID_COLLECTOR_RANK,COLLECTOR_TAG, MPI_COMM_WORLD);
}



void collector(int number_processes, MPI_Datatype input_type)
{
	//receive from every single worker
	printf("C: In the collector \n");

	car buffer_all_cars_received[N * (number_processes - 2)];

	int j = 0;

	for(int i = 2; i < number_processes; i++)
	{
		car buffer_cars_received[N];
		MPI_Recv(&buffer_cars_received, N, input_type, i, COLLECTOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//take all the received cars and add them to a global array containing all of them
		for(int h = 0; h < N; h++)
		{
			buffer_all_cars_received[j] = buffer_cars_received[h];
			j++;
		}
	}

	//print all the resulting final structs received
	for(int i = 0; i < N * (number_processes - 2); i++)
	{
		printf("C: Received %d \n", buffer_all_cars_received[i].shifts);
	}

}
