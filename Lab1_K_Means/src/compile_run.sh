AMOUNT_PROCESSES=$1
echo "Running program with $AMOUNT_PROCESSES processes[K=1,...,$AMOUNT_PROCESSES]."
mpicc kmeans.c util.c -o kmeans
mpirun -np $AMOUNT_PROCESSES kmeans -stdin, --stdin 2 < dataset.txt
#!/bin/bash
#mpicc farm_skeleton.c -o farm_skeleton
#time mpirun --hostfile hostfile -np $AMOUNT_PROCESSES  farm_skeleton
