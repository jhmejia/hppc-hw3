// "Hello World" program for MPI in C

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv); // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the number of processes

    printf("Hello World from process %d of %d\n", rank, size);

    MPI_Finalize(); // Finalize MPI
    return 0;
}