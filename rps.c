#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>


// Definitions
#define READY "READY"
#define GO "GO"
#define STOP "STOP"
#define RPS_CHOICES 3

const char *choices[] = {"Rock", "Paper", "Scissors"};


//Helper Functions

/**
 * Pick "Rock", "Paper", or "Scissors" randomly
 * 
 * @return const char*
 */
const char* get_choice() {
    return choices[rand() % RPS_CHOICES];
}

/**
 * Determine the winner of the game
 * 
 * @param player1_choice
 * @param player2_choice
 * @return int 0: Draw, 1: Player 1 wins, -1: Player 2 wins
 */
int determine_winner(const char* player1_choice, const char* player2_choice) {
    if (strcmp(player1_choice, player2_choice) == 0)
        return 0; // Draw
    if ((strcmp(player1_choice, "Rock") == 0 && strcmp(player2_choice, "Scissors") == 0) ||
        (strcmp(player1_choice, "Paper") == 0 && strcmp(player2_choice, "Rock") == 0) ||
        (strcmp(player1_choice, "Scissors") == 0 && strcmp(player2_choice, "Paper") == 0))
        return 1; // Player 1 wins
    else
        return -1; // Player 2 wins
}



int main(int argc, char **argv) {
    int rank, size, i;
    int iterations, score_player1 = 0, score_player2 = 0, result;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (size != 4) {
        if (rank == 0)
            fprintf(stderr, "ERROR: This program requires exactly four processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (argc != 2 || (iterations = atoi(argv[1])) <= 0) {
        if (rank == 0)
            fprintf(stderr, "Usage: mpirun -n 4 %s <number_of_iterations>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }


    srand(time(NULL) + rank);

    if (rank == 0) {
        //MAIN PROCESS

        printf("MAIN: Rock, Paper, Scissors: %d iterations\n", iterations);
        // Send number of iterations to referee
        MPI_Send(&iterations, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);



        // Wait for processes to finish
        char msg[100];

        //AWAIT referee and players to finish and send "STOP" message to main


        for (i = 0; i < 3; ++i) {
            MPI_Recv(msg, sizeof(msg), MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (strcmp(msg, STOP) == 0) {
                if (status.MPI_SOURCE == 3)
                    printf("MAIN: Referee Process Exited\n");
                else
                    printf("MAIN: Player %d Process Exited\n", status.MPI_SOURCE);
            }
        }

        // end main process


    } else if (rank == 1 || rank == 2) {
        // PLAYER PROCESSES

        char * msg = (char *)malloc(100 * sizeof(char));

        MPI_Recv(msg, 100, MPI_CHAR, 3, 0, MPI_COMM_WORLD, &status);  // Wait for READY from referee


        free(msg);

        MPI_Send(READY, strlen(READY) + 1, MPI_CHAR, 3, 0, MPI_COMM_WORLD);  // Tell referee READY


        while (1) {
            char buffer[10];

            MPI_Recv(buffer, sizeof(buffer), MPI_CHAR, 3, 0, MPI_COMM_WORLD, &status);

            if (strcmp(buffer, GO) == 0) {
                const char* choice = get_choice();
                MPI_Send(choice, strlen(choice) + 1, MPI_CHAR, 3, 0, MPI_COMM_WORLD);
            } else if (strcmp(buffer, STOP) == 0) {

                //Send STOP to main
                MPI_Send(STOP, strlen(STOP) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
                break;  // Termination signal received, stop player process
            }
        }
    } else if (rank == 3) {
        // This is the referee process
        char player1_choice[10], player2_choice[10];

        int iterations_ref;

        // GET number of iterations from main
        MPI_Recv(&iterations_ref, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        for (i = 0; i < 2; i++) {
            MPI_Send(READY, strlen(READY) + 1, MPI_CHAR, i + 1, 0, MPI_COMM_WORLD);
        }



        //AWAIT ready from both players
        char * msg = (char *)malloc(100 * sizeof(char));

        MPI_Recv(msg, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        printf("REF: Player %d: Ready\n", status.MPI_SOURCE);

        MPI_Recv(msg, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        free(msg);

        printf("REF: Player %d: Ready\n", status.MPI_SOURCE);



        for (i = 0; i < iterations; i++) {

            

            printf("REF: Go Players (%d)\n", i+1);
            
            MPI_Send(GO, strlen(GO) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Send(GO, strlen(GO) + 1, MPI_CHAR, 2, 0, MPI_COMM_WORLD);

            MPI_Recv(player1_choice, sizeof(player1_choice), MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(player2_choice, sizeof(player2_choice), MPI_CHAR, 2, 0, MPI_COMM_WORLD, &status);

            printf("REF: Player 1: %s\n", player1_choice);
            printf("REF: Player 2: %s\n", player2_choice);

            result = determine_winner(player1_choice, player2_choice);
            if (result == 1) {
                score_player1++;
                printf("REF: Player 1 wins\n");
            } else if (result == -1) {
                score_player2++;
                printf("REF: Player 2 wins\n");
            } else {
                printf("REF: Draw\n");
            }
        }
        
        printf("REF: Tournament Ends\n");
        if (score_player1 > score_player2)
            printf("REF: Player 1 Wins (%d/%d)\n", score_player1, iterations);
        else if (score_player1 < score_player2)
            printf("REF: Player 2 Wins (%d/%d)\n", score_player2, iterations);
        else
            printf("REF: The tournament is a Draw (%d/%d)\n", score_player1, iterations);

        // Signal both players to stop
        MPI_Send(STOP, strlen(STOP) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Send(STOP, strlen(STOP) + 1, MPI_CHAR, 2, 0, MPI_COMM_WORLD);

        // Signal main process that referee has finished
        MPI_Send(STOP, strlen(STOP) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return 0;
}
