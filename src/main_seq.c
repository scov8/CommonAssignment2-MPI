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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include "utils.h"

void countingSort(int A[], int C[], int length, int k);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("ERROR! Usage: ./main dimarray ");
        exit(1);
    }
    int dimArray = atoi(argv[1]); //write the dimension of array

    int *arr = (int *)calloc(dimArray, sizeof(int));
    int *output = (int *)calloc(dimArray, sizeof(int));

    double generateArray_elapsed;
    double countingSort_elapsed;

    //========= START TAKE TIME FOR THE GENERATE ARRAY
    STARTTIME(0);
    generateArray(arr, dimArray); //populating the array
    ENDTIME(0, generateArray_elapsed);
    //============= END FIRST TAKE TIME

    int k = getMax(arr, dimArray);

    //============= START TAKE TIME FOR THE SORTING OF ARRAY
    STARTTIME(1);
    countingSort(arr, output, dimArray, k); //sort array
    ENDTIME(1, countingSort_elapsed);
    //============= END SECOND TAKE TIME

    double elapsed = countingSort_elapsed + generateArray_elapsed;

    printf("%d,0,%f,%f,%f\n", dimArray, generateArray_elapsed, countingSort_elapsed, elapsed);

    free(arr);
    free(output);
    return 0;
}

/**
 * @brief           The application of the counting sort algorithm.
 *
 * @param arr       input array
 * @param output    array where we want save the sorted arr
 * @param length    length of arr
 * @param k max of array
 */
void countingSort(int arr[], int output[], int length, int k)
{
    int *count = (int *)calloc(k + 1, sizeof(int));
    for (int i = 0; i <= length - 1; i++)
        count[arr[i]] += 1;

    for (int i = 1; i <= k; i++)
        count[i] += count[i - 1];

    for (int i = 0; i < length; i++)
    {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }
    free(count);
}