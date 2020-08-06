#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 1000000
//#define TEST_SIZE 50
//#define MAX_INT 5000000

void read_file(FILE * fp, int * array, int size)
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
}


int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Status status;
	MPI_Request request;
	int done, myfound, inrange, nvalues;
	int * array;
   	int i, j, dummy;
	FILE * infile;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	myfound = 0;

	if(rank == 0)
	{
		array = (int *) malloc(SIZE * sizeof(int));
		if (array == NULL)
		{
			printf("Could not allocate space for millions ints. Exiting..\n");
			exit(EXIT_FAILURE);
		}

		infile = fopen("./input_files/q2-sample-input.txt", "r");
		if(infile == NULL)
		{
			printf("Could not open file for reading. Exiting..\n");
			exit(EXIT_FAILURE);
		}

		read_file(infile, array, SIZE);

		fclose(infile);

	}

	MPI_Bcast(array, SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Irecv(&dummy, 1, )
