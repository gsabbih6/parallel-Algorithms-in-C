	#include <stdio.h>	#include <pthread.h>	#include <stdlib.h>	#include <unistd.h>	#include <stdbool.h>	#include <time.h>	#define NUM_VALUES  10	#define NUM_THREADS 3	// #define MAX_TRIES 100	#define MAXVAL 10000000//0000	/* declared global variables*/	int prime_count=0;	unsigned long int *prime_array;	/* initialise mutex and condVar using the default attributes*/	pthread_mutex_t prime_count_mutex =PTHREAD_MUTEX_INITIALIZER;	pthread_cond_t prime_count_cv=PTHREAD_COND_INITIALIZER;	void* thread_func(void *arg);	// void* thread_wait(void *arg);	unsigned long int get_random_num();	bool isPrime(unsigned long int test_num);	void print_array(unsigned long int *test_num,int size);	int main(){		clock_t start=clock(); // start time		prime_array= (unsigned long int*) malloc(NUM_VALUES* sizeof(unsigned long int));		pthread_t tid[NUM_THREADS];		// create threads		printf("Creating %d threads...\n",NUM_THREADS);		for(int i=0;i<NUM_THREADS;i++){			pthread_create(&tid[i], NULL, thread_func,NULL);		}		printf("Thread creation completed\n");		pthread_mutex_lock(&prime_count_mutex);	    while (prime_count<NUM_VALUES) 	   {	     pthread_cond_wait(&prime_count_cv, &prime_count_mutex);	     printf("wait signal received, condition was met\n");	   }	   pthread_mutex_unlock(&prime_count_mutex);	   				for(int i=0;i<NUM_THREADS;i++)		{			pthread_cancel(tid[i]);			pthread_join(tid[i],NULL);			}	   print_array(prime_array,NUM_VALUES);	   free(prime_array);	   	   	   clock_t finish=clock();	   printf("Program used a total of %d thread(s) and completed in %fms. \n",NUM_THREADS,(double)(finish-start)*1000/CLOCKS_PER_SEC);	   pthread_exit(NULL);	   return 0;	}	void *thread_func(void* arg){		unsigned long int test_num=0;		// pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);		while(1)		{			// sleep(1);			pthread_testcancel();			test_num=get_random_num();			if(isPrime(test_num))			{				/* request a lock on the mutex to uptate prime_count and global array*/				pthread_mutex_lock(&prime_count_mutex);				prime_array[prime_count]=test_num;				prime_count++;								if(prime_count==NUM_VALUES)				{					pthread_cond_signal(&prime_count_cv);					pthread_mutex_unlock(&prime_count_mutex);					printf("signal sent to the master thread\n");				}								pthread_mutex_unlock(&prime_count_mutex);							}			// sleep(1);					}		return NULL;	}	bool isPrime(unsigned long int test_num){		for(int i=2; i<test_num;i++){				if(test_num % i== 0){ // is not prime					return false;				}			}			return true;		}	/* this subroutine returns a random unsigned long int */		unsigned long int get_random_num(){			unsigned long int to_test ;			arc4random_buf (& to_test , sizeof ( to_test ));			to_test = to_test % MAXVAL ;			return to_test;		}	void print_array(unsigned long int* array, int size){		for(int i=0;i<size;i++)		{			printf("%lu\n",array[i]);		}	}