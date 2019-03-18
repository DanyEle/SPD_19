#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc farm_skeleton.c -o farm_skeleton
mpirun -np $AMOUNT_PROCESSES  farm_skeleton