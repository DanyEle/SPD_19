#!/bin/bash
AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes"
mpicc ping_pong.c -o ping_pong
mpirun -np $AMOUNT_PROCESSES  ping_pong
