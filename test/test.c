/*
 * Course: High Performance Computing 2021/2022
 *
 * Lecturer: Francesco Moscato	fmoscato@unisa.it
 *
 * Group:
 * Rosa Gerardo	     0622701829	 g.rosa10@studenti.unisa.it
 * Scovotto Luigi    0622701702  l.scovotto1@studenti.unisa.it
 * Tortora Francesco 0622701700  f.tortora21@studenti.unisa.it
 *
 * Copyright (C) 2021 - All Rights Reserved
 *
 * This file is part of CommonAssignment1.
 *
 * Requirements: Parallelize and Evaluate Performances of "COUNTING SORT" Algorithm ,by using MPI.
 *
 * The previous year's group 02 files proposed by the professor during the course were used for file generation and extraction.
 *
 * The counting sort function for test case n. 1 was taken here:
 * https://github.com/ianliu/programacao-paralela/blob/master/omp-count-sort/main.c
 *
 * CommonAssignment1 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CommonAssignment1 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CommonAssignment2.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You can find the complete project on GitHub:
 * https://github.com/scov8/CommonAssignment2-Team02
 */

/**
	@file test.c
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define LENGTH 20
#define MAX_VALUE 10000
#include "utils.h"
void countingSort(int arr[], int output[], int length, int k);

int main(int argc, char *argv[])
{
	int *array = (int *)malloc(LENGTH * sizeof(int));
	int *sorted = (int *)malloc(LENGTH * sizeof(int));
	int *sortedMPI = (int *)malloc(LENGTH * sizeof(int));
	int res = 0, count = 0;
	FILE *fd;

	fd = fopen("../test/init.bin", "r");
	if (fd == NULL)
	{
		fprintf(stderr, "Error in opening the file");
		exit(1);
	}

	res = fread(&array[count++], sizeof(int), 1, fd);
	while (res == 1 && count < LENGTH)
	{
		res = fread(&array[count], sizeof(int), 1, fd);
		count++;
	}
	if (res != 1)
	{
		fprintf(stderr, "Error in reading the file");
		exit(1);
	}

	countingSort(array, sorted, LENGTH, getMax(array, LENGTH));

	fd = fopen("../test/sorted.bin", "r");
	if (fd == NULL)
	{
		fprintf(stderr, "Error in opening the file");
		exit(1);
	}
	count = 0;
	res = fread(&sortedMPI[count++], sizeof(int), 1, fd);
	while (res == 1 && count < LENGTH)
	{
		res = fread(&sortedMPI[count], sizeof(int), 1, fd);
		count++;
	}
	if (res != 1)
	{
		fprintf(stderr, "Error in reading the file");
		exit(1);
	}
	fclose(fd);

	for (int i = 0; i < LENGTH; i++)
	{
		if (sorted[i] != sortedMPI[i])
		{
			fprintf(stderr, "\nError found at index %d, expected %d, actual %d\n", i, sorted[i], sortedMPI[i]);
			exit(EXIT_FAILURE);
		}
	}

	printf("Test done\n");
	exit(EXIT_SUCCESS);
}

/**
 * @brief
 *
 * @param arr
 * @param output
 * @param length
 * @param k max of array
 */
void countingSort(int arr[], int output[], int length, int k)
{
	int *count = (int *)calloc(k + 1, sizeof(int));
	for (int i = 0; i <= length - 1; i++)
		count[arr[i]] += 1;

	for (int i = 1; i <= k; i++)
		count[i] += count[i - 1];

	for (int i = length - 1; i >= 0; i--)
	{
		output[count[arr[i]] - 1] = arr[i];
		count[arr[i]]--;
	}
	free(count);
}
