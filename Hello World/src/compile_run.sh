#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc hello_world.c -o hello_world
mpirun -np $AMOUNT_PROCESSES  hello_world
