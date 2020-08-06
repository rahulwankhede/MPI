#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 1000000
#define MAX_INT 5000000
#define SEED 100

int find_elem(int * array, int size, int elem)
// returns index of elem in array of length size if found, -1 otherwise
{
	int i;
	for(i = 0; i < size; ++i)
	{
		if(array[i] == elem)
		{
			return i;
		}
	}
	return -1;
}



int main(int argc, char * argv[])
{
	MPI_Init(&argc, &argv);

	int * array = (int *) calloc(SIZE, sizeof(int));

	if(array == NULL)
	{
		printf("Could not allocate space on heap for array of million doubles");
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	srand(SEED);

	int i;
	for(i = 0; i < SIZE; ++i)
	{
		array[i] =  rand() % MAX_INT;
	}
	
	int random = 54354;
	printf("%d\n", random);
	
	int index = find_elem(array, SIZE, random);
	
	printf("Element %d is at index %d\n", random, index);


	MPI_Finalize();

	return EXIT_SUCCESS;

}
