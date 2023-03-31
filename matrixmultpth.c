#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

void *tester(void*param)
{
	int *rc = (int*) param;  // each row 
	int x = rc[0];
	int i=0;
	int k=0;
	for(int i=1;i<=x;i++)
	{
		k+= rc[i]*rc[i+x];
		
	}
	int *rend = (int *)malloc(sizeof(int));
	*rend = k;
	pthread_exit(rend);

}
void main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("Error, wrong number \n");
	}
	else
	{
		clock_t start, end;
		double timing=0.0;
		start = clock();


		int MR= atoi(argv[1]); 
		printf("%d is the maximum number for rows of the matrices\n", MR);
		int matrix_m[MR][MR];
		int matrix_n[MR][MR];

        int row_n = MR;
		int row_m = MR;
		
        int col_n = MR;
		int col_m = MR;
		

		printf("\nMatrix M:\n");
		for(int i=0;i< MR; i++)
		{
			for(int j=0;j< MR; j++)
			{
				scanf("%d", &matrix_m[i][j]);
			}
		}
		printf("\nMatrix N:\n");	
		for(int i=0;i< MR; i++)
		{
			for(int j=0;j< MR; j++)
			{
				scanf("%d", &matrix_n[i][j]);
			}
		}

		int c=0;
		int thnum= row_m * col_n;
		pthread_t threads_total[thnum];
		int *rc;

		for(int i=0;i<row_m;i++)
		{
			for(int j=0;j<col_n;j++)
			{
				rc = (int*) malloc((MR*(MR+1))*sizeof(int));
				rc[0] = col_m;

				for(int l = 0; l<col_m; l++)
				{
					rc[l+1] = matrix_m[i][l];
				}
				for(int l = 0; l<row_n; l++)
				{
					rc[l+col_m+1] = matrix_n[l][j];
				}

				pthread_create(&threads_total[c++], NULL, tester, (void*) rc);

			}
		}

		printf("\nMatrix  C (The result of the matrix multiplication:)\n");
		for(int g=0;g<thnum;g++)
		{
			void *results;
			pthread_join(threads_total[g], &results);

			int *rend = (int *)results;
			printf("%d ", *rend);
			if((g+1)%col_n==0) 
			{
				printf("\n");
			}
		}


		end = clock();
		timing = ((double)(end-start))/CLOCKS_PER_SEC;

		printf("\nTime of execution: %.9f\n",timing);
	}
}
