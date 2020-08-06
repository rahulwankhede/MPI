#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUM_ELEMS 1000000
#define NUM_QUERIES 50

void read_file(char * input_file, char * query_file, int * my_array, int size, int * query_array, int num_queries, int myrank, int p)
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
	if (ferror(query_fp))
	{
		printf("Error while reading query file\n");
		exit(EXIT_FAILURE);
	}

	fclose(input_fp);
	fclose(query_fp);
}


int main(int argc, char * argv[])
{
	MPI_Init(&argc, &argv);

	int p, myrank;
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	int size = NUM_ELEMS/p;
	int * my_array = (int *) malloc(size * sizeof(int));
	int * query_array = (int *) malloc(NUM_QUERIES * sizeof(int));

	read_file("./input_files/binary/q2-sample-elements.dat", "./input_files/binary/q2-sample-queries.txt", my_array, size, query_array, NUM_QUERIES, myrank, p);
	
	FILE * output_file = fopen("./input_files/binary/my_output.txt", "w");

	for(int num = 0; num < NUM_QUERIES; num++)
	{
		int test_elem = query_array[num];
	
		int flag = 0;			 // for testing if some process has found it by Ireceiving into a request and testing that request's flag if it has completed
		int message;			 // buffer to recieve message of having found the element into
		MPI_Request request;	 // to pass to MPI_Test to see if the Irecv has occured
		MPI_Status status;		 // for holding status which is actually not used but must be passed as an argument to Test
		int message_tag = num;	 // some commonn tag for this message

		MPI_Irecv(&message, 1, MPI_INT, MPI_ANY_SOURCE, message_tag, MPI_COMM_WORLD, &request); // non-blockingly receive message of some process having found the element that I'm searching
		MPI_Test(&request, &flag, &status);			 											// check if previous request has completed and make flag true if it has

		int i = 0;

		while(!flag && i < size)																// flag is changed to true only when Test has observed that the Irecv request has completed
		{
			if(my_array[i] == test_elem)
			{
				int global_index = myrank * size + i;
				message = global_index;
				for(int j = 0; j < p; j++)
				{
					MPI_Send(&message, 1, MPI_INT, j, message_tag, MPI_COMM_WORLD);				// send message to all processes (including self) so they can receive at IRecv above
				}
			}
			
			MPI_Test(&request, &flag, &status);													// check if request has completed in every iteration
			++i;																				// increment index variable
		}

		if(myrank == 0)
		{
			if(!flag)
			{
				fprintf(output_file, "-1\n");
			}
			else
			{
				fprintf(output_file, "%d\n", message);
			}
		}

	}

	fclose(output_file);

	MPI_Finalize();
	
	return EXIT_SUCCESS;

}

