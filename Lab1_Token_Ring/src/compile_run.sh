#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc token_ring.c -o token_ring
mpirun -np $AMOUNT_PROCESSES  token_ring
