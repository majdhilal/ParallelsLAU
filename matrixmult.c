#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <omp.h>

int matrixsize=0;

void main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("Wrong number\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		omp_set_num_threads(10);
		clock_t start, end;
		double timing=0.0;

		
		int ms = atoi(argv[1]);

		
		matrixsize = ms;
		

		int matrix_m[ms][ms]; 
		int matrix_n[ms][ms]; 
		int matrix_c[ms][ms]; 

		printf("Filling matrices M and N:\n");
		printf("\nMatrix M:\n");
		for(int i=0;i<ms;i++)
		{
			for(int j=0;j<ms;j++)
			{
				scanf("%d", &matrix_m[i][j]);
			}
		}
		printf("\nMatrix N:\n");
		for(int i=0;i<ms;i++)
		{
			for(int j=0;j<ms;j++)
			{
				scanf("%d", &matrix_n[i][j]);
			}
		}

		start = clock();
		int i,j,k;
		#pragma omp parallel for private(i,j,k) shared(matrix_m, matrix_n, matrix_c)
		
			for(i=0;i<matrixsize; i++)
			{
				for(j=0;j<matrixsize;j++)
				{
					matrix_c[i][j] =0.0;
					for(k=0;k<matrixsize;k++)
					{
						matrix_c[i][j] += matrix_m[i][k]*matrix_n[k][j];
					}
				}
			}
		
		
		


		printf("\n(Result of the matrix multiplication):\n");

		for(int i=0;i<ms;i++)
		{
			for(int j=0;j<ms;j++)
			{
				printf("%d ", matrix_c[i][j]);

				if((j+1)%ms==0)
				{
					printf("\n");
				}
			}
		}
		end= clock();
		timing = ((double)(end-start))/CLOCKS_PER_SEC;
		printf("Time of execution: %.8f\n", timing);

	}
}