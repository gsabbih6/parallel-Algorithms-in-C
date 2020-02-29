## Brief introduction to Parallel Programming

First of all Parallel programming is not the same as sequenntial programming common to most programming languages. However there exist several tools and libraries out there to make it a bit easy when one practices well enough or unless you are a total complete genuis. POSIX, OpenMP, MPI etc. This very tutorial will cover the POSIX *(our beloved pthreads)*

I am writinng this tutorial as way to reteach myself these concepts again ( nothing personal) since I will need this in Phd qualification and oral exams so if you find some mistakes feel free to initiate a pull request. Also refer to this [free book](https://bitbucket.org/VictorEijkhout/hpc-book-and-course/src/default/) by [Professor Victor Eijkhout](https://pages.tacc.utexas.edu/~eijkhout/) of University of Texas. he has other free and very educative resource materials as well.

## Pthread Programming with Pthreads

Pthread archieve parallelism based on the Fork/Join machanism that basically means to spawn as many threads as prefered and then when the thread are done their 'thing' join them and return some data or just simple terminated it. Our example below is an assignment from [UTC](www.utc.edu) given by [Dr.Tannis](https://www.utc.edu/college-engineering-computer-science/profiles/computer-science-engineering/faculty/wyk255.php) *(check him out cool and supersmart guy he taught all this)*

### Problem 1

Disclaimer: if you don't like C, sorry but I was also a java-only guy but, this stuff is best done in C so just learn C like I did sorry. *It's actually pretty amazing to be direct friends with a compiler. She will never break your heart*

### Write a naive multithreaded parallel prime number generator using pthreads. 

### Objective
* Launch NUM_THREADS individual threads, each engaged in a potentially time-consuming brute force algorithm that searches for a prime
* Finally join all the threads collecting the generated numbers and print it out beautifully
* Make sure your numbers are prime ( you can verify primality on <https://www.wolframalpha.com>

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
```

Start by calling all important packages pthreads can be found in the **stdlib.h** header file. You can also add header files as needed as you proceed through your code. This website can help a lot <https://www.cprogramming.com/>

```
#define NUM_THREADS 100
#define MAX_TRIES 10
#define MAXVAL 99999999999	
```

'#define' is the **C** way of saying this variable is a constant, it will never be changed so please, store in a safe location in memory. 
In the above we will **spawn 100 threads**, each thread will **try 10 times** to find a prime if and giveup if it fails or stops searching if it finds one. Finally the  MAXVAL is used to compute the random number (random generator code given by Dr Tannis)


```c
int main(){

	pthread_t tid[NUM_THREADS]; // array of 100 threads declared and to be spawned by the main thread

	// create threads, in other words FORKING THREADS
	printf("Creating %d threads...\n",NUM_THREADS);

	for(int i=0;i<NUM_THREADS;i++){

		pthread_create(&tid[i], NULL, thread_func,NULL);

	}
	printf("Thread creation completed\n");
	
	// waiting stage the join

	printf("Threads are now searching for primes by brute force...\n");

	printf("Here’s the output of my program with %d threads and MAX_TRIES set to %d :\n\n",NUM_THREADS,MAX_TRIES);
	
	// join threads, in other words JOINING THREADS
	
	for(int i=0;i<NUM_THREADS;i++){
		void* ref;
		pthread_join(tid[i],&ref);
		unsigned long int dref= (unsigned long int)ref;
		
		if(dref>0){
			printf("thread %d reports that %lu is prime\n",i,dref);
		}else{
			printf("thread %d reports no prime\n",i);
		}
	}

	return 0;

}
```

Two important syntax for thread creation are the pthread_create(...) and p_thread_join(...). Take a look at the man pages on your terminal or visit this [link](http://man7.org/linux/man-pages/man7/pthreads.7.html) for the documentation. 

```c
/*
void pointer function that will be run by every thread.
It basicall gets a random lont int and test if it is
a prime if not repeats the process 10 times and then gives up and return 0;
*/
void *thread_func(void* arg){
	unsigned long int test_num=0;
	for(int i=0;i<MAX_TRIES;i++){
		test_num=get_random_num();
		if(isPrime(test_num)){
			break;
		}
		test_num=0;

	}

	return (void*)test_num;
}
```
This function or method is the piece of code that all the threads are going to run since we assigned it to the pthread_create() as the third argument. We can create as many of these functions ans required to perform all sort of functions by different threads but it must return a void pointer as seen above

```c
bool isPrime(unsigned long int test_num){

	for(int i=2; i<test_num;i++){

			if(test_num % i== 0){ // is not prime
				return false;
			}
		}

		return true;

	}
```
 The prime testing function basically uses and inefficient brute force algorithm to test for primality. Other efficienct alogorthms exist, youc anread more [here](https://en.wikipedia.org/wiki/Primality_test). However the idea in this problem is to literally make threads SUFFER!!

```c
/* this subroutine returns a random unsigned long int */

unsigned long int get_random_num(){

	unsigned long int to_test ;
	arc4random_buf (& to_test , sizeof ( to_test ));
	to_test = to_test % MAXVAL ;

	return to_test;
	}
	
```

Finally the random number geratator for getting a random number.

After this compile with ***gcc -Wall -O3 pthread.c*** in terminal, where pthread.c is the file name and the run with ***./a.out***







