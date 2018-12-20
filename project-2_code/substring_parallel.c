#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024
#define NUM_THREADS 4

char *s1, *s2;
FILE *filePtr;

typedef struct threadStruct_ {
  int thread_ID;
  int thread_count; // keep track of the substring count for each thread
} threadStruct;
int l1, l2, ll;

int readStringsFile(FILE *filePtr) {
  filePtr = fopen("strings.txt", "r");
  if (filePtr == NULL) return 0;
  s1 = (char *) malloc(sizeof(char) *MAX);
  s2 = (char *) malloc(sizeof(char) *MAX);
  if(s1 == NULL || s2 == NULL) return -1;
  s1 = fgets(s1, MAX, filePtr);
  s2 = fgets(s2, MAX, filePtr);
  l1 = strlen(s1);
  l2 = strlen(s2) - 1;
}

int number_substring(void) {
  int i, j, k, l, totalCount = 0;
  for(i = 0, l = 0; i <= l1 - l2; i++) {
    for(j = i, k = 0; k < l2; j++, k++) {
      if(*(s1 + j) == *(s2 + k)) l++; // if the characters are equal
      else break;
      if(l2 == l) totalCount++;
    }
    l = 0;
  }
  return totalCount;
}

void parallel_number_substring(void *ptr) {
  int i, j, k, l, totalCount = 0, id = ((threadStruct *) ptr) -> thread_ID;
  for (i = id, l = 0; i <= l1 - l2; i += NUM_THREADS) {
    for (j = i, k = 0; k < l2; j++, k++) {
      if (*(s1 + j) == *(s2 + k)) l++; // if the character are equal
      else break;
      if (l2 == l) totalCount++;
    }
    l = 0;
  }
  ((threadStruct *) ptr) -> thread_count = totalCount;
}

int main(int argc, char *argv[]) {
  pthread_t thread0, thread1, thread2, thread3;
  threadStruct data0, data1, data2, data3;
  int count, thread_countSum = 0;

  data0.thread_ID = 0;
  data1.thread_ID = 1;
  data2.thread_ID = 2;
  data3.thread_ID = 3;

  readStringsFile(filePtr);
  count = ll = number_substring();

  pthread_create(&thread0, NULL, (void *) &parallel_number_substring, (void *) &data0);
  pthread_create(&thread1, NULL, (void *) &parallel_number_substring, (void *) &data1);
  pthread_create(&thread2, NULL, (void *) &parallel_number_substring, (void *) &data2);
  pthread_create(&thread3, NULL, (void *) &parallel_number_substring, (void *) &data3);

  pthread_join(thread0, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  thread_countSum += data0.thread_count;
  thread_countSum += data1.thread_count;
  thread_countSum += data2.thread_count;
  thread_countSum += data3.thread_count;

  printf("There were %d substrings,\n", count);
  printf("And there were %d substrings found in parallel!\n", thread_countSum);
  return 1;
}
