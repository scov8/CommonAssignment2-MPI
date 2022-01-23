#
# Course: High Performance Computing 2021/2022
#
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Rosa Gerardo	    0622701829	g.rosa10@studenti.unisa.it
# Scovotto Luigi    0622701702  l.scovotto1@studenti.unisa.it
# Tortora Francesco 0622701700  f.tortora21@studenti.unisa.it
#
# Copyright (C) 2022 - All Rights Reserved
# This file is part of CommonAssignment2.
#
# Requirements: Parallelize and Evaluate Performances of "COUNTING SORT" Algorithm ,by using MPI.
#
# The previous year's group 02 files proposed by the professor during the course were used for file generation and extraction.
#
# The starting point for the counting sort function was from this video:
# https://www.youtube.com/watch?v=qcOoEjdYSz0
#
# CommonAssignment2 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CommonAssignment2 is distributed in the hope that it will be useful,
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
NMEASURES=1

ARRAY_RC=(50000000 100000000 150000000 200000000)
ARRAY_THS=(0 1 2 4 8 16)
ARRAY_OPT=(0 1 2 3)
VERSION=(1 2)

trap "exit" INT

# Get script path
SCRIPTPATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

for size in "${ARRAY_RC[@]}"; do
	for ths in "${ARRAY_THS[@]}"; do
		for opt in "${ARRAY_OPT[@]}"; do
			for ver in "${VERSION[@]}"; do
				ths_str=$(printf "%02d" $ths)

				OUT_FILE=$SCRIPTPATH/measure/SIZE-$size-O$opt-V$ver/SIZE-$size-NP-$ths_str-O$opt-V$ver.csv

				if [[ $opt -eq 0 && $ths -ne 0 ]]; then
					continue;
				fi

				#to jump the measures for the MPI with 0 process (to jump the serial run)
				if [[ $ths -eq 0 ]]; then
					continue;
				fi

				mkdir -p $(dirname $OUT_FILE) 2> /dev/null

				echo $(basename $OUT_FILE)

				echo "DimArray,processes,generateArray_time,countingSort_time,elapsed" > "$OUT_FILE"

				for ((i = 0 ; i < $NMEASURES ; i++)); do
					if [[ $ths -eq 0 ]]; then
						$1/program_seq_O$opt $size >> "$OUT_FILE"
						printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
						printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
						continue;
					else
						mpirun -np $ths $1/program_O$opt-V$ver  $size >> "$OUT_FILE"
						printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
						printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
					fi
				done
				printf "\n"
			done
		done
	done
done
