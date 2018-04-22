#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "qsort.h"
#include "timing.h"

/* Returns true if the given list is sorted in ascending order */
int is_sorted(int *list, size_t length)
{
	int retVal = 1;
	size_t i;
	#pragma omp parallel for private(i)
	for (i = 1; i < length; ++i)
	{
		if (list[i - 1] > list[i])
		{
			#pragma omp atomic write
			retVal = 0;
		}
	}
	return retVal;
}

/* Creates a list and generates random int values.
   The invoker of this function owns the created list */
int *random_generate_list(size_t length)
{
	int *list = malloc(sizeof(int) * length);
	srand(time(0));
	size_t i;
	for (i = 0; i < length; ++i)
		list[i] = rand() % 1000;
	return list;
}

int main(int argc, const char *argv[])
{
	printf("Running OpenMP %d\n", _OPENMP);
	size_t length = 0;
	{
		int i;
		for (i = 1; i < argc; ++i)
		{
			if (!strcmp("--length", argv[i]))
				length = strtoul(argv[++i], NULL, 10);
		}
	}
	if (length <= 0)
	{
		printf("Usage: proj --length [length]\n");
		return 1;
	}
	
	int *list = random_generate_list(length);
	assert(!is_sorted(list, length));
	int *list2 = malloc(sizeof(int) * length);
	memcpy(list2 /* dest */, list /* src */, sizeof(int) * length);
		
	{
		START_TIMING(start);
		quicksort(list, length);
		PRINT_TIMING("Elapsed time (single thread): %.3fms, ", start);
		printf("is_sorted: %d\n", is_sorted(list, length));
	}
	
	{
		START_TIMING(start);
		quicksort_parallel(list2, length);
		PRINT_TIMING("Elapsed time (parallel): %.3fms, ", start);
		printf("is_sorted: %d\n", is_sorted(list2, length));
	}
	
	free(list);
	free(list2);
	
	return 0;
}