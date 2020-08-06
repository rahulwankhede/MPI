#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 1000000
#define TEST_SIZE 50
#define MAX_INT 5000000

void read_file(FILE * fp, int * array, int size, int * test_array, int test_size)
{
	int i, count = 0;
	for(i = 0; i < size; i++, count++)
	{
		fscanf(fp, "%d", &array[i]);
	}
	if(count != SIZE)
	{
		printf("Did not read exactly million elements. Exiting..\n");
		exit(EXIT_FAILURE);
	}

	int j, test_count = 0;
	for(j = 0; j < test_size; j++, test_count++)
	{
		fscanf(fp, "%d", &test_array[j]);
	}
	if(test_count != test_size)
	{
		printf("Did not read exactly 50 elements. Exiting..\n");
		exit(EXIT_FAILURE);
	}
}

int get_index(int * array, int size, int elem)
//	Returns index of elem in array of length size
//	-1 if not found
{
	for(int i = 0; i < size; ++i)
	{
		if(array[i] == elem)
		{
			return  i;
		}
	}
	return -1;
}



int main(int argc, char * argv[])
{

	int my_rank, size, global_pos = -1;

	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Request request;
	MPI_Status status;

	int done, myfound, inrange, nvalues;
	int i, j, dummy;

	FILE * fp;

	MPI_Init(&argc, &argv);
  	MPI_Comm_size(comm, &size);
	MPI_Comm_rank(comm, &my_rank);

	myfound = 0;

	if(my_rank == 0)
	{
		fp = fopen("./input_files/q2-sample-input.txt", "r");
		if(fp == NULL)
		{
			printf("Could not open file for reading. Exiting..\n");
			exit(EXIT_FAILURE);
		}

		int * array = (int *) malloc(SIZE * sizeof(int));
		if(array == NULL)
		{
			printf("Could not allocate space for million ints. Exiting..\n");
			exit(EXIT_FAILURE);
		}

		int * test_array = (int *) malloc(TEST_SIZE * sizeof(int));
		if(test_array == NULL)
		{
			printf("Could not allocate space for 50 ints. Exiting..\n");
			exit(EXIT_FAILURE);
		}
	
		read_file(fp, array, SIZE, test_array, TEST_SIZE);

		fclose(fp);


	}
	
	//MPI_Bcast(array, SIZE, MPI_INT, 0, comm);
	//MPI_IRecv(&dummy, 1, )

	int elem = test_array[5];
	//printf("%d", elem);


	int other_i, other_rank, flag;


	other_rank = ((my_rank == 0) ? 1 : 0);

	if(size != 2)
	{
		printf("This program works with only two processes.\n");
		exit(EXIT_FAILURE);
	}
	
	//int * output_array = (int *) malloc(TEST_SIZE * sizeof(int));

	for(int i = 0; i < SIZE/2; i++)
	{
		MPI_Irecv(&other_i, 1, MPI_INT, other_rank, 10, comm, &request);
		if(array[i] == elem)
		{
			MPI_Send(&i, 1, MPI_INT, other_rank, 10, comm);
			if(my_rank == 0)
			{
				global_pos = i;
			}
			break;
		}
		else
		{
			MPI_Test(&request, &flag, &status);
			if (flag == 1)
			{
				MPI_Wait(&request, &status);
				if(my_rank == 0)
				{
					global_pos = other_i + SIZE/2;
				}
			break;
		}
	}

	if(my_rank == 0)
	{
		printf("Found the element %d in position %d\n", elem, global_pos);
	}




	MPI_Finalize();

	return EXIT_SUCCESS;

}
