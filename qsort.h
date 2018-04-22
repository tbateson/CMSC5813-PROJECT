#include <stdlib.h>

/* Swaps the value contained at the a & b pointers */
void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

/* Partitions the list less than the pivot to the left, and greater than to the right. Returns the pivot index */
size_t partition(int *list, size_t lo /* lo is inclusive */, size_t hi /* hi is exclusive */)
{
	int pivot = list[hi - 1];
	size_t i, j;
	for (i = lo - 1, j = lo; j < hi; ++j)
	{
		if (list[j] < pivot)
		{
			swap(&list[++i], &list[j]);
		}
	}
	swap(&list[++i], &list[hi - 1]);
	return i;
}

/* Runner function for quicksort */
void quicksort_recursive(int *list, size_t lo, size_t hi)
{
	size_t pivot = partition(list, lo, hi);
	if (lo < pivot)
		quicksort_recursive(list, lo, pivot);
	if (pivot + 1 < hi)
		quicksort_recursive(list, pivot + 1, hi);
}

/* Sorts a list using a single thread */
void quicksort(int *list, size_t length)
{
	if (length > 1)
		quicksort_recursive(list, 0, length);
}

/* Runner function for quicksort_parallel */
void quicksort_recursive_parallel(int *list, size_t lo, size_t hi)
{
	size_t pivot = partition(list, lo, hi);
	if (lo < pivot)
		#pragma omp task
		quicksort_recursive_parallel(list, lo, pivot);
	if (pivot + 1 < hi)
		#pragma omp task
		quicksort_recursive_parallel(list, pivot + 1, hi);
}

/* Sorts a list using multiple threads */
void quicksort_parallel(int *list, size_t length)
{
	if (length > 1)
	{
		#pragma omp parallel
		#pragma omp single
		{
			quicksort_recursive_parallel(list, 0, length);
			#pragma omp taskwait
		}
	}
}