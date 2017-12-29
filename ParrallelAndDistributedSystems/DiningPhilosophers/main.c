//
//  main.c
//  131Lab1
//
//  Created by Aditya Iyer on 2/1/17.
//  Copyright © 2017 Aditya Iyer. All rights reserved.
//
// This program is Multi-threaded Dining Philosopher's solution

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define COURSES 3;

struct philosopher {
    //int index;
    int course;
 //   int threadNumber;
    //    pthread_t thread;
};

//struct to pass in multiple arguments to thread_work function
struct arg_struct {
    pthread_mutex_t fork1;
    pthread_mutex_t fork2;
    struct philosopher philosopher;
    //int numberOfThreads;
};


void errorCheck(int error) {
    if (error != 0) {
        fprintf(stderr, "Value of error number: %d\n", error);
    }
}

// This implements the Philosopher functionality
void* thread_work(void* arguments) {
    struct arg_struct args= *(struct arg_struct*) arguments;
    
    while(args.philosopher.course != 0) {
        // trylock() returns immediately if lock unsuccessfull rather than waiting
        if (pthread_mutex_trylock(&args.fork1) == 0) {
            if (pthread_mutex_trylock(&args.fork2) == 0) {
                sleep(1);
                fprintf(stdout, "Philosopher Eaten\n");
                args.philosopher.course--;
                    
                pthread_mutex_unlock(&args.fork1);
                pthread_mutex_unlock(&args.fork2);
            }
        } else {
            pthread_mutex_unlock(&args.fork1);
        }
    }
    
    if (args.philosopher.course == 0) {
        printf("DONE\n");
        pthread_exit(NULL);
    }
    
    return NULL;
}

int main(int argc, const char * argv[]) {
    int numberOfThreads;
    
    if (argc <= 1) {
        fprintf(stdout, "Please give the number of threads desired\n");
        return 0;
    }

    numberOfThreads = atoi(argv[1]);
    fprintf(stdout, "%d\n", numberOfThreads);
    pthread_mutex_t forks[numberOfThreads];
    pthread_t threads[numberOfThreads];
    struct philosopher struct_array[numberOfThreads];
    struct arg_struct argument;
    
    //initialize mutex array
    for (int i = 0; i < numberOfThreads; i++) {
        pthread_mutex_t fork;
        forks[i] = fork;
        int errorNumber = pthread_mutex_init(&forks[i], NULL);
        errorCheck(errorNumber);
    }
    
    //initialize philosopher array
    for (int i = 0; i < numberOfThreads; i++) {
        struct philosopher p;
        //p.index = i;
        p.course = COURSES;
      //p.threadNumber = numberOfThreads;
        struct_array[i] = p;
    }
    
    
    //create threads and perform concurrent algorithm
    for (int i = 0; i < numberOfThreads; i++) {
        int errorNumber;
        argument.philosopher = struct_array[i];
        argument.fork1 = forks[i];
        argument.fork2 = forks[((i+1)%numberOfThreads)];
        errorNumber = pthread_create(&threads[i], 0, thread_work, (void*) &argument);
        errorCheck(errorNumber);
        
    }
    
    // Join threads at the end of the program
    for (int i = 0; i < numberOfThreads; i++) {
        int errorNumber = pthread_join(threads[i], NULL);
        errorCheck(errorNumber);
    }
    
    // Destroy the mutexes
    for (int i = 0; i < numberOfThreads; i++) {
        int errorNumber = pthread_mutex_destroy(&forks[i]);
        errorCheck(errorNumber);
    }
    
    return 0;
}
