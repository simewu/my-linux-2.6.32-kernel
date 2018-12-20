
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/time.h>
#include <unistd.h>

#define K 200 // K times for each thread

struct Node {
  int data;
  struct Node *next;
};

struct list {
  struct Node *header;
  struct Node *tail;
};

struct list *List;
pthread_mutex_t mutex_lock;

typedef struct threadData {
  int CPU;
  struct list myList;
} threadData;

struct Node *newNode() {
  struct Node *p;
  p = (struct Node *) malloc(sizeof(struct Node));

  if (NULL != p) {
    p -> next = NULL;
  }
  return p;
}

void *producer_thread(void *arg) {
  struct Node *p, temp;
  struct list myList;
  myList.header = myList.tail = NULL;
  int i;
  for (i = 0; i < K; i++) {
    p = newNode();
    p -> data = 1;
    if (myList.header == NULL) {
      myList.header = myList.tail = p;
    } else {
      myList.tail -> next = p;
      myList.tail = p;
    }
  }
  pthread_mutex_lock(&mutex_lock);
  if (List -> header == NULL) {
    List -> header = myList.header;
    List -> tail = myList.tail;
  } else {
    List -> tail -> next = myList.header;
    List -> tail = myList.tail;
  }
  pthread_mutex_unlock(&mutex_lock);
}


int main(int argc, char *argv[]) {
  int numThreads;

  int *CPUs = NULL;

  struct Node *temp, *next;
  struct timeval t1, t2; // Start time and end time

  numThreads = atoi(argv[1]); //read numThreads from user
  pthread_t producer[numThreads];
  threadData datas[numThreads];

  // Get the processors
  int numberOfProcessors = sysconf(_SC_NPROCESSORS_CONF), i;

  if (numberOfProcessors > 0) {
    CPUs = (int *) malloc(numberOfProcessors * sizeof(int));
    if (CPUs != NULL) {
      // Fill the CPU array with processors
      for (i = 0; i < numberOfProcessors; i++) {
        CPUs[i] = i;
      }
    } else exit(0);
  }


  pthread_mutex_init(&mutex_lock, NULL);

  List = (struct list *) malloc(sizeof(struct list));
  if (List == NULL) exit(0);
  List -> header = List -> tail = NULL;

  gettimeofday(&t1, NULL); //Get program start time
  for (i = 0; i < numThreads; i++) {
    pthread_create(&(producer[i]), NULL, (void *) producer_thread, NULL);
  }

  for (i = 0; i < numThreads; i++) {
    if (producer[i] != 0) {
      pthread_join(producer[i], NULL);
    }
  }
  gettimeofday(&t2, NULL); //Get program end time

  if (List -> header != NULL) {
    next = temp = List -> header;
    while (temp != NULL) {
      next = temp -> next;
      free(temp);
      temp = next;
    }
  }
  if (CPUs != NULL) free(CPUs); // Prevent memory issues
  
  printf("The program runtime was %ld microseconds\n",
  (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec));
  return 0;
}
