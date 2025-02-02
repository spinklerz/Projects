#include "dining.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
typedef int people;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t student_enters = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t student_leaves = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t uni = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cleaning_entry = PTHREAD_COND_INITIALIZER;
pthread_cond_t condition_cap = PTHREAD_COND_INITIALIZER;

typedef struct dining {
  // TODO: Add your variables here
  people capacity;
  people inside;
  people cleaning;
  people cleaner_waiting;
} dining_t;

dining_t *dining_init(int capacity) {
  // TODO: Initialize necessary variables

  dining_t *dining = calloc(1, sizeof(dining_t));
  if (!dining) {
    printf("Memory Allocation Failure\n");
  }
  dining->capacity = capacity;
  dining->inside = 0;
  dining->cleaning = 0;
  dining->cleaner_waiting = 0;
  return dining;
}
void dining_destroy(dining_t **dining) {
  // TODO: Free dynamically allocated memory
  free(*dining);
  *dining = NULL;
}

void dining_student_enter(dining_t *dining) {
  // TODO: Your code goes here
  pthread_mutex_lock(&student_enters);
  while ((dining->cleaning == 1 || dining->capacity == dining->inside) ||
         dining->cleaner_waiting == 1) {
    pthread_cond_wait(&condition_cleaning_entry, &student_enters);
  }
  dining->inside++;
  // printf("dining->cleaning = %d\ndining->capacity = %d\ndining->inside =
  // %d\n", dining->cleaning ,dining->capacity, dining->inside);
  pthread_mutex_unlock(&student_enters);
  // sem_post(&semaphore);
}
void dining_student_leave(dining_t *dining) {
  // TODO: Your code goes here
  pthread_mutex_lock(&student_leaves);
  /*while (dining->cleaning == 1) {
    pthread_cond_wait(&condition_cleaning_entry, &mutex);
  }*/
  if (dining->inside != 0) {
    dining->inside--;
  }
  if (dining->cleaner_waiting == 1 && dining->inside == 0) {
    pthread_cond_broadcast(&condition_cap);
  } else {
    pthread_cond_signal(&condition_cleaning_entry);
  }
  pthread_mutex_unlock(&student_leaves);
  // if student waiting let him in
}
void dining_cleaning_enter(dining_t *dining) {
  // TODO: Your code goes here
  // wait until capcaity==0
  pthread_mutex_lock(&uni);
  dining->cleaner_waiting = 1;
  while (dining->inside != 0 || dining->cleaning == 1) {
    pthread_cond_wait(&condition_cap, &uni);
  }
  dining->cleaner_waiting = 0;
  dining->cleaning = 1;
}

void dining_cleaning_leave(dining_t *dining) {
  // TODO: Your code goes here
  dining->cleaning = 0;
  pthread_mutex_unlock(&uni);
  pthread_cond_broadcast(&condition_cleaning_entry);
}
