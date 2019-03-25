#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc matrix_mult.c -o matrix_mult
mpirun -np $AMOUNT_PROCESSES matrix_mult
