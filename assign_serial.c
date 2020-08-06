#include <stdio.h>
#include <stdlib.h>

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

void test(int * array, int size)
{
	for(int i = 0; i < size; i++)
	{
		printf("%d\n", array[i]);
	}
	printf("\n");
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
	FILE * fp = fopen("./input_files/q2-sample-input.txt", "r");
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

	//test(array, 10);
	//test(test_array, 50);

	fclose(fp);

	FILE * o_fp = fopen("./input_files/my_sample_output.txt", "w");
	if(o_fp == NULL)
	{
		printf("Could not open file for writing. Exiting..\n");
		exit(EXIT_FAILURE);
	}
	
	int index;

	for(int i = 0; i < TEST_SIZE; i++)
	{
		int elem = test_array[i];
		index = get_index(array, SIZE, elem);
		fprintf(o_fp, "%d\n", index);
	}


	fclose(o_fp);


	return 0;
}

