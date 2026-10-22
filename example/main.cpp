#include "pi.hpp"
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long n = 0;
    if (rank == 0) {
        n = pi_demo::read_n(argc, argv);
        for (int peer = 1; peer < size; ++peer) {
            MPI_Send(&n, 1, MPI_LONG_LONG_INT, peer, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&n, 1, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if (n == 0) {
        MPI_Finalize();
        return 1;
    }

    const double partial = pi_demo::partial_pi(n, rank, size);
    if (rank == 0) {
        double total = partial;
        for (int peer = 1; peer < size; ++peer) {
            double received;
            MPI_Recv(&received, 1, MPI_DOUBLE, peer, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += received;
        }
        pi_demo::print_result("v1_send_recv", n, size, total);
    } else {
        MPI_Send(&partial, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
