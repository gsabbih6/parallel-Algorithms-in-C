#include <stdio.h>   
#include <stdlib.h>
#include <math.h>
#include <time.h>

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




