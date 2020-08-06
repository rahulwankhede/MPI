#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE * fp = fopen("./input_files/q2-sample-input.txt", "rb");
	int my_int;
	fseek(fp, sizeof(int) + sizeof(char), 0);
	fscanf(fp, "%d", &my_int);
	printf("%d\n", my_int);
	fclose(fp);

	return 0;

}
