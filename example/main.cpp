#include "pi.hpp"
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    long long n = 0;
    if (world_rank == 0) {
        n = pi_demo::read_n(argc, argv);
    }
    MPI_Bcast(&n, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
    if (n == 0) {
        MPI_Finalize();
        return 1;
    }

    // Even and odd world ranks form independent communicators.
    const int color = world_rank % 2;
    MPI_Comm group;
    MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &group);
    int rank, size;
    MPI_Comm_rank(group, &rank);
    MPI_Comm_size(group, &size);

    MPI_Barrier(group);
    const double start = MPI_Wtime();
    const double partial = pi_demo::partial_pi(n, rank, size);
    double total = 0.0;
    MPI_Reduce(&partial, &total, 1, MPI_DOUBLE, MPI_SUM, 0, group);
    if (rank == 0) {
        pi_demo::print_result("v5_split", n, size, total, MPI_Wtime() - start, color);
    }

    MPI_Comm_free(&group);
    MPI_Finalize();
    return 0;
}
