
#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#define N 32 /* Number of Rows in Matrix.*/
#define P 32 /* Number of Columns in Matrix.*/  

/*
    References :- 
        1. https://www.youtube.com/watch?v=BA_Mqi3a9HI
        2. https://mpitutorial.com/tutorials/mpi-send-and-receive/
        3. https://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/node23.html
        4. https://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/node149.html#Node149
        5. https://stackoverflow.com/questions/5298739/mpi-global-execution-time
        6. https://mpitutorial.com/tutorials/mpi-broadcast-and-collective-communication/
        7. https://mpitutorial.com/tutorials/mpi-scatter-gather-and-allgather/
*/

int main(int argc, char** argv)
{
    double sendTime[100], recieveTime[100]; //For noting the time period taken by each process to execute the task.
    MPI_Status status; 
    int a[N][P], b[P][N], c[N][N]; //Initializing the arrays
    //Initializing some elements which might need further in the program. 
    int i, j, k, rank, commsize, workers, source, destination, rowsToSend, offset;

    //Starting the MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Rank of the Processes.
    MPI_Comm_size(MPI_COMM_WORLD, &commsize); //Total Number of Processes, given by user.

    //Deducting one from total process as one process will be the "Root Process".
    workers = commsize - 1; //Number of slave processes.

    //Checking whether the number of processes given by user does not exceed than rows in the matrix.
    if ((N % workers) != 0)
    {
        if (rank == 0)
        {
            printf("Error: Invalid Number of Process.");
        }
        MPI_Finalize();
    }else {
        double startTime, endTime; //Time to note the exact execution time of the program.
        startTime = MPI_Wtime(); //Taking the timestamp.
        
        /*
            Here, if the rank of process is root i.e 0, then it will get into this loop.
            Array a and b will be intialized..
        */

        //Initialization of two Arrays.
        if (rank == 0) {
            for (i = 0; i < N; i++) {
                for (j = 0; j < P; j++) {
                    a[i][j] = 1;
                }
            }

            for (i = 0; i < P; i++) {
                for (j = 0; j < N; j++) {
                    b[i][j] = 2;
                }
            }
            
            //Dividing to give rows to each process.
            rowsToSend = N / workers;
            offset = 0; //Number of the row elements taken together.
            for (destination = 1; destination <= workers; destination++)
            {
                sendTime[destination] = MPI_Wtime(); //Noting the timestamp when the task is send to process.
                MPI_Send(&offset, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);
                MPI_Send(&rowsToSend, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);
                MPI_Send(&a[offset][0], rowsToSend * N, MPI_INT, destination, 1, MPI_COMM_WORLD);
                MPI_Send(&b, N * N, MPI_INT, destination, 1, MPI_COMM_WORLD);
                MPI_Send(&sendTime, 1, MPI_INT, destination, destination, MPI_COMM_WORLD);
                offset = offset + rowsToSend;//Adding Offset.
            }

            //Recieved messages from the processes will be received here...
            for (i = 1; i <= workers; i++)
            {
                source = i;
                MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
                MPI_Recv(&rowsToSend, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
                MPI_Recv(&c[offset][0], rowsToSend * N, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
                recieveTime[i] = MPI_Wtime(); //Noting the timestamp when the task is received from the process.
            }

            //Printing all the difference between the timestamps of processes.
            for (i = 1; i <= workers; i++) {
                printf("Difference in Time : %f\n",  recieveTime[i] - sendTime[i]);
            }

            //Printing Arrays.
            printf("\nMatrix A\n\n");
            for (i = 0; i < N; i++) {
                for (j = 0; j < P; j++) {
                    printf("%d\t", a[i][j]);
                }
                printf("\n");
            }

            printf("\nMatrix B\n\n");
            for (i = 0; i < P; i++) {
                for (j = 0; j < N; j++) {
                    printf("%d\t", b[i][j]);
                }
                printf("\n");
            }

            endTime = MPI_Wtime(); //Noting the timestamp when the execution of the root process is over.
            printf("\nHere is the result matrix:\n\n");
            for (i = 0; i < N; i++) {
                for (j = 0; j < N; j++)
                    printf("%d   ", c[i][j]);
                printf("\n");
            }
            //Printing the time elapsed for the execution of the program.
            printf("\nTime elapsed for complete Process : %f\n", endTime - startTime);
        }
        //This is for the processes which are not root.
        if (rank > 0) {
            //Recieving all the tasks from the root process.
            source = 0;
            MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&rowsToSend, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&a, rowsToSend * N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&b, N * N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);


            //Main logic of Matrix Multiplication.
            for (k = 0; k < N; k++) {
                for (i = 0; i < rowsToSend; i++) {
                    c[i][k] = 0;
                    for (j = 0; j < P; j++) {
                        c[i][k] = c[i][k] + a[i][j] * b[j][k];
                    }
                }
            }

            //Sending back the results of the task assigned.
            MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
            MPI_Send(&rowsToSend, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
            MPI_Send(&c, rowsToSend * N, MPI_INT, 0, 2, MPI_COMM_WORLD);
        }
        MPI_Finalize();
    }
}
