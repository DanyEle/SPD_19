#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc farm_skeleton.c util.c -o farm_skeleton
time mpirun --oversubscribe -np $AMOUNT_PROCESSES farm_skeleton < dataset.txt