#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc generatedata.c -o kmeans
mpirun -np $AMOUNT_PROCESSES  kmeans
