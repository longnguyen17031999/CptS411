#!/bin/bash
#SBATCH -n 4                    # Number of MPI tasks (i.e. processes)
#SBATCH -N 2                    # Maximum number of nodes to be allocated
#SBATCH --output=411_%j.log  
#SBATCH --job-name=myawesomecodefrom411

mpirun $HOME/HW3/myprogram 