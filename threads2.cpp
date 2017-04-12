/*
	Programmer(s): Andrew Huy (Skelletion code in 'do not modify' sections 
				Rocio Salguero 
	Created: 03/12/17
	Last Modified: 03/23/17
	
	compile: g++  main.cpp -lpthread -o as2
    	run input: time cat test.txt | ./as2
	check name: ./as2 -n

	Description: Uses threads to make searching for a prime easier. 
	Using data parallelism to make the sequencial search faster to find a prime. 
	Parallel threads terminates when a prime is found. 

	Issues: Largest value able to read is long long, largest amount of numbers is 
	30000. Number of threads used 20. 
	

	Since this stradegy is to read from a stdio and insert into array till last one is inputed best case is O(n). 
	Comparing it to the original making it fair by timing it after inserting all values to array.
	Results were:
		Original main.cpp
			return value: 4 time:106300.78125ms
			real	1m46.309s
			user	1m46.300s
			sys	0m0.000s
		Current main.cpp
			(Done)Prime status:4  Time: 104267.99219ms 
			real	1m44.368s
			user	1m44.240s
			sys	0m0.028s

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
#include <math.h>  
#include <time.h>

#define STUDENT_NAME "Rocio Salguero"
#define STUDENT_CWID "891379752"
#define NEGATIVE 4
#define POSITIVE 2

#define MAX 30000
#define NTHREADS 20

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

//global variables 
long long sera[MAX]; int n; 
int iterations; 
bool isprime = false; 
pthread_mutex_t prime_mutex;

void *search_for_p(void *tid);

int main(int argc, char ** argv)
{
	/*====you may not change code in this section====*/
	if (argc == 2 && strncmp(argv[1], "-n", 2) == 0) {
		print_name();
		return 0;
	}
	/*==============end of section==================*/ 
   
	
	
	//Read  in the characters into a int array 
	long long x;
	int index =0; 
	while(std::cin >> x) {
    		if(index < MAX) {
			sera[index++] = x; 
			
		}
	}

	//time the program 
	clock_t startTime, endTime;
	startTime=clock();

	//get the number of elements to split up 
	n = index; 
	iterations = ceil((double)n/(double)NTHREADS); 
	if (n < NTHREADS) { 
		iterations = 1;
	}	
	//printf("n:%d it:%d nth:%d\n", n, iterations, NTHREADS); 
	
	//threading process
	int i, start, tids[NTHREADS];
	pthread_t threads[NTHREADS];
	pthread_attr_t attr;
	
	  //Pthreads setup: initialize mutex and explicitly create threads in a
     	  //joinable state (for portability).  Pass each thread its loop offset 
	  pthread_mutex_init(&prime_mutex, NULL);
	  pthread_attr_init(&attr);
	  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	  //create threads 
	  for (i=0; i<NTHREADS; i++) {
		//if(isprime == true) break; //stop making threads
		tids[i] = i; 
		pthread_create(&threads[i], &attr, search_for_p, (void *) &tids[i]);	 
		pthread_join(threads[i], NULL); 
	  }    
	
	  //time program end 
	  endTime=clock();
      	  float diff = (float)endTime - (float)startTime; 
	  double millisec =(diff)/(CLOCKS_PER_SEC/1000);
	  
	  //output 
	  char out; 
	  if(isprime == true) out = NEGATIVE; else out = POSITIVE;
	  printf("(Done)Prime status:%d  Time: %.5fms \n", out, millisec); 
	
	  //Clean up and exit 
	  pthread_attr_destroy(&attr);
	  pthread_mutex_destroy(&prime_mutex);
	  pthread_exit (NULL);
	
}

void *search_for_p(void *tid) 
{
	 if(isprime == true) //exit out
		pthread_exit(NULL);  
	
	  int j, start, *mytid, end;
	  mytid = (int *) tid; 
	  int rc; //for error checking 
	
	 //how many each thread will read 
	 start = (*mytid) * iterations; 
	 end = start + iterations;
	 if(end >= n) { end = n;}
	 
	//printf ("Thread %d doing iterations %d to %d\n",*mytid,start,end-1); 
	if(start < end || isprime != true) {
	   for(j=start; j < end; j++) {
		  
		   if(isprime == true) {
		   	pthread_exit(NULL); 
		   }
		   else if (is_prime(sera[j]) == 1) { // check if is a prime 
		   	     rc = pthread_mutex_lock (&prime_mutex);
			     if (rc) { /* an error has occurred */ perror("pthread_mutex_lock"); pthread_exit(NULL);}
			   
			     isprime = true;
			     
		             rc = pthread_mutex_unlock (&prime_mutex);
			     if (rc) { /* an error has occurred */ perror("pthread_mutex_unlock"); pthread_exit(NULL);}			   
		   } //end ifelse 
			//printf("Val:%d isprime: %d \n", sera[j], isprime);
	   }//end for 
	} //end if 
	
	pthread_exit(NULL);  

}

