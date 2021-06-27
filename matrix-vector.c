#include <stdio.h>
#include <mpi.h>
#define NCOLS 5
int main(int argc, char **argv) {
int i,j,k,l;
int ierr, rank, size, root;
float A[NCOLS];
float Apart[NCOLS];
float Bpart[NCOLS];
float C[NCOLS];
float A_exact[NCOLS];
float B[NCOLS][NCOLS];
float Cpart[1];
root = 0;
/* Initiate MPI. */
ierr=MPI_Init(&argc, &argv);
ierr=MPI_Comm_rank(MPI_COMM_WORLD, &rank);
ierr=MPI_Comm_size(MPI_COMM_WORLD, &size);
/* Initialize B and C. */
if (rank == root) {
B[0][0] = 1;
B[0][1] = 2;
B[0][2] = 6;
B[0][3] = 5;
B[0][4] = 2;
B[1][0] = 3;
B[1][1] = -5;
B[1][2] = 6;
B[1][3] = 4;
B[1][4] = 3;
B[2][0] = -9;
B[2][1] = 8;
B[2][2] = 9;
B[2][3] = -1;
B[2][4] = 0;
B[3][0] = 3;
B[3][1] = -1;
B[3][2] = 5;
B[3][3] = 0;
B[3][4] = 4;
C[0] = 1;
C[1] = -4;
C[2] = 1;
C[3] = 0;
C[4] = 8;
}

/* Put up a barrier until I/O is complete */
ierr=MPI_Barrier(MPI_COMM_WORLD);
/* Scatter matrix B by rows. */
ierr=MPI_Scatter(B,NCOLS,MPI_FLOAT,Bpart,NCOLS,
MPI_FLOAT,root, MPI_COMM_WORLD);
/* Scatter matrix C by columns */
ierr=MPI_Scatter(C, 1, MPI_FLOAT, Cpart, 1, MPI_FLOAT,
root,MPI_COMM_WORLD);
/* Do the vector-scalar multiplication. */
for(j=0;j<NCOLS;j++)
Apart[j] = Cpart[0]*Bpart[j];
/* Reduce to matrix A. */
ierr=MPI_Reduce(Apart,A,NCOLS,MPI_FLOAT,
MPI_SUM, root, MPI_COMM_WORLD);

if (rank == 0) {
printf("\nThis is the result of the parallel computation:\n\n");
printf("A[0]=%g\n",A[0]);
printf("A[1]=%g\n",A[1]);
printf("A[2]=%g\n",A[2]);
printf("A[3]=%g\n",A[3]);
printf("A[4]=%g\n",A[4]);
for(k=0;k<NCOLS;k++) {
A_exact[k] = 0.0;
for(l=0;l<NCOLS;l++) {
A_exact[k] += C[l]*B[l][k];
}
}
printf("\nThis is the result of the serial computation:\n\n");
printf("A_exact[0]=%g\n",A_exact[0]);
printf("A_exact[1]=%g\n",A_exact[1]);
printf("A_exact[2]=%g\n",A_exact[2]);
printf("A_exact[3]=%g\n",A_exact[3]);
printf("A_exact[4]=%g\n",A_exact[4]);
}
MPI_Finalize();
}
