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

#ifndef H_UTILS
#define H_UTILS
#include <time.h>
#include <sys/time.h>

#define STARTTIME(id)                             \
    clock_t start_time_42_##id, end_time_42_##id; \
    start_time_42_##id = clock()

#define ENDTIME(id, x)          \
    end_time_42_##id = clock(); \
    x = ((double)(end_time_42_##id - start_time_42_##id)) / CLOCKS_PER_SEC

int getMax(int *arr, int n);

void generateArray(int *arr, int dim);

void generateArrayParallel(int arr[], int n, int rank, int nprocs);

#endif
