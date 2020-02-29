
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define SIZE 1000000

int main()
{
	double *values= (double*) malloc(SIZE*sizeof(double));
	double sum=0;
	int A,B,C;
	for(int i=0;i<SIZE;i++){
		values[i]= 100;
		// printf("%d\n",values[i] );
	}
	/* this pragma decalares parallelism with implicit barrier and 
	   enable the array to be shared by each thread*/
	#pragma omp parallel shared(values)

	{
		/* parallel for loop for reduction of array*/
		#pragma omp for reduction(+:sum) 
		for (int i=0; i<SIZE; i++)
			{    
				sum += values[i];
			}
		/* parallel single derivative because usinng multiple 
	       threads will mean each thread wont have info from previous*/
		#pragma omp single 
		for (int i=1; i<SIZE; i++)
			{    
				values[i] = (values[i] + values[i-1]);
			}
		
		/* parallel for loop for with a barrier
			since this block is dependent of reduction sum value and nnew values array*/
		#pragma omp for schedule(guided)
		for (int i=0; i<SIZE; i++)
			{    
				values[i] /= sum;
				printf("%f ", values[i]);
			}
	}

}