#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
		
MPI_Init(&argc, &argv);
		
int gsize;
int root = 0, myrank, * rbuf;

MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
int sendarray[100] = { myrank * 10 ,myrank * 10 ,myrank * 10 ,myrank * 10 ,myrank * 10 };
if (myrank == root) {
	MPI_Comm_size(MPI_COMM_WORLD, &gsize);
	rbuf = (int*)malloc(gsize*100*sizeof(int));
	MPI_Gather(&sendarray, 100, MPI_INT, rbuf, 100, MPI_INT, root, MPI_COMM_WORLD);
	printf("Final buffer size : %d\n", gsize * 100);
	printf("Final Buffer Array Value :  ");
	for (int i = 0;i < gsize*100;i++) {
		printf( " %d ", rbuf[i]);
		}
		printf("\n");
	}
else
	{
	MPI_Gather(&sendarray, 5, MPI_INT, NULL, 0, MPI_INT, root, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return(0);
	}
