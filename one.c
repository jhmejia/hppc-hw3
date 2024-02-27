// "Hello World" program for MPI in C

#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv); // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get the rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Get the number of processes

    if (size < 2) {
        printf("This program requires at least 2 processes\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
        printf("===Part 1 ===\n");

    // Build the message 
    char message[100];
    char hostname[100];
    gethostname(hostname, 100);
    sprintf(message, "Greetings from process %d of %d on %s", rank, size, hostname);

    char recv_message[100];

    // Barrier to synchronize all processes
    MPI_Barrier(MPI_COMM_WORLD);

    // Create a for loop in which process 0 recieves the message from all other processes in order and prints them. Note only process 0 will print the messages
    if (rank == 0) {
        printf("%s\n", message);

        for (int i = 1; i < size; i++) {
            MPI_Recv(recv_message, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", recv_message);
        }
    } else {
        MPI_Send(message, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0)
        printf("\n===Part 2 ===\n");
    // Barrier
    MPI_Barrier(MPI_COMM_WORLD);

    
    // Create a for loop in which process 0 recieves the message all other process in any order and prints them. Note only process 0 will print the messages
    if (rank == 0) {
        MPI_Send(message, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

        for (int i = 0; i < size; i++) {
            MPI_Recv(recv_message, 100, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", recv_message);
        }
    } else {
        MPI_Send(message, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize(); // Finalize MPI
    return 0;
}