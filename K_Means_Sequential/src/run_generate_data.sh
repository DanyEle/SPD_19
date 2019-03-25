#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
gcc generatedata.c -o generatedata
#mpirun -np $AMOUNT_PROCESSES  kmeans
