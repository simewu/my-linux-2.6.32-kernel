#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE_SIZE 5

pthread_mutex_t mutex;

char queue[QUEUE_SIZE];
pthread_cond_t emptySignal, fullSignal;
int i = 0, j = 0, k = 0;

void produce(FILE *filePtr) {
  char str = fgetc(filePtr);
  while (!k) {
    pthread_mutex_lock(&mutex);
    queue[i] = str;
    if (i % QUEUE_SIZE == j) pthread_cond_signal(&emptySignal);
    i = (i + 1) % QUEUE_SIZE;
    if (i == j) pthread_cond_wait(&fullSignal, &mutex);
    pthread_mutex_unlock(&mutex);

    str = fgetc(filePtr);
    if (feof(filePtr)) {
      pthread_mutex_lock(&mutex);
      k = 1;
      pthread_cond_signal(&emptySignal);
      pthread_mutex_unlock(&mutex);
      //break;
    }
  }
}

void consume(void *filePtr) {
  while (!k) {
    pthread_mutex_lock(&mutex);
    if (i == j) {
      if (!k) {
        pthread_cond_wait(&emptySignal, &mutex);
        if (k && i == j) break;
      } else break;
    }
    // Print a single character from the file
    printf("%c", queue[j]);
    if (i == j % QUEUE_SIZE) pthread_cond_signal(&fullSignal);
    j = (j + 1) % QUEUE_SIZE;
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t producerThread, consumerThread;
  FILE *filePtr = fopen("message.txt", "r");
  if (filePtr == NULL) return -1;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&emptySignal, NULL);
  pthread_cond_init(&fullSignal, NULL);
  pthread_create(&producerThread, NULL, (void *) &produce, (FILE *) filePtr);
  pthread_create(&consumerThread, NULL, (void *) &consume, NULL);

  pthread_join(producerThread, NULL);
  pthread_join(consumerThread, NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&emptySignal);
  pthread_cond_destroy(&fullSignal);
  
  printf("\nThe file has beec successfully read!\n");
  return 1;
}
