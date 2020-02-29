##Brief introduction to Parallel Programming

First of all Parallel programming is no the same as sequenntial programming that is common to most programming languages. However there exist seeral tools and libraries out there to make it a bit easy when one practices well enough or unless you are a total complete genuis. POSIX, OpenMP, MPI etc. This very tutorial will cover the POSIX *(our beloved pthreads)*

I am writinng this tutorial as way to reteach myself these concepts again from memory( nothing personal) since I will need this in Phd qualification and oral exams so if yiu find some mistakes feel free to initiate a pull request. Also refer to this [free book](https://bitbucket.org/VictorEijkhout/hpc-book-and-course/src/default/) by [Professor Victor Eijkhout](https://pages.tacc.utexas.edu/~eijkhout/) of University of Texas. he has other free and very educative resource materials as well.

##Pthread Programming with Pthreads

Pthread archiieve paralleism based ont he Fork/Join machanism that basically means to spawn as many threads as prefere and then when the thread are done done their 'thing' join them and return some data or just simple terminated it. Our example below is an assignment from UTC given by Dr.Tannis *(check him out cool and supersmart guy he taught all this)*

### Problem 1

Disclaimer: if you don't like C, sorry I too was a java only guy but, this stuff is best done in C so just swallow your pride and learn C like I did sorry. *It's actually pretty amazing to be direct friends with a compiler. She will never break your heart*

### Write a naive multithreaded parallel prime number generator using pthreads. 

###Objective
* Launch NUM_THREADS individual threads, each engaged in a potentially time-consuming brute force algorithm that searches for a prime
* Finally join all the threads collecting the generated numbers and print it out beautifully
* Make sure your numbers are prime ( you can verify primality on <https://www.wolframalpha.com>

```c
#include <stdio.h>   
#include <stdlib.h>
#include <math.h>
#include <time.h>
```

Start by calling all important packages pthreads can be found in the **stdlib.h** header file
void random_array(double* array, int size, int scale);
double sum(double* array, int size);
double stdev(double* array, int size);
void smooth(double* array, int size, double w);


int main (){

time_t t;
    
int size=5;int iterations=10;int scale=5;double w=0.1;

printf("Enter the size of array:");
    scanf("%d", &size);

printf("Enter the scale of random array:");
    scanf("%d", &scale);

printf("Enter the smoothing weight:");
    scanf("%lf", &w);

printf("How namy iterations do you prefer?:");
    scanf("%d", &iterations);
double *array = (double *) malloc(size * sizeof(double));; 
srand((unsigned) time(&t));

random_array(array, size, scale);
int i=1;

while( i<=iterations){
	
	smooth(array,size,w);
	printf("At step %d standard deviation is %f\n",i, stdev(array,size));
	// printf("At step %d standard deviation is %f\n",i, array[size--]);
	i++;
}
free(array); // free memory

return 0;

}


void random_array(double* array, int size, int scale){
	for ( int i=0;i<size; i++){
		array[i]= ((double) rand() / RAND_MAX) * scale ;
	}

	
}

double sum(double* array, int size){

	double sum=0;

	for ( int i=0;i<size; i++){
		sum+=array[i];
	}
	return sum;

}

double stdev(double* array, int size){

	double average=sum(array,size)/size; /// calculate the mean
	double sqdDiff =0;

	for(int i=0;i<size;i++){	
		sqdDiff+=pow(array[i]-average,2);
	}

	return sqrt(sqdDiff/(size-1));

}

void smooth(double* array, int size, double w){
	for(int i=1;i<size-1;i++){	
		array[i]=(array[i]* w )+ (array[i-1] + array[i+1])* (1 -w)/2;
	}
}


`

