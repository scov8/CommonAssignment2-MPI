#
# Course: High Performance Computing 2021/2022
#
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Rosa Gerardo	    0622701829	 g.rosa10@studenti.unisa.it
# Scovotto Luigi    0622701702  l.scovotto1@studenti.unisa.it
# Tortora Francesco 0622701700  f.tortora21@studenti.unisa.it
#
# Copyright (C) 2021 - All Rights Reserved
# This file is part of CommonAssignment1.
#
# Requirements: Parallelize and Evaluate Performances of "COUNTING SORT" Algorithm ,by using MPI.
#
# The previous year's group 02 files proposed by the professor during the course were used for file generation and extraction.
#
# The counting sort function for test case n. 1 was taken here:
# https://github.com/ianliu/programacao-paralela/blob/master/omp-count-sort/main.c
#
# CommonAssignment1 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CommonAssignment1 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with CommonAssignment2.  If not, see <http://www.gnu.org/licenses/>.
#
# You can find the complete project on GitHub:
# https://github.com/scov8/CommonAssignment2-Team02

TIME_STAMP=$(date +%s)
NMEASURES=7

ARRAY_RC=(100000000 200000000 500000000)
ARRAY_THS=(0 1 2 4 8 16)
ARRAY_OPT=(0 1 2 3)

trap "exit" INT

# Get script path
SCRIPTPATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

for size in "${ARRAY_RC[@]}"; do
	for ths in "${ARRAY_THS[@]}"; do
		for opt in "${ARRAY_OPT[@]}"; do
			ths_str=$(printf "%02d" $ths)

			OUT_FILE=$SCRIPTPATH/measure/SIZE-$size-O$opt/SIZE-$size-NP-$ths_str-O$opt.csv

			if [[ $opt -eq 0 && $ths -ne 0 ]]; then
				continue;
			fi

			#to jump the measures for the MPI with 0 process
			if [[ $ths -eq 0 ]]; then
				continue;
			fi

			mkdir -p $(dirname $OUT_FILE) 2> /dev/null

			echo $(basename $OUT_FILE)
			#if [[ $opt -gt 1 && $ths -eq 0 ]]; then
			#	OLD_OUT_FILE=$SCRIPTPATH/measure/SIZE-$size-O$opt/SIZE-$size-NP-$ths_str-O$opt.csv
			#	ln -srf -T $OLD_OUT_FILE $OUT_FILE
			#	echo Created symbolic link to $(basename $OLD_OUT_FILE)
			#	continue
			#fi

			echo "DimArray,processes,generateArray_time,countingSort_time,elapsed" > "$OUT_FILE"

			for ((i = 0 ; i < $NMEASURES ; i++)); do
				if [[ $ths -eq 0 ]]; then
					$1/program_seq_O$opt $size >> "$OUT_FILE"
					printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
					printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
					continue;
				else
					mpirun -np $ths $1/program_O$opt $size >> "$OUT_FILE"
					printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
					printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
				fi
			done
			printf "\n"
		done
	done
done
