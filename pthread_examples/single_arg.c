#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define NUM_THREADS 4

void *thread_main(void *arg)
{
	int *tid;
	tid = (int *)arg;
	printf("Hello World! It's me, thread #%d!\n", *tid);
	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int rc, i;
	int pid[NUM_THREADS];
	for(i=0; i<NUM_THREADS; i++){
		//printf("In main: creating thread %ld\n", t);
		pid[i] = i;
		rc = pthread_create(&threads[i], NULL, thread_main, (void *) &pid[i]);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* Last thing that main() should do */
	pthread_exit(NULL);
}
