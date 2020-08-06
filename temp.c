#include <stdio.h>
#include <stdlib.h>

#define NUM_ELEMS 1000000
#define NUM_QUERIES 50

void read_file(char * input_file, char * query_file, int * my_array, int size, int * query_array, int num_queries)
{
	FILE * input_fp = fopen(input_file, "rb");
	FILE * query_fp = fopen(query_file, "r");

	long position = (long) myrank * size * sizeof(int);

	fseek(input_fp, position, SEEK_SET);
	int n_obj = (int) fread(my_array, sizeof(int), size, input_fp);
	if (n_obj != size)
	{
		printf("Wrong number of elements read. objects read = %d, size = %d\n", n_obj, size);
		exit(EXIT_FAILURE);
	}
	if (ferror(input_fp))
	{
		printf("Error while reading input file\n");
		exit(EXIT_FAILURE);
	}

	int query_count = 0;
	for(int i = 0; i < NUM_QUERIES; i++, query_count++)
	{
		fscanf(query_fp, "%d", &query_array[i]);
	}
	if(query_count != NUM_QUERIES)
	{
		printf("Did not read exactly 50 elements. Exiting..\n");
		exit(EXIT_FAILURE);
	}

	fclose(input_fp);
	fclose(query_fp);
}



int main(int argc, char * argv[])
{
	int size = NUM_ELEMS;
	int * my_array = (int *) malloc(size * sizeof(int));
	int * query_array = (int *) malloc(NUM_QUERIES * sizeof(int));


	read_file("./input_files/binary/q2-sample-elements.dat", "q2-sample-queries.txt", my_array, size, query_array, NUM_QUERIES, myrank, p);

	



	MPI_Finalize();
	return EXIT_SUCCESS;
}
