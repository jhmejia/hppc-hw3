#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int partner_rank;

    int rank, size;

    double start_time, end_time, total_time;
    double latency, bandwidth;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 2) {
        if (rank == 0) {
            printf("This program is designed to run on exactly two processes.\n");
        }
        MPI_Finalize();
        exit(0);
    }

    // Other process rank
    partner_rank = (rank + 1) % 2; // 0 -> 1, 1 -> 0

    // MSG sizes
    int lengths[] = { 16000000, 32000000, 48000000, 64000000, 80000000, 96000000, 112000000, 128000000, 144000000, 160000000, 176000000, 192000000,
     208000000, 224000000, 240000000, 256000000, 272000000, 288000000, 304000000, 320000000, 336000000, 352000000, 368000000, 384000000, 400000000, 
     416000000, 432000000, 448000000, 464000000, 480000000, 496000000, 512000000, 528000000, 544000000, 560000000, 576000000, 592000000, 608000000,
      624000000, 640000000, 656000000, 672000000, 688000000, 704000000, 720000000, 736000000, 752000000, 768000000, 784000000, 800000000, 816000000,
       832000000, 848000000, 864000000, 880000000, 896000000, 912000000, 928000000, 944000000, 960000000, 976000000, 992000000, 1008000000, 1024000000,
        1040000000, 1056000000, 1072000000, 1088000000, 1104000000, 1120000000, 1136000000, 1152000000, 1168000000, 1184000000, 1200000000, 1216000000,
         1232000000, 1248000000, 1264000000, 1280000000, 1296000000, 1312000000, 1328000000, 1344000000, 1360000000, 1376000000, 1392000000, 1408000000, 
         1424000000, 1440000000, 1456000000, 1472000000, 1488000000, 1504000000, 1520000000, 1536000000, 1552000000, 1568000000, 1584000000, 1600000000,
          1616000000, 1632000000, 1648000000, 1664000000, 1680000000, 1696000000, 1712000000 };
    int num_lengths = sizeof(lengths) / sizeof(lengths[0]);

    if (rank == 0)
        printf("Running ping-pong test with %d byte messages\n", lengths[num_lengths - 1]);

    for (int i = 0; i < num_lengths; i++) {
        int message_len = lengths[i];
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




            
            printf("Message size: %d bytes\t", message_len);
            printf("Total time: %.6f s\t\n", total_time);
            // printf("Latency (λ): %.6f s\t", latency);
            // printf("Bandwidth (β): %.6f bytes/s\n", bandwidth);
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
