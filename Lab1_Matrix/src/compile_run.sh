#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc array.c -o array
mpirun -np $AMOUNT_PROCESSES array
