#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>


#include <omp.h>

void main()

{

#pragma omp parallel 
{
	printf(“ world(%d) \n”, ID);
}

}