/*
    Import all neccesarry packages
*/


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

/* this @NUM_THREAD set the number of threads at compile time to 2 for testinng purposes*/
#define NUM_THREADS 10 

/* global variale the is ponitored for signalinng purposes and also aceessed by all threads for decision onn which action to perform*/
int next_action=0;

/* initialise mutex and condVar using the default attributes*/
pthread_mutex_t mutex =PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition=PTHREAD_COND_INITIALIZER;


// choose a time-consuming activity based on action ...
void perform_action(int action);



void* threadfunc(void* arg) // parameter name "args" was missen in the question so it was added so it to be set to arg
{
    int t_id=*(int*) arg;
    while (1)
    {

        pthread_testcancel(); // this will be called to ensure that we can cancel this thread if user enters -1
        pthread_mutex_lock(&mutex);
        while (next_action==0){
            printf("Thread --%d-- is busily waiting for nonzero next_action...\n",t_id);
            pthread_cond_wait(&condition, &mutex);
            printf("Thread --%d-- has received a nonzero next_action,proceeding to perform action ...\n",t_id);

        }
        
        int my_action=next_action;
        next_action=0;

        pthread_mutex_unlock(&mutex);

        printf("Thread --%d-- is now performing a long operation based on the signal\n",t_id);
        perform_action(my_action);
        
    }


}
void perform_action(int action){

    switch(action){
        case 1: 
            sleep(1000000);
            printf("slept for 100000s\n");
            
            break;
        case 2:
            sleep(15000000);
            printf("sleepng for 4ms\n");
            
            break;
        default:
            printf("defaulting to silly printing text for values not >2\n");
    }
}


int main()
{
    // assume we've spawned threads executing threadfunc ...

    pthread_t tid[NUM_THREADS];
    int args[NUM_THREADS];

        // create threads
        printf("Creating %d threads...\n",NUM_THREADS);

        for(int i=0;i<NUM_THREADS;i++){
            args[i]=i;
            pthread_create(&tid[i], NULL, threadfunc,&args[i]);

        }

        printf("Thread creation completed\n");



    while (1)
    {
         /*

         1. Lock a mutex since are going to update the global nnext_action Variable
         2. check the value entered and determinn a break a a signal sennding process
         3. unlock the mutex annd wait a short while ofr thread to also use and modify the next_actionn back to zer
         */

         
         pthread_mutex_lock(&mutex);// this called to ensure no race condition exists


         printf( "Enter a value: ");
        // determine what action should be dispatched to a thread next
        // next_action=
        
        scanf("%d", &next_action);

        // exit the program
        if (next_action == -1) 
            { 
                pthread_mutex_unlock(&mutex);
                break ;
            }
        if(next_action>0){

            pthread_cond_signal(&condition);
        }
            

        pthread_mutex_unlock(&mutex);

        /* This ensures that the spawned thread can also grab the mutex in time*/
        sleep(1);


    }

    // clean up by joining with threads ... 

    for(int i=0;i<NUM_THREADS;i++){

        pthread_cancel(tid[i]);
        pthread_join(tid[i],NULL);  

        }

    return 0;
}


