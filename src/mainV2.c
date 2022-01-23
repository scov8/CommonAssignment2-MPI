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

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"

/**
 * @brief
 *
 * @param arr
 * @param len
 * @return int
 */
int test(int *arr, int len)
{
    for (int i = 1; i < len; i++)
        if (arr[i] < arr[i - 1])
        {
            return 0;
            printf("NON FUNZIONA");
        }
    printf("Funziona");
    return 1;
}

/**
 * @brief           This function initializes the data structure to be sorted.
 *
 * @param arr       a pointer to an empty array which must be populated with random numbers.    
 * @param n         size of arr.
 * @param rank      unique number to identify process.
 * @param nprocs    number of process.
 */
void generateArrayParallel(int arr[], int n, int rank, int nprocs)
{
    int size;
    if (nprocs == 1)
        size = 0; //size of sub-arrays managing processes
    else
        size = n / (nprocs - 1);
    int leftElements = n - (size * (nprocs - 1)); //remaining elements

    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Status status;

    if (rank == 0)
    {
        int *arrRicv = (int *)calloc(size, sizeof(int *));
        srand(rank);

        for (int i = 1; i < nprocs; i++)
            MPI_Send(&arr[(i - 1) * size], size, MPI_INT, i, 1, MPI_COMM_WORLD);

        for (int i = n - leftElements; i < n; i++)
            arr[i] = (int)rand() % 100000;

        for (int i = 1; i < nprocs; i++)
        {
            MPI_Recv(arrRicv, size, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            for (int j = 0; j < size; j++)
            {
                arr[((i - 1) * size) + j] = arrRicv[j];
            }
        }
        free(arrRicv);
    }
    else
    {
        int *arrRicv = (int *)calloc(size, sizeof(int *));
        MPI_Recv(arrRicv, size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        srand(rank);
        for (int i = 0; i < size; i++)
            arrRicv[i] = (int)rand() % 100000;

        MPI_Send(arrRicv, size, MPI_INT, 0, 2, MPI_COMM_WORLD);
        free(arrRicv);
    }
}

int main(int argc, char **argv)
{
    int world_rank, world_size;
    double countingSort_start_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Status status;

    if (argc < 2)
    {
        if (world_rank == 0)
            printf("ERROR! Usage: ./main dimarray ");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]); // write the dimension of array

    int *arr = (int *)calloc(n, sizeof(int *));

    double generateArray_start_time = MPI_Wtime();
    generateArrayParallel(arr, n, world_rank, world_size);
    double generateArray_end_time = MPI_Wtime();

    //==========================================Start count time for count sorting
    if (world_rank == 0)
        countingSort_start_time = MPI_Wtime();

    int max = getMax(arr, n);
    int size;            //size dimension of subarray divided by arr
    if (world_size == 1) //check how many processes the program is run
        size = 0;        //data is managed by a single process
    else
        size = n / (world_size);

    //rest of the elements divided equally between processes
    int leftElements = n - (size * (world_size));

    //Send in broadcast the size and the max to all the processes
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        int *output = (int *)calloc(n, sizeof(int *));
        int *count = (int *)calloc(max + 1, sizeof(int *));
        int *countRecv2 = (int *)calloc(max + 1, sizeof(int *));

        //Sends part of the array according to the specified size
        for (int i = 1; i < world_size; i++)
            MPI_Send(&arr[(i)*size], size, MPI_INT, i, 1, MPI_COMM_WORLD);

        for (int i = 0; i < size; i++) // prima era ++i
            count[arr[i]]++;

        //receives calculation results from processes and adds them into one.
        for (int i = 1; i < world_size; i++)
        {
            MPI_Recv(countRecv2, max + 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            for (int j = 0; j <= max; j++)
            {
                //merge arrays of leftElements with arrays received from processes.
                count[j] += countRecv2[j];
            }
        }

        //Sum the element with the previous element to match the
        //Ssrting position later.
        for (int i = 1; i <= max; i++)
            count[i] += count[i - 1];

        //Insertion the arr element based on the sorting position calculated in the output
        for (int i = 0; i < n; i++)
        {
            output[count[arr[i]] - 1] = arr[i];
            count[arr[i]]--;
        }
        double countingSort_end_time = MPI_Wtime();
        //==========================================End count time for count sorting

        double generateArray_time = generateArray_end_time - generateArray_start_time;
        double countingSort_time =
            countingSort_end_time - countingSort_start_time;

        double elapsed = generateArray_time + countingSort_time;
        printf("%d,%d,%f,%f,%f\n", n, world_size, generateArray_time,
               countingSort_time, elapsed);
        free(output);
        free(count);
        free(countRecv2);
    }
    else
    {
        int *sub_array = (int *)calloc(size, sizeof(int *));
        int *countRecv1 = (int *)calloc(max + 1, sizeof(int *));

        MPI_Recv(sub_array, size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        if (world_rank == 1)
            for (int i = 0; i < size + leftElements; i++)
                countRecv1[sub_array[i]]++;
        else
            for (int i = 0; i < size; i++)
                countRecv1[sub_array[i]]++;

        MPI_Send(countRecv1, max + 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        free(sub_array);
        free(countRecv1);
    }

    free(arr);
    MPI_Finalize();
    return 0;
}
