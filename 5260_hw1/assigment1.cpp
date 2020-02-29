#include <iostream>
#include <stdio.h>   
#include <stdlib.h>
#include <math.h>
#include <ctime>

void random_array(double* array, int size, int scale){
time_t current_time;
        // Stores time in current_time
	time(&current_time);
	for ( int i=0;i<size; i++){
		array[i]= ((double) srand(current_time) / RAND_MAX) ;
	}

	
}

double sum(double* array, int size){

	double sum(0.00);

	for ( int i=0;i<size; i++){
		sum+=array[i];
	}
	return sum;

}

double stdev(double* array, int size){

	double average=sum(array,size)/size; /// calculate the mean
	double sqdDiff(0);

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


int main (){

int size=5;
int scale=5;
double w=0.1;
double A[size];
double *array= new double[size]; // can also use malloc for C

random_array(array, size, scale);
std::cout << "At "<<"step "<<0 <<"\n"<< "standard deviation is "<<stdev(array,size)<<std::endl;
// printf(array, size );
int i(1);
while( i<=10){
	
	smooth(array,size,w);
	std::cout << "At "<<"step "<<i<<"\n"<< "standard deviation is "<<stdev(array,size)<<std::endl;

	// w+=0.20;
	i++;
}
delete[] array; // free memory


}
