#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc async.c -o async
mpirun -np $AMOUNT_PROCESSES async
