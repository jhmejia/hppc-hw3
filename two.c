#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <ctype.h> //For upper case conversion

#define MAX_STRING_LENGTH 9001 //it is over 9000

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Check if the number of processes is  3
    if (size != 3) {
        if (rank == 0) {
            fprintf(stderr, "This program requires exactly 3 processes.\n");
        }
        MPI_Finalize();
        exit(1);
    }

    char input_str[MAX_STRING_LENGTH];
    char receive_str[MAX_STRING_LENGTH];

    if (rank == 0) {
        // read str 
        printf("Please enter a string: ");
        fflush(stdout);
        scanf("%9000s", input_str);
        // Send the string to Process 1.
        MPI_Send(input_str, strlen(input_str) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        // Receive the processed string from Process 2.
        MPI_Recv(receive_str, MAX_STRING_LENGTH, MPI_CHAR, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // Output the processed string.
        printf("Processed string is: %s\n", receive_str);
    } 
    else if (rank == 1) 
    {
        // Process 1 receives the string and reverses it.
        MPI_Recv(receive_str, MAX_STRING_LENGTH, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int len = strlen(receive_str);

       

        char reversed_str[MAX_STRING_LENGTH];
        for (int i = 0; i < len; ++i) {
            reversed_str[i] = receive_str[len - i - 1];
        }
        


        // Send the reversed string to Process 2.
        MPI_Send(reversed_str, len + 1, MPI_CHAR, 2, 0, MPI_COMM_WORLD);
    }
    else 
    {
        // AWAIT string from p1, change to uppercase, SEND to p0
        MPI_Recv(receive_str, MAX_STRING_LENGTH, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; receive_str[i] != '\0'; i++) {
            receive_str[i] = toupper(receive_str[i]);
        }
        // Send str to p0
        MPI_Send(receive_str, strlen(receive_str) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

