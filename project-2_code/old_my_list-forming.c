
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

/*
void bind_thread_to_cpu(int cpuid) {
  cpu_set_t mask;
  CPU_ZERO(&mask);
  CPU_SET(cpuid, &mask);
  if (sched_setaffinity(0, sizeof(cpu_set_t), &mask)) {
    fprintf(stderr, "sched_setaffinity");
    exit(EXIT_FAILURE);
  }
}*/

struct Node *newNode() {
  struct Node *p;
  p = (struct Node *) malloc(sizeof(struct Node));

  if (NULL != p) {
    p -> next = NULL;
  } else {
    printf("Node allocation failed!\n");
  }
  return p;
}

void *producer_thread(void *arg) {

  //bind_thread_to_cpu(*((int*)arg));//bind this thread to a CPU

  struct Node *p, temp;
  struct list myList;
  myList.header = myList.tail = NULL;
  int i;
  for (i = 0; i < K; i++) {
    p = newNode();
    p -> data = 1; //generate data
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

  int NPROCESSORS;
  int *CPUs = NULL;

  struct Node *temp, *next;
  struct timeval t1, t2; // Start time and end time

  numThreads = atoi(argv[1]); //read numThreads from user
  pthread_t producer[numThreads];
  threadData datas[numThreads];

  // Get the processors
  NPROCESSORS = sysconf(_SC_NPROCESSORS_CONF);

  if (NPROCESSORS > 0) {
    CPUs = (int *) malloc(NPROCESSORS *sizeof(int));
    if (CPUs != NULL) {
      // Fill the CPU array with processors
      int i;
      for (i = 0; i < NPROCESSORS; i++) {
        CPUs[i] = i;
      }
    } else {
      printf("Allocation failed!\n");
      exit(0);
    }
  }

  pthread_mutex_init(&mutex_lock, NULL);

  List = (struct list *) malloc(sizeof(struct list));
  if (NULL == List) {
    printf("End here\n");
    exit(0);
  }
  List -> header = List -> tail = NULL;

  gettimeofday(&t1, NULL); //get program start time
  int i;
  for (i = 0; i < numThreads; i++) {
    pthread_create(&(producer[i]), NULL, (void *) producer_thread, NULL);
  }

  for (i = 0; i < numThreads; i++) {
    if (producer[i] != 0) {
      pthread_join(producer[i], NULL);
    }
  }
  gettimeofday(&t2, NULL); //get program end time

  if (List -> header != NULL) {
    next = temp = List -> header;
    while (temp != NULL) {
      next = temp -> next;
      free(temp);
      temp = next;
    }
  }
  if (CPUs != NULL) {
    free(CPUs);
  }
  /*calculate program runtime */
  printf("The program ran for %ld microseconds.\n",
  (t2.tv_sec - t1.tv_sec) *1000000 + (t2.tv_usec - t1.tv_usec));
  return 0;
}
