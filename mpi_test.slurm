#!/bin/bash -l
#SBATCH --job-name=mpi-pi
#SBATCH --partition=normal
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=2
#SBATCH --cpus-per-task=1
#SBATCH --time=00:01:00
#SBATCH --output=mpi-pi-%j.out
#SBATCH --error=mpi-pi-%j.err

set -e
module load gnu9/9.3.0 openmpi4/4.0.5
N=${1:-1000000}

echo "nodes=$SLURM_JOB_NODELIST ranks=$SLURM_NTASKS N=$N"
mpirun ./build/mpi_pi "$N"
