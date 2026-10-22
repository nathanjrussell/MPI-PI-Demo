# MPI Pi Demo

- Estimate pi with midpoint rectangles under `4 / (1 + x*x)` on `[0, 1]`
- Argument `N` sets the rectangle count and must be a positive integer
- Each rank computes every Pth rectangle, where P is the communicator size
- Current version is `v3-barrier`

## Versions

- Each Git branch contains one driver and its own Slurm file
- `v1-send-recv` sends N and receives partial sums
- `v2-bcast` distributes N with `MPI_Bcast`
- `v3-barrier` adds `MPI_Barrier` and times computation plus collection
- `v4-reduce` collects partial sums with `MPI_Reduce`
- `v5-split` uses `MPI_Comm_split` for even and odd world ranks
- Each split group independently computes all N rectangles with its own reduction
- `main` matches `v5-split`
- Barrier waits for all ranks to enter it, broadcast distributes data

## Files

- `include/` declarations and `src/` shared calculation and input
- `example/main.cpp` driver and `gtest/` accuracy, partition, and input tests

## Modules and build

- Modules select software and set executable and library paths
- `gnu9/9.3.0` provides the C++ compiler
- `openmpi4/4.0.5` provides MPI and `mpirun`
- `cmake/3.19.4` provides CMake and CTest
- These module names apply to MCC
- Configuration downloads GoogleTest from GitHub
- Switch branches manually and rebuild before submitting

```bash
git switch v3-barrier
module load gnu9/9.3.0 openmpi4/4.0.5 cmake/3.19.4
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 2
cd build
ctest --output-on-failure
cd ..
```

## Slurm

- Submit from the repository root after building
- `--account=PROJECT` selects a valid project allocation
- `#!/bin/bash -l` enables modules through the login environment
- `--job-name` labels the job and `--partition` selects the queue
- `--nodes=4` requests four hosts
- `--ntasks-per-node=2` requests eight MPI processes across those hosts
- `--cpus-per-task=1` assigns one CPU per process
- `--time=00:01:00` limits runtime to one minute
- `--output` and `--error` separate stdout and stderr, `%j` is the job ID
- `set -e` stops on failure and `module load` selects compiler and MPI
- `N=${1:-1000000}` uses the first argument or defaults to one million
- `mpirun ./build/mpi_pi "$N"` reads the process count and placement from Slurm

```bash
sbatch --account=PROJECT mpi_test.slurm 1000000
squeue --me
scontrol show job JOBID
sacct -j JOBID --format=JobID,State,Elapsed,NNodes,NTasks,ExitCode
cat mpi-pi-JOBID.out
cat mpi-pi-JOBID.err
scancel JOBID
```

- `sbatch` submits and `squeue` lists jobs
- `scontrol` shows allocations and `sacct` shows history
- `cat` reads output and `scancel` cancels jobs
- Replace `PROJECT` with an allocation and `JOBID` with the submitted job number
