#!/bin/bash

DENSITY=$1
SIZE=$2
VERSION=$3
ALGORITHM=$4

input="input/generated/dens0${DENSITY}/n${SIZE}/n_${SIZE}_v${VERSION}.col"
output="output/dens0${DENSITY}/n${SIZE}.out"

echo  "./build/benchmark/benchmark_exact_vertex_coloring ${ALGORITHM} ${input} >> ${output}"
./build/benchmark/benchmark_exact_vertex_coloring ${ALGORITHM} ${input} >> ${output}