#!/bin/bash
#SBATCH -n 1
#SBATCH --time=00:10:00
#SBATCH --partition=normal

mpirun -np 1 ./abordagemExaustivaMPI entrada
