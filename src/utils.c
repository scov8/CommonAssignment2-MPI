/*
* Course: High Performance Computing 2021/2022
*
* Lecturer: Francesco Moscato	fmoscato@unisa.it
*
* Group:
* Rosa Gerardo	    0622701829	g.rosa10@studenti.unisa.it
* Scovotto Luigi    0622701702  l.scovotto1@studenti.unisa.it
* Tortora Francesco 0622701700  f.tortora21@studenti.unisa.it
*
* Copyright (C) 2022 - All Rights Reserved
* This file is part of CommonAssignment2.
*
* Requirements: Parallelize and Evaluate Performances of "COUNTING SORT" Algorithm ,by using MPI.
*
* The previous year's group 02 files proposed by the professor during the course were used for file generation and extraction.
*
* The starting point for the counting sort function was from this video:
* https://www.youtube.com/watch?v=qcOoEjdYSz0
*
* CommonAssignment2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* CommonAssignment2 is distributed in the hope that it will be useful,
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
	@file utils.c
	@brief Utility functions for CommonAssignmentMPI01.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

/**
 * @brief           This function initializes the data structure to be sorted.
 *
 * @param arr       a pointer to an empty array which must be populated with random numbers.    
 * @param dim       size of arr.
 */
void generateArray(int arr[], int dim)
{
    int i;
    unsigned int seed = time(NULL);
    for (i = 0; i < dim; i++)
        arr[i] = rand_r(&seed) % 100000;
}

/**
 * @brief       Get the Max object.
 *
 * @param arr   array that we want calculate the maximum element.
 * @param n     dimension of array.
 * @return int  return the max element of array.
 */
int getMax(int *arr, int n)
{
    int max = arr[0];
    for (int i = 0; i < n; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

/**
 * @brief       Print array sorted after the application of counting sort algorithm.
 *
 * @param arr   ordered array.
 * @param len   dimension of array.
 */
void printArray_(int *arr, int len)
{
    for (int i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

/**
 * @brief       test function that check if a array is sorted.
 *
 * @param arr   array that we want check.
 * @param len   length of the array.  
 * @return      int 1 if is sorted, 0 if isn't.
 */
int check(int *arr, int len)
{
    for (int i = 1; i < len; i++)
        if (arr[i] < arr[i - 1])
            return 0;
    return 1;
}