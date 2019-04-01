#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct car_s {
        int shifts;
        int topSpeed;
} car;

#define PID_RECEIVER 1
#define PID_SENDER 0


int main(int argc, char ** argv)
{
	const int tag = 13;

	int my_rank;
	int size;
	MPI_Init(&argc, &argv);
	//basic communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* create a type for struct car */
	const int nitems=2;
	int          blocklengths[2] = {1,1};
	MPI_Datatype types[2] = {MPI_INT, MPI_INT};
	MPI_Datatype mpi_car_type;
	MPI_Aint     offsets[2];

	//offers =  {0, (void *) &car.shifts - ((void *) &car)};

	offsets[0] = offsetof(struct car_s, shifts);

	offsets[1] = offsetof(struct car_s, topSpeed);

	MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_car_type);
	MPI_Type_commit(&mpi_car_type);

	//sender --> Need to send the created car data structure
	if(my_rank == 0)
	{
		//init the struct
		car car_send;
		car_send.shifts = 4;
		car_send.topSpeed = 100;

		MPI_Send(&car_send, 1, mpi_car_type, PID_RECEIVER, tag, MPI_COMM_WORLD);
	}

	//receiver --> need to receive the created data structure and get the data it was initialized with.
	if(my_rank == 1)
	{
		MPI_Status status;

		car car_received;

		MPI_Recv(&car_received, 1, mpi_car_type, PID_SENDER, tag, MPI_COMM_WORLD, &status);
		printf("Received: shifts = %d topSpeed = %d\n", car_received.shifts, car_received.topSpeed);
	}

	MPI_Finalize();

	return 0;


}
