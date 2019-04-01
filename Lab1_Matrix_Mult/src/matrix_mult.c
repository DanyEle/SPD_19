
//Credits: https://gist.github.com/rehrumesh/b103636b6337baafb52f
//for some inspiration

#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>


void handle_master(int numworkers);
void handle_worker();


// number of rows and columns in matrix (square matrix)

//need for as many workers as the amount of rows
#define N 3
//value set in matrix A
#define A_VALUE 5
//value set in matrix B
#define B_VALUE 5




#define MASTER_RANK 0



int a[N][N],b[N][N],c[N][N];




//Por


int main(int argc, char ** argv)
{
	 //int numtasks,taskid,numworkers,source,rows,offset,i,j,k;
	 int numtasks;
	 int taskid;

	 //initialize every single process.
	 MPI_Init(&argc, &argv);
	 MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	 MPI_Comm_size(MPI_COMM_WORLD, &numtasks);


     if(numtasks <= 1)
     {
    	printf("You have provided %d processes. At least 2 processes are required though (1 master and 1 worker.\n", numtasks);
    	return - 1;
     }

     int numworkers = numtasks - 1;

     //MASTER --> Handle the matrix distribution.
     if (taskid == MASTER_RANK)
     {
    	 handle_master(numworkers);

     }

     if(taskid != MASTER_RANK)
     {
    	 handle_worker(numworkers);
     }

     //handle the sending of the matrix to the workers

	MPI_Finalize();

	return 0;

}

//Handle the distribution of the matrix among the workers.
void handle_master(int numworkers)
{
	printf("M: Sending work to every single worker.\n");

	int rows = N/numworkers;
	int offset = 0;


	for (int i=0; i<N; i++)
	{
	  for (int j=0; j<N; j++)
	  {
		a[i][j]= A_VALUE;
		b[i][j]= B_VALUE;
	  }
	}

	//send the matrices to the workers and the part they will need to process.
	for (int dest = 1; dest <= numworkers; dest++)
	{
		MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
		MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
		MPI_Send(&a[offset][0], rows*N, MPI_INT,dest,1, MPI_COMM_WORLD);
		MPI_Send(&b, N*N, MPI_INT, dest, 1, MPI_COMM_WORLD);
		offset = offset + rows;
	}

	MPI_Status status;

	//now start receiving the results from the workers
	for (int i = 1; i <= numworkers; i++)
	{
	  MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
	  MPI_Recv(&rows, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
	  MPI_Recv(&c[offset][0], rows*N, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
	}

	//print the resulting matrix
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			printf("%d ", c[i][j]);
		}
		printf("\n");

	}



}

void handle_worker()
{
	int offset, rows;
	MPI_Status status;

	MPI_Recv(&offset, 1, MPI_INT, MASTER_RANK, 1, MPI_COMM_WORLD, &status);
	MPI_Recv(&rows, 1, MPI_INT, MASTER_RANK, 1, MPI_COMM_WORLD, &status);
	MPI_Recv(&a, rows*N, MPI_DOUBLE, MASTER_RANK, 1, MPI_COMM_WORLD, &status);
	MPI_Recv(&b, N*N, MPI_DOUBLE, MASTER_RANK, 1, MPI_COMM_WORLD, &status);

	/* Matrix multiplication - O(n^3) */
	for (int k=0; k<N; k++)
	{
	  for (int i=0; i<rows; i++)
	  {
		c[i][k] = 0;

		for (int j=0; j<N; j++)
		{
		  c[i][k] = c[i][k] + a[i][j] * b[j][k];
		}
	  }
	}

	//send back the results to the master
	MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
	MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
	MPI_Send(&c, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);

}



