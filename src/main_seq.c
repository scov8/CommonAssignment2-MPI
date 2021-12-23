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

    //=========START TAKE TIME FOR THE GENERATE ARRAY
    struct tms generateArray_start_times;
    clock_t generateArray_start_etime;
    generateArray_start_etime = times(&generateArray_start_times);

    generateArray(arr, dimArray); //popolamento dell array

    struct tms generateArray_end_times;
    clock_t generateArray_end_etime;
    generateArray_end_etime = times(&generateArray_end_times);
    //=============END FIRST TAKE TIME===============

    int k = getMax(arr, dimArray);

    //---------start take time
    struct tms countingSort_start_times;
    clock_t countingSort_start_etime;
    countingSort_start_etime = times(&countingSort_start_times);

    countingSort(arr, output, dimArray, k); //sort array

    struct tms countingSort_end_times;
    clock_t countingSort_end_etime;
    countingSort_end_etime = times(&countingSort_end_times);
    //---------end take time

    /*printf("L'array ordinato è:\n");
    for (int i = 0; i < dimArray; i++)
        printf("%d ", output[i]);*/

    //Test
    /*
    int flag = 1;
    for (int i = 1; i < dimArray; i++)
        if (output[i] < output[i - 1])
            flag = 0;

    if (flag == 1)
        printf("\nL'algoritmo funziona");
    else
        printf("\nL'algoritmo NON funziona");
    */

    // Get clock ticks per sec
    long clktck = 0;
    if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
    {
        fprintf(stderr, "ERROR: filed to get slock ticks per sec\n");
        exit(EXIT_FAILURE);
    }

    double generateArray_elapsed = (generateArray_end_etime - generateArray_start_etime) / (double)clktck;
    double countingSort_elapsed = (countingSort_end_etime - countingSort_start_etime) / (double)clktck;

    double elapsed = countingSort_elapsed + generateArray_elapsed;

    printf("%d,0,%f,%f,%f\n", dimArray, generateArray_elapsed, countingSort_elapsed, elapsed);

    free(arr);
    free(output);
    return 0;
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
