#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/**
 * This program measures the latency and bandwidth of a pair of MPI processes on infiniband.
 */
int main(int argc, char *argv[]) {
    int partner_rank;
    int rank, size;
    double start_time, end_time, total_time;
    
    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Ensure there are exactly two processes
    if (size != 2) {
        if (rank == 0) {
            printf("This program is designed to run on exactly two processes.\n");
        }
        MPI_Finalize();
        exit(0);
    }
    // Other process rank
    partner_rank = (rank + 1) % 2; // 0 -> 1, 1 -> 0

    // Generate list of message lengths to test from 16MB to 3.2GB
    long step = 16000000;
    long lengths[200];

    for (int i = 0; i < 200; i++) {
        lengths[i] = step;
        step += 16000000;
    }
    int num_lengths = sizeof(lengths) / sizeof(lengths[0]);

    if (rank == 0)
        printf("Running ping-pong test with %d messages from 16MB to 3.2GB\n", num_lengths);
        
    for (int i = 0; i < num_lengths; i++) {
        long message_len = lengths[i];
        char *message = (char *)malloc(message_len * sizeof(char));

        // Ensure all processes start at the same time
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == 0) {
            // Send and then receive the message to calculate round trip time
            start_time = MPI_Wtime();
            MPI_Send(message, message_len, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);
            MPI_Recv(message, message_len, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            end_time = MPI_Wtime();

            total_time = (end_time - start_time) / 2.0; // Average one-way time

            printf("Message size: %ld bytes\t", message_len);
            printf("Total time: %.6f s\t\n", total_time);

        } else {
            // Just receive and send back
            MPI_Recv(message, message_len, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(message, message_len, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);
        }

        free(message);
    }

    // Finalize MPI environment
    MPI_Finalize();
    return 0;
}
