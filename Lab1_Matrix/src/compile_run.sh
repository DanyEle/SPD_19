#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc matrix.c -o matrix
mpirun -np $AMOUNT_PROCESSES matrix
