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

cmake_minimum_required(VERSION 3.9)
project(CommonAssignmentMPI01
	DESCRIPTION "Counting Sort MPI"
	VERSION 0.1
	LANGUAGES C
	)

# Generate comile_commands.json for semantic completion in YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# DEBUG flag
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

enable_testing()

add_executable(program_O1-V1 src/mainV1.c src/utils.c include/utils.h)
add_executable(program_O2-V1 src/mainV1.c src/utils.c include/utils.h)
add_executable(program_O3-V1 src/mainV1.c src/utils.c include/utils.h)
add_executable(program_O1-V2 src/mainV2.c src/utils.c include/utils.h)
add_executable(program_O2-V2 src/mainV2.c src/utils.c include/utils.h)
add_executable(program_O3-V2 src/mainV2.c src/utils.c include/utils.h)
add_executable(program_seq_O0 src/main_seq.c src/utils.c include/utils.h)
add_executable(program_seq_O1 src/main_seq.c src/utils.c include/utils.h)
add_executable(program_seq_O2 src/main_seq.c src/utils.c include/utils.h)
add_executable(program_seq_O3 src/main_seq.c src/utils.c include/utils.h)
add_library(lib src/utils.c include/utils.h)

target_include_directories(program_O1-V1 PRIVATE include)
target_include_directories(program_O2-V1 PRIVATE include)
target_include_directories(program_O3-V1 PRIVATE include)
target_include_directories(program_O1-V2 PRIVATE include)
target_include_directories(program_O2-V2 PRIVATE include)
target_include_directories(program_O3-V2 PRIVATE include)
target_include_directories(program_seq_O0 PRIVATE include)
target_include_directories(program_seq_O1 PRIVATE include)
target_include_directories(program_seq_O2 PRIVATE include)
target_include_directories(program_seq_O3 PRIVATE include)
target_include_directories(lib PRIVATE include)


find_package(MPI REQUIRED)
if(MPI_C_FOUND)
	message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
	target_link_libraries(program_O1-V1 PUBLIC MPI::MPI_C)
	target_link_libraries(program_O2-V1 PUBLIC MPI::MPI_C)
	target_link_libraries(program_O3-V1 PUBLIC MPI::MPI_C)
	target_link_libraries(program_O1-V2 PUBLIC MPI::MPI_C)
	target_link_libraries(program_O2-V2 PUBLIC MPI::MPI_C)
	target_link_libraries(program_O3-V2 PUBLIC MPI::MPI_C)
	target_link_libraries(lib PUBLIC MPI::MPI_C)
endif()

target_compile_options(program_O1-V1 PRIVATE -O1)
target_compile_options(program_O2-V1 PRIVATE -O2)
target_compile_options(program_O3-V1 PRIVATE -O3)
target_compile_options(program_O1-V2 PRIVATE -O1)
target_compile_options(program_O2-V2 PRIVATE -O2)
target_compile_options(program_O3-V2 PRIVATE -O3)

target_compile_features(program_O1-V1 PRIVATE c_std_11)
target_compile_features(program_O2-V1 PRIVATE c_std_11)
target_compile_features(program_O3-V1 PRIVATE c_std_11)
target_compile_features(program_O1-V2 PRIVATE c_std_11)
target_compile_features(program_O2-V2 PRIVATE c_std_11)
target_compile_features(program_O3-V2 PRIVATE c_std_11)
target_compile_features(program_seq_O0 PRIVATE c_std_11)
target_compile_features(program_seq_O1 PRIVATE c_std_11)
target_compile_features(program_seq_O2 PRIVATE c_std_11)
target_compile_features(program_seq_O3 PRIVATE c_std_11)
target_compile_features(lib PRIVATE c_std_11)

# ------------------------------------ DOCUMENTATION ------------------------------------
find_package(Doxygen
             REQUIRED dot)

if(DOXYGEN_FOUND)
	set(DOXYGEN_GENERATE_XML YES)
	doxygen_add_docs(
		docs
		./src/utils.c ./include/utils.h
		)
endif()

# ---------------------------------- GENERATE MEASURES ----------------------------------
add_custom_target(
	 generate_measures
	 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating measures [${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash ${CMAKE_CURRENT_BINARY_DIR}]"
    DEPENDS
		program_O1-V1
		program_O2-V1
		program_O3-V1
		program_O1-V2
		program_O2-V2
		program_O3-V2
		program_seq_O0
		program_seq_O1
		program_seq_O2
		program_seq_O3
)

# --------------------------------- MEASURES EXTRACTION ---------------------------------
find_package(Python3 COMPONENTS Interpreter REQUIRED)
find_program(PipEnv pipenv REQUIRED)

add_custom_target(
	 install_pipenv
	 COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} install --ignore-pipfile
    COMMENT "Installing pipenv dependencies [${PipEnv} install]"
    DEPENDS	${CMAKE_CURRENT_SOURCE_DIR}/Pipfile.lock
)

add_custom_target(
    extract_measures
    COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/extract.py
    COMMENT "Generating version header [${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/extract.py]"
    DEPENDS
		install_pipenv
		${CMAKE_CURRENT_SOURCE_DIR}/measures/extract.py
)
