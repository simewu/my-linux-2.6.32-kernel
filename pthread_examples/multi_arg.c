#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define NUM_THREADS 4

struct thread_data{
   int  tid;
   char sum;
};


void *thread_main(void *arg)
{
	struct thread_data *thread_data;
	thread_data = (struct thread_data *)arg;
	printf("Hello World! It's me, thread #%d and my sum is %d\n", thread_data->tid, thread_data->sum);
	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int rc, i, sum=0;
	struct thread_data thread_data_array[NUM_THREADS];
	for(i=0; i<NUM_THREADS; i++){
		sum = sum + i;
		thread_data_array[i].tid = i;
		thread_data_array[i].sum = sum;
		rc = pthread_create(&threads[i], NULL, thread_main, (void *) &thread_data_array[i]);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* Last thing that main() should do */
	pthread_exit(NULL);
}
