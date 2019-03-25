#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc farm_dynamic.c -o farm_dynamic
time mpirun --hostfile hostfile -np $AMOUNT_PROCESSES  farm_dynamic