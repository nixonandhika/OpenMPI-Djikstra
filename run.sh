#!/bin/bash

echo Input NP:
read np

echo Input N:
read n

mpirun -np $np --hostfile mpi_hostfile out $n
