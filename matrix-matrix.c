#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <sys/time.h>
#define N 5

MPI_Status status;

double matrix_a[N][N],matrix_b[N][N],matrix_c[N][N];

int main(int argc, char **argv)
{
  int processCount, processId, slaveTaskCount, source, dest, rows, offset;

  struct timeval start, stop;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &processId);
  MPI_Comm_size(MPI_COMM_WORLD, &processCount);
  slaveTaskCount = processCount - 1;
  
 if (processId == 0) {
	
// Matrix A and Matrix B both will be filled with random numbers

    
    matrix_a[0][0] = 9;
    matrix_a[0][1] = 7;
    matrix_a[0][2] = -5;
    matrix_a[0][3] = 5;
    matrix_a[0][4] = -5;
    matrix_a[1][0] = 0; 
    matrix_a[1][1] = 4; 
    matrix_a[1][2] = 6;
    matrix_a[1][3] = -7;
    matrix_a[1][4] = 4;
    matrix_a[2][0] = 1;
    matrix_a[2][1] = -6;
    matrix_a[2][2] = 8;
    matrix_a[2][3] = -7;
    matrix_a[2][4] = 7;
    matrix_a[3][0] = 5;
    matrix_a[3][1] = -2;
    matrix_a[3][2] = 1;
    matrix_a[3][3] = -9;
    matrix_a[3][4] = 5;
    matrix_a[4][0] = 5;
    matrix_a[4][1] = -2;
    matrix_a[4][2] = 1;
    matrix_a[4][3] = -9;
    matrix_a[4][4] = 5;
   
    matrix_b[0][0] = 5;
    matrix_b[0][1] = 7;
    matrix_b[0][2] = -1;
    matrix_b[0][3] = 2;
    matrix_b[0][4] = 9;
    matrix_b[1][0] = -6;
    matrix_b[1][1] = 7;
    matrix_b[1][2] = 2;
    matrix_b[1][3] = 4;
    matrix_b[1][4] = 8;
    matrix_b[2][0] = 1;
    matrix_b[2][1] = 2;
    matrix_b[2][2] = -5;
    matrix_b[2][3] = -8;
    matrix_b[2][4] = 2;
    matrix_b[3][0] = -4;
    matrix_b[3][1] = 0;
    matrix_b[3][2] = 2;
    matrix_b[3][3] = 4;
    matrix_b[3][4] = 6;
    matrix_b[4][0] = -4;
    matrix_b[4][1] = 0;
    matrix_b[4][2] = 0;
    matrix_b[4][3] = 4;
    matrix_b[4][4] = -6;
	
  printf("\n\t\tMatrix - Matrix Multiplication using MPI\n");

// Print Matrix A
    printf("\nMatrix A\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        printf("%.0f\t", matrix_a[i][j]);
      }
	    printf("\n");
    }

// Print Matrix B
    printf("\nMatrix B\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        printf("%.0f\t", matrix_b[i][j]);
      }
	    printf("\n");
    }

    rows = N/slaveTaskCount;
    offset = 0;

    for (dest=1; dest <= slaveTaskCount; dest++)
    {
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&matrix_a[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
      MPI_Send(&matrix_b, N*N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
      
      offset = offset + rows;
    }

    for (int i = 1; i <= slaveTaskCount; i++)
    {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_c[offset][0], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    }

// Print the result matrix
    printf("\nResult Matrix C = Matrix A * Matrix B:\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++)
        printf("%.0f\t", matrix_c[i][j]);
      printf ("\n");
    }
    printf ("\n");
  }

  if (processId > 0) {
    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status); 
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_a, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_b, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

    // Matrix multiplication

    for (int k = 0; k<N; k++) {
      for (int i = 0; i<rows; i++) {
        matrix_c[i][k] = 0.0;
        for (int j = 0; j<N; j++)
          matrix_c[i][k] = matrix_c[i][k] + matrix_a[i][j] * matrix_b[j][k];
      }
    }
    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&matrix_c, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}
