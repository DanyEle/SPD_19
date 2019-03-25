
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
void collector(int number_processes, int my_rank);





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
    	collector(number_processes, my_rank);

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

	printf("C: Preparing work in the emitter\n");

	//send row and column to one single process.

	//loop through all the columns of the matrix A
	//loop through all the rows of the matrix B
	for(int i = 0; i < N; i++)
	{
		//columnA is one single column of the matrix A
		int columnA[N];
		for(int k = 0; i < N; i++)
		{
			columnA[k] = A[i][k];
		}

		//rowB is one single row of the matrix B
		int rowB[N];
		for(int j = 0; j < N; j++)
		{
			rowB[j] = B[j][i];
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
			//need to propagate the EOS to the collector too, actually
			MPI_Send(columnA, N, MPI_INT, COLLECTOR_RANK, COLLECTOR_TAG, MPI_COMM_WORLD);
			break;
		}
		else
		{
			printf("W: Received column A\n");
		}

		int rowB[N];
		MPI_Recv(&rowB, N, MPI_INT, EMITTER_RANK, EMITTER_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		printf("W: Received row B\n");

		//perform multiplication among elements in columnA and rowB
		int lineC[N];

		for(int i = 0; i < N; i++)
		{
			lineC[i] = columnA[i] * rowB[i];
		}

		//and now need to send lineC to the collector, which will need to perform the proper operations
		MPI_Send(lineC, N, MPI_INT, COLLECTOR_RANK, COLLECTOR_TAG, MPI_COMM_WORLD);
	}
}

void collector(int number_processes, int my_rank)
{
	//receive from every single worker
	//printf("C: In the collector \n");
	int buffer_received[N];
	int i = 0;
	int amount_EOS = 0;
	int pid = 0;
	while(1 == 1)
	{
		//receive in a round-robin fashion from all worker processes
		int worker_receive = pid % (number_processes - 2) + 2;
		//printf("C: Receiving from worker %d \n", worker_receive);
		int lineC[N];
		MPI_Recv(&lineC, N, MPI_INT, worker_receive, COLLECTOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


		//received an EOS from a worker
		if(lineC[0] == EOS_VALUE)
		{
			printf("Received an EOS\n");
			amount_EOS++;
		}
		//no EOS received, then sum all the elements in the array received
		else
		{
			int valueC = 0;

			for(int i = 0; i < N; i++)
			{
				valueC += lineC[i];
			}
			buffer_received[i] = valueC;
		}
		//if all the workers have sent an EOS, then stop receiving values
		if(amount_EOS == (number_processes - 2))
		{
			printf("C: All workers have sent an EOS \n");

			for(int i = 0; i < N; i++)
			{
				printf("Received %d \n", buffer_received[i]);
			}
			break;
		}
		pid++;
	}
}






