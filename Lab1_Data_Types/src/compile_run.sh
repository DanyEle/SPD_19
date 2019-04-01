#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc data_types.c -o data_types
mpirun -np $AMOUNT_PROCESSES  data_types
