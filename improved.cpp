/*
	Programmer(s): Andrew Huy (Skelletion code in 'do not modify' sections 
		       Rocio Salguero 
	Created: 04/01/17 11:30PM
	Last Modified: 04/02/17 10:01AM
	
	compile: g++  main.cpp -lpthread -o as2
    	run input: time cat test.txt | ./as2
	check name: ./as2 -n

	Description: Uses threads to make searching for a prime easier. 
	Using task parallelism giving each thread to do the is_prime() function dividing the work. 
	Uses only 4 threads;  
	
	Results were:
		Original main.cpp
			Status:4
			real	1m41.841s
			user	1m41.792s
			sys	0m0.044s


		Threaded main.cpp
			Status:4
			real	0m59.299s
			user	2m46.332s
			sys	0m0.484s


	References for PThreads used: 
	POSIX Threads Programming 
		URL(https://computing.llnl.gov/tutorials/pthreads/#Overview)
	PThread Summation Example 
		URL(https://computing.llnl.gov/tutorials/pthreads/samples/arrayloops.c)
	Multithreaded Programming 
		URL(http://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/multi-thread.html#thread_create_stop)
	How to Terminate a Thread in C Program ( pthread_exit Example )
		URL(http://www.thegeekstuff.com/2012/04/terminate-c-thread/)
	Compiling multithread code with g++
		URL(http://stackoverflow.com/questions/19463602/compiling-multithread-code-with-g)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <pthread.h>

#define STUDENT_NAME "Rocio Salguero"
#define STUDENT_CWID "891379752"
#define PRIME 4
#define NOPRIME 2

#define NTHREADS 4

/*====you may not change code in this section====*/
void print_name(void) {
	printf("\n%s\n", STUDENT_NAME);
	printf("%s\n", STUDENT_CWID);
}

int is_prime(long long num)
{
     if (num <= 1) return 0;
     if (num % 2 == 0 && num > 2) return 0;
     for(int i = 3; i < num / 2; i+= 2)
     {
         if (num % i == 0)
             return 0;
     }
     return 1;
}
/*============end of section==============*/ 

bool status; 
void *search_for_p(void *n);
pthread_mutex_t prime_mutex;

int main(int argc, char ** argv)
{
	/*====you may not change code in this section====*/
	if (argc == 2 && strncmp(argv[1], "-n", 2) == 0) {
		print_name();
		return 0;
	}
	/*==============end of section==================*/ 
   
	
	//threading process
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;
	
	 //Pthreads setup: initialize mutex and explicitly create threads in a
	 //joinable state (for portability).  Pass each thread its loop offset 
	 pthread_attr_init(&attr);
	 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	
	//Read  in the characters into a int array 
	long long x[NTHREADS]; 
	int i;	
	while(true) {	
		if(std::cin.eof()) break;
		for(i=0; i <NTHREADS; i++) {
			std::cin >> x[i]; 
			if(std::cin.eof()) x[i] = 0;			
			pthread_create(&threads[i], &attr, search_for_p, (void*) &x[i]);	
 		}

		for(i=0; i <NTHREADS; i++) {
			pthread_join(threads[i], NULL); 
			if(status) {
				printf("Status:%d",PRIME); 
				return PRIME;
			}
 		}		
		
		
	}

	printf("Status:%d",NOPRIME); 
	return NOPRIME; 
	
}

void *search_for_p(void *n) {
	
	long long *num =(long long *) n;
	//printf("value thread:%llu\n",*num);	
	if(is_prime(*num)) {	 
		pthread_mutex_lock (&prime_mutex);		
		status = true; 
		pthread_mutex_unlock (&prime_mutex);	

		//printf("int:%llu status:1\n", *num);
	
		pthread_exit(NULL);  
	}


	//printf("int:%llu status:0\n", *num);
	pthread_exit(NULL);  
}

