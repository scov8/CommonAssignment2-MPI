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
void generateArrayParallel(int arr[], int n, int rank, int nprocs)
{
    int size;
    if (nprocs == 1)
        size = 0;
    else
        size = n / (nprocs - 1);                  // dim dei sott array da gestire si processi
    int leftElements = n - (size * (nprocs - 1)); // elementi rimanenti

    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Status status;

    if (rank == 0)
    {
        int *arrRicv = (int *)calloc(size, sizeof(int *));
        srand(rank);

        for (int i = 1; i < nprocs; i++)
            MPI_Send(&arr[(i - 1) * size], size, MPI_INT, i, 1, MPI_COMM_WORLD);

        for (int i = n - leftElements; i < n; i++) // era i<=n
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
        for (int i = 0; i < size; i++) // anche qua era ++i
            arrRicv[i] = (int)rand() % 100000;

        MPI_Send(arrRicv, size, MPI_INT, 0, 2, MPI_COMM_WORLD);
        free(arrRicv);
    }

    // if(rank==0){
    //     for (int i = 0; i < n; i++)
    //         printf("%d ", arr[i]);
    // }
}

int main(int argc, char **argv)
{
    int world_rank, world_size;
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

    // int n = sizeof(arr) / sizeof(arr[0]); //numero di elementi dell'array
    int max = getMax(arr, n);
    int size;
    if (world_size == 1)
        size = 0;
    else
        size = n / (world_size); // size dimensione del sottoarray diviso per arr

    int leftElements = n - (size * (world_size)); // elementi rimanenti
    // int leftElements= dimarray - (size *(world_size-1));

    // Mando in broadcast la size ed il max a tutti i processi
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0)
    {
        /*
        printf("array before sorting:\n");
        printArray_(arr, n);
        */

        int *output = (int *)calloc(n, sizeof(int *));
        int *count = (int *)calloc(max + 1, sizeof(int *));
        int *countRecv2 = (int *)calloc(max + 1, sizeof(int *));

        double countingSort_start_time = MPI_Wtime(); //===============================Start count time for
                                                      // count sorting
        // invia parte dell'array in base alla dimensione specificata
        for (int i = 1; i < world_size; i++)
            MPI_Send(&arr[(i)*size], size, MPI_INT, i, 1, MPI_COMM_WORLD);

        for (int i = 0; i < size; i++) // prima era ++i
            count[arr[i]]++;

        // riceve i risultati del calcolo dai processi e li aggiunge in un solo.
        for (int i = 1; i < world_size; i++)
        {
            MPI_Recv(countRecv2, max + 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            for (int j = 0; j <= max; j++)
            {
                // merge degli array di leftElements con array ricevuti dai
                // processi.
                count[j] += countRecv2[j];
            }
        }

        // PARTE SERIALE, NO PARALLELIZZABILE!!!
        // Somma l'elemento con l'elemento precedente per far corrispondere la
        //  posizione di ordinamento in seguito.
        for (int i = 1; i <= max; i++)
            count[i] += count[i - 1];

        // Inserimento dell'elemento arr in base alla posizione di smistamento
        // calcolata nell'output
        for (int i = 0; i < n; i++)
        {
            output[count[arr[i]] - 1] = arr[i];
            count[arr[i]]--;
        }
        double countingSort_end_time = MPI_Wtime(); //==========================================End count
                                                    // time for count sorting

        /*
        printf("array after sorting:\n");
        printArray_(output, n);
        */

        double generateArray_time = generateArray_end_time - generateArray_start_time;
        double countingSort_time =
            countingSort_end_time - countingSort_start_time;

        double elapsed = generateArray_time + countingSort_time;
        printf("%d,%d,%f,%f,%f\n", n, world_size, generateArray_time,
               countingSort_time, elapsed);
        /*test(output, n);
        printf(",");
        printArray_(output, n);*/
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
            for (int i = 0; i < size + leftElements; i++) // same ++i
                countRecv1[sub_array[i]]++;
        else
            for (int i = 0; i < size; i++) // same ++i
                countRecv1[sub_array[i]]++;

        MPI_Send(countRecv1, max + 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        free(sub_array);
        free(countRecv1);
    }

    free(arr);
    MPI_Finalize();
    return 0;
}
