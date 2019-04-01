
#include <mpi.h>
#include <omp.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


//forward declarations
void emitter(int number_processes, int my_rank);
void collector(int number_processes);
void worker(int my_rank, int number_processes);
int random_in_range(int minimum_number, int max_number, int my_rank);


//global variables for handling the communication
#define ID_EMITTER_RANK 0
#define ID_COLLECTOR_RANK 1

#define EMITTER_TAG 500
#define COLLECTOR_TAG 501

//N = amount of values to send in the data stream (data stream size)
#define N 10
//M = value being sent
#define END_OF_STREAM 0
#define CONSTANT_WAITING_TIME 1000000
//random time to wait for
#define LOWER_BOUND_RANDOM_TIME  0
#define UPPER_BOUND_RANDOM_TIME 2000000

#define WORKER_WORK_REQUEST -10

//output type not necessarility = input


int main(int argc, char ** argv)
{
	//my current process rank
	int my_rank;
	//total amount of processes
	int number_processes;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &number_processes);

	if(number_processes <= 2)
	{
		printf("Cannot start a farm with %d processes. Required: 1 processor for emitter; 1 processor for collector; at least 1 worker", number_processes);
		return -1;
	}

    //emitter
    if(my_rank == ID_EMITTER_RANK)
    {
    	emitter(number_processes, my_rank);
    }
    //collector
    else if(my_rank == ID_COLLECTOR_RANK)
    {
    	collector(number_processes);
    }
    //worker
    else if(my_rank != ID_EMITTER_RANK && my_rank != ID_COLLECTOR_RANK)
    {
    	worker(my_rank, number_processes);
    }

	MPI_Finalize();
	return 0;
}

int custom_function(int inputI, int my_rank)
{
	//constant waiting time of 1 second.
	//wait for a random amount [0 - 2 seconds]

	int rand_waiting_time = random_in_range(LOWER_BOUND_RANDOM_TIME, UPPER_BOUND_RANDOM_TIME, my_rank);
	printf("Waiting for %d\n", rand_waiting_time);
	usleep(rand_waiting_time);
	//usleep(CONSTANT_WAITING_TIME);

	return inputI * 2;
}

int random_in_range(int minimum_number, int max_number, int my_rank)
{
	 int seed = time(NULL) * my_rank;
	 srand(seed);

	return (rand() % (max_number + 1 - minimum_number) + minimum_number);
}



void emitter(int number_processes, int my_rank)
{
	//use a flag variable signalising whether more work is available.

	int i = 0;
	//emitter keeps waiting for workers to send requests for data as long as the stream has not been exhauster
	int amount_EOS_received = 0;
	while(1 == 1)
	{
		int worker_destination = (i % (number_processes - 2)) + 2;
		int value_received = -1;
		MPI_Recv(&value_received, 1, MPI_INT, worker_destination, EMITTER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//received an EOS msg from a worker, so no need to send anything back.
		if(value_received == END_OF_STREAM)
		{
			amount_EOS_received++;
		}
		//received a request for work
		else
		{
			int value_send = random_in_range(LOWER_BOUND_RANDOM_TIME, UPPER_BOUND_RANDOM_TIME, my_rank);

			MPI_Send(&value_send, 1, MPI_INT, worker_destination, EMITTER_TAG, MPI_COMM_WORLD);
		}

		if(amount_EOS_received == (number_processes - 2))
		{
			break;
		}
		i++;
	}
}

void worker(int my_rank, int number_processes)
{
	//receive data.
	//printf("W: In the worker with PID %d \n", my_rank);
	//i represents the amount of values received by the present worker
	int i = 0;

	int stream_size_worker = (int) N / number_processes;

	while(i < stream_size_worker)
	{
		//send a request for work to the emitter
		int work_request = WORKER_WORK_REQUEST;
		MPI_Send(&work_request, 1, MPI_INT, ID_EMITTER_RANK, EMITTER_TAG, MPI_COMM_WORLD);

		//and then start receiving a value from the emitter (the actual work to be carried out)
		int value_received = -1;
		MPI_Recv(&value_received, 1, MPI_INT, ID_EMITTER_RANK, EMITTER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		printf("W: Value received %d \n", value_received);

		//we are sure to have some work here.
		int processed_value = custom_function(value_received, my_rank);

		MPI_Send(&processed_value, 1, MPI_INT, ID_COLLECTOR_RANK,COLLECTOR_TAG, MPI_COMM_WORLD);
		printf("W: Sending %d to collector \n", processed_value);
		i++;
	}

	//now need to send an EOS to the emitter, because the stream being sent has finished.
	int eos_msg = END_OF_STREAM;
	MPI_Send(&eos_msg, 1, MPI_INT, ID_EMITTER_RANK, EMITTER_TAG, MPI_COMM_WORLD);

	//and send an EOS to the collector as well
	MPI_Send(&eos_msg, 1, MPI_INT, ID_COLLECTOR_RANK, COLLECTOR_TAG, MPI_COMM_WORLD);

}


void collector(int number_processes)
{
	//receive from every single worker
	//printf("C: In the collector \n");
	int buffer_received[N];

	int amount_EOS = 0;
	int i = 0;
	int pid = 0;
	while(1 == 1)
	{
		//receive in a round-robin fashion from all worker processes
		int worker_receive = pid % (number_processes - 2) + 2;
		//printf("C: Receiving from worker %d \n", worker_receive);
		int value_received = -1;
		MPI_Recv(&value_received, 1, MPI_INT, worker_receive, COLLECTOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//received an EOS from a worker
		if(value_received == END_OF_STREAM)
		{
			amount_EOS++;
		}
		//no EOS received, then just add it the element to the buffer
		//else
		//{
		//	buffer_received[i] = value_received;
		//	i++;
		//}
		//if all the workers have sent an EOS, then stop receiving values
		if(amount_EOS == (number_processes - 2))
		{
			break;
		}
		pid++;
	}

}
