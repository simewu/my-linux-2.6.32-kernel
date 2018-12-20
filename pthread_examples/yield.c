#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS	4

void *BusyWork(void *t)
{
   int i;
   long tid;
   double result=0.0;
   tid = (long)t;
   for (i=0; i<1000000; i++)
   {
	   result = result + sin(i) * tan(i);
#if 1
	   if (tid == 1)
		   pthread_yield();
#endif
   }
   printf("Thread %ld done. Result = %e\n",tid, result);
   pthread_exit((void*) t);
}

int main (int argc, char *argv[])
{
   pthread_t thread[NUM_THREADS];
   pthread_attr_t attr;
   cpu_set_t c;
   int rc;
   long t;
   void *status;

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   CPU_ZERO(&c);
   CPU_SET(0, &c);
   CPU_SET(1, &c);
   CPU_SET(2, &c);
   CPU_SET(3, &c);
   pthread_attr_setaffinity_np(&attr, sizeof(c), &c);

   for(t=0; t<NUM_THREADS; t++) {
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
   }
#endif
printf("Main: program completed. Exiting.\n");
pthread_exit(NULL);
}
