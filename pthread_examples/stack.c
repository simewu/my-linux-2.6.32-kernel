#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define NUM_THREADS 4
#define NR_DATA 1310700

void *thread_main(void *arg)
{
	int *tid;
	tid = (int *)arg;
	long array[NR_DATA]; 
	printf("Hello World! It's me, thread #%d!\n", *tid);
	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int rc, i;
	int pid[NUM_THREADS];
	size_t stack_size;


	pthread_attr_t a;
	pthread_attr_init(&a);
	pthread_attr_getstacksize(&a, &stack_size);
	printf("Default stack size = %lu\n", stack_size);
	stack_size = 1895825408; 
	pthread_attr_setstacksize (&a, stack_size);

	for(i=0; i<NUM_THREADS; i++){
		//printf("In main: creating thread %ld\n", t);
		pid[i] = i;
		rc = pthread_create(&threads[i], &a, thread_main, (void *) &pid[i]);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* Last thing that main() should do */
	pthread_exit(NULL);
}
