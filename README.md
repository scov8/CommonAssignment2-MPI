# CommonAssignmentMPI1

## Dependencies

* CMake 3.9+
* MPICH
* Python3
* Pipenv

## How to run

1. Create a build directory and launch cmake

   ```batch
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   ```

2. Generate executables with `make`
3. To generate measures (TAKE A LOT OF TIME! Our measures are already included so you should skip this step) run `make generate_measures`
4. To extract mean times and speedup curves from them run `make extract_measures`

Results of measures can be found in the measures/measure directory divided by problem size, the gcc optimization option used and case study (V1 for case study 1 and V2 for case study 2).
