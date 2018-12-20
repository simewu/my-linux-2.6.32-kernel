#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define NUM_THREADS	4


static pthread_barrier_t b;

void *BusyWork(void *t)
{
   int i;
   long tid;
   double result=0.0;
   tid = (long)t;
   srandom((unsigned int) time(NULL));
   int rand = random() % 10;
   for (i=0; i<1000000*rand; i++)
   {
      result = result + sin(i) * tan(i);
   }
   printf("Thread %ld has done phase 1. Result = %e\n",tid, result);
   if (tid !=1)
   pthread_barrier_wait(&b);
   for (i=0; i<100; i++)
   {
      result = result + sin(i) * tan(i);
   }
   printf("Thread %ld has done phase 2. Result = %e\n",tid, result);
   pthread_exit((void*) t);
}

int main (int argc, char *argv[])
{
   pthread_t thread[NUM_THREADS];
   pthread_attr_t attr;
   int rc;
   long t;
   void *status;

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   pthread_barrier_init(&b, NULL, NUM_THREADS);

   for(t=0; t<NUM_THREADS; t++) {
      printf("Main: creating thread %ld\n", t);
      rc = pthread_create(&thread[t], &attr, BusyWork, (void *)t); 
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
      }

   /* Free attribute and wait for the other threads */
   pthread_attr_destroy(&attr);
#if 1
   for(t=0; t<NUM_THREADS; t++) {
      rc = pthread_join(thread[t], &status);
      if (rc) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
         }
//      printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
      }
#endif
printf("Main: program completed. Exiting.\n");
pthread_exit(NULL);
}
