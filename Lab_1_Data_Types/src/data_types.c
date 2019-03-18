
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct square_matrix
{
	char * matrix_name;
	int matrix_amount;

} square_matrix;


int main(int argc, char ** argv)
{
	int my_rank;

	MPI_Init(&argc, &argv);


	//MATRIX DATA TYPE DEFINITION BEGIN
	//need to allocate memory for it?

	square_matrix my_matrix = { "Test", 1};

	int array_of_blocklengths[] = { 1, 1 };

	//offset of a certain part of the data type. must be specified for every single field.
	MPI_Aint array_displacements[] = {0, (void *) &my_matrix.matrix_name - ((void *) &my_matrix)};

	MPI_Datatype array_of_types[] = { MPI_CHAR, MPI_INT };
	MPI_Datatype my_type;

	MPI_Type_create_struct(2, array_of_blocklengths, array_displacements,
	                        array_of_types, &my_type );

	MPI_Type_commit( &my_type );
	//MATRIX DATA TYPE DEFINITION END


	const int MAX_MESSAGE_SIZE = 100;
	//buffer for receiving messages
	//struct my_matrix received_message[MAX_MESSAGE_SIZE];

	//basic communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);



	if(my_rank == 0)
	{
		//after sending, start receiving
		MPI_Send(&my_matrix, 1, my_type , 1, 789, MPI_COMM_WORLD);
	}

	if(my_rank == 1)
	{
		//after receiving, start sending.
		MPI_Recv(&my_matrix, 1, my_type, 0, 789, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Received message at process %d is: %s\n", my_rank, my_matrix.matrix_amount);
		//mpi_error_code = MPI_Send(my_message_1_0, strlen(my_message_1_0)+1,MPI_CHAR,0, 789, MPI_COMM_WORLD);
	}



	MPI_Finalize();

	return 0;

}
