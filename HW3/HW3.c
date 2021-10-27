/*
    Description: This is Game of Life code
    Class: CptS_411 WSU Fall 2021
    Name: Long Nguyen
    Method: Using MPI to partition work load

    *****IMPORTANT*****
    Assumption: Given M x N matrix and n number of processes
                M % n must be 0 for this code to be accurate

    Contain of slurm_submit.sh:
        #!/bin/bash
        #SBATCH -n 4                    # Number of MPI tasks (i.e. processes)
        #SBATCH -N 2                    # Maximum number of nodes to be allocated
        #SBATCH --output=411_%j.log  
        #SBATCH --job-name=myawesomecodefrom411

        mpirun $HOME/HW3/myprogram

    How to run this code:
        mpicc HW3.c -o myprogram
        sbatch slurm_submit.sh
        cat 411_******.log (****** = id of the job returned)

    Strategy used:
        Partition the main matrix by rows
        Each process handles 1 partition
        Use buffer that has (M / n) + 2 rows to store a partition
        Send top and bottom data among processes
     *******************
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include<time.h>

#define M 128
#define N 10
#define TIME_STEPS 1
#define NUM_PROCESSES 4

//This function will return 0 if a cell has a value that are neither 0 nor 1
int value(int** buffer, int row, int column){
    if (column < 0 || column > N - 1)
        return 0;
    else{ 
        if(buffer[row][column] == 1)
            return 1;
        else
            return 0;
    }
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    //options for command line argument
    //must be in M-N-TIME_STEPS order 
/*    if(argc < 4)
        printf("Missing arguments!\n");
    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int TIME_STEPS = atoi(argv[3]);*/

    //world_rank = id
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    //world_size = processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);


    //allocating 2d arrays - main matrix
    int *Matrix[M];
    for(int i = 0; i < M; i++){
        Matrix[i] = (int*) malloc(sizeof(int) * N);
    }

    //set pseudo-random seed
    srand((unsigned)time(NULL));

    //initialize the maxtrix randomly
    for(int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            Matrix[i][j] = rand() % 2;
        }
    } 


    //allocating 2d array - buffer
    int *buffer[M];
    for(int i = 0; i < ((M / world_size) + 2); i++){
        buffer[i] = (int*) malloc(sizeof(int) * N);
    }

    int work_load = M / world_size;

    //Main logic
    for(int outer = 0; outer < TIME_STEPS; ++outer){
        //Initialize the buffer for each process
        for(int index = 0; index < work_load; index++){
            for (int j = 0; j < N; ++j){
                buffer[index + 1][j] = Matrix[world_rank*work_load + index][j];
            }
        }

        //Initialize the very top and bottom row of the buffer to 0
        for (int j = 0; j < N; ++j){
            buffer[0][j] = 0;
            buffer[(M / world_size) + 1][j] = 0;
        }

        //Sending and receiving data
        if (world_rank == 0){
            MPI_Send(buffer[(M / world_size)], N, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer[((M / world_size) + 1)], N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        else if (world_rank == 1){
            MPI_Recv(buffer[0], N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer[1], N, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(buffer[(M / world_size)], N, MPI_INT, 2, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer[((M / world_size) + 1)], N, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        else if (world_rank == 2){
            MPI_Send(buffer[(M / world_size)], N, MPI_INT, 3, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer[((M / world_size) + 1)], N, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(buffer[0], N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer[1], N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }

        else if (world_rank == 3){
            MPI_Recv(buffer[0], N, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer[1], N, MPI_INT, 2, 0, MPI_COMM_WORLD);
        }

        for(int i = 1; i <= (M / world_size); ++i){
            for(int j = 0; j < N; ++j){
                int neigh_count = value(buffer,i-1,j-1) + value(buffer,i-1,j) + value(buffer,i-1,j+1) +
                                  value(buffer,i+1,j-1) + value(buffer,i+1,j) + value(buffer,i+1,j+1) +
                                  value(buffer,i,j-1) + value(buffer,i,j+1);
                if(buffer[i][j]==1)
                    Matrix[world_rank*work_load + (i - 1)][j] = neigh_count>=2 ? 1:0;
                else  
                    Matrix[world_rank*work_load + (i - 1)][j] = neigh_count>=4 ? 1:0;
            }
        }
    }

    //free allocated 2d arrays
    for(int i = 0; i < M; i++){
        free(Matrix[i]);
    }
    //free allocated 2d arrays
    for(int i = 0; i < ((M / world_size) + 2); i++){
        free(buffer[i]);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}