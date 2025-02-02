#include "mr.h"

#include <ctype.h>
#include <dirent.h>
#include <err.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hash.h"
#include "kvlist.h"
#define LEN_HashTable 100000000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *reduce(void *args);
struct HashTableOBJ *itorHT;

void *mapp(void *args);
typedef struct HashTableOBJ {
  kvlist_t *HTlist;
  int hash;
  char *key;
} HashTableOBJ;

typedef struct ThreadMAPOBJ {
  kvlist_t *Lst;  // seperate list
  int ID;
  kvlist_t *out;
  mapper_t map;
} ThreadMAPOBJ;

typedef struct ThreadREDOBJ {
  struct HashTableOBJ *htptr;
  kvlist_t *out;
  reducer_t reduc;
  kvlist_t *Lst;
  int partition_length;
} ThreadREDOBJ;

void map_reduce(mapper_t mapper, size_t num_mapper, reducer_t reducer,
                size_t num_reducer, kvlist_t *input, kvlist_t *output) {
  // Phase 1:	*********************Split The Input Lists in num_mappers
  // lists**********************************
  (void)mapper;
  (void)reducer;
  (void)num_reducer;

  pthread_t thread_map[num_mapper];  // create num of threads
  struct ThreadMAPOBJ *ThreadList_map = malloc(
      sizeof(ThreadMAPOBJ) * (int)num_mapper);  // create num of thread structs

  for (size_t i = 0; i < num_mapper; i++) {
    ThreadList_map[i].Lst = kvlist_new();
    ThreadList_map[i].out = kvlist_new();
    ThreadList_map[i].ID = i;
    ThreadList_map[i].map = mapper;
  }

  kvlist_iterator_t *itor = kvlist_iterator_new(input);
  for (size_t i = 0;;) {
    kvpair_t *pair = kvlist_iterator_next(itor);
    if (pair == NULL) {
      break;
    }
    kvlist_append(ThreadList_map[i].Lst, kvpair_new(pair->key, pair->value));
    i++;
    if (i == num_mapper) {
      i = 0;
    }
  }
  kvlist_iterator_free(&itor);

  // Phase 2:	*********************Create Threads and Call Mapper
  // function**********************************
  for (size_t i = 0; i < num_mapper; i++) {
    pthread_create(&thread_map[i], NULL, mapp, (void *)&ThreadList_map[i]);
  }

  for (size_t i = 0; i < num_mapper; i++) {
    pthread_join(thread_map[i], NULL);
  }

  // Intermidiate Phase:	*********************If num_mapper !=
  // num_reducor********************************** 1.) Concat mapper lists 2.)
  // Then redistribute lists into num_reducor lists
  kvlist_t *filler = kvlist_new();
  // Struct Reducser Initialization
  struct ThreadREDOBJ *ThreadList_reduce =
      malloc(sizeof(ThreadREDOBJ) * (int)num_reducer);
  int p = (int)LEN_HashTable / (int)num_reducer;
  int pr = (int)LEN_HashTable % (int)num_reducer;
  for (size_t i = 0; i < num_reducer; i++) {
    ThreadList_reduce[i].out = kvlist_new();
    ThreadList_reduce[i].reduc = reducer;
    ThreadList_reduce[i].partition_length = p;
    ThreadList_reduce[i].Lst = kvlist_new();
  }
  for (size_t i = 0; i < num_mapper; i++) {
    kvlist_extend(filler, ThreadList_map[i].out);
  }
  kvlist_iterator_t *itora = kvlist_iterator_new(filler);
  for (size_t i = 0;;) {
    kvpair_t *pair = kvlist_iterator_next(itora);
    if (pair == NULL) {
      break;
    }
    if (i == num_reducer) {
      i = 0;
    }
    kvlist_append(ThreadList_reduce[i].Lst, kvpair_new(pair->key, pair->value));
    i++;
  }
  kvlist_iterator_free(&itora);
  kvlist_free(&filler);

  // Phase 3:	*********************Create
  // HashTable**********************************
  HashTableOBJ *ht = calloc((int)LEN_HashTable, sizeof(HashTableOBJ));
  int hashed_value;
  for (size_t i = 0; i < num_reducer; i++) {
    kvlist_iterator_t *itorator = kvlist_iterator_new(ThreadList_reduce[i].Lst);
    for (;;) {
      kvpair_t *pair = kvlist_iterator_next(itorator);
      if (pair == NULL) {
        break;
      }
      hashed_value = hash(pair->key) % LEN_HashTable;
      if (ht[hashed_value].hash == 0) {
        ht[hashed_value].HTlist = kvlist_new();
        ht[hashed_value].key = malloc(strlen(pair->key));
        strcpy(ht[hashed_value].key, pair->key);
        ht[hashed_value].hash = hashed_value;
      }
      kvlist_append(ht[hashed_value].HTlist, kvpair_new(pair->key, "1"));
      ht[hashed_value].hash = hashed_value;
    }
    kvlist_iterator_free(&itorator);
  }

  // Phase 4:	*********************Use Threads Reducer
  // Function**********************************
  itorHT = ht;
  // printf("\nPhase 2:\n");
  pthread_t thread_reduce[num_reducer];
  printf("Phase 1:\n");
  ThreadList_reduce[0].partition_length = pr + p;
  for (size_t i = 0; i < num_reducer; i++) {
    pthread_create(&thread_reduce[i], NULL, reduce,
                   (void *)&ThreadList_reduce[i]);
  }
  for (size_t i = 0; i < num_reducer; i++) {
    pthread_join(thread_reduce[i], NULL);
  }

  // concat
  // kvlist_print(1, ThreadList_reduce[0].out);
  for (size_t i = 0; i < num_reducer; i++) {
    kvlist_extend(output, ThreadList_reduce[i].out);
  }

  // Freeing Functions
  // -----------------------------------------------------------------------------
  for (size_t i = 0; i < num_mapper; i++) {
    kvlist_free(&ThreadList_map[i].Lst);
    kvlist_free(&ThreadList_map[i].out);
  }
  free(ThreadList_map);

  for (size_t i = 0; i < num_reducer; i++) {
    kvlist_free(&ThreadList_reduce[i].Lst);
    kvlist_free(&ThreadList_reduce[i].out);
  }
  free(ThreadList_reduce);

  for (size_t i = 0; i < LEN_HashTable; i++) {
    if (ht[i].hash == 0) {
      continue;
    }
    kvlist_free(&ht[i].HTlist);
    free(ht[i].key);
  }
  free(ht);
    kvlist_sort(output);
}

void *mapp(void *args) {
  struct ThreadMAPOBJ *ThreadList = (struct ThreadMAPOBJ *)args;
  kvlist_iterator_t *itor = kvlist_iterator_new(ThreadList->Lst);
  for (;;) {
    kvpair_t *pair = kvlist_iterator_next(itor);
    if (pair == NULL) {
      break;
    }
    ThreadList->map(pair, ThreadList->out);
  }
  kvlist_iterator_free(&itor);
  return NULL;
}

void *reduce(void *args) {
  struct ThreadREDOBJ *ThreadList = (struct ThreadREDOBJ *)args;
  pthread_mutex_lock(&mutex);
  int length = ThreadList->partition_length;
  for (int i = 0; i != length; length--) {
    if (itorHT->hash == 0) {
      itorHT++;
      continue;
    }
    ThreadList->reduc(itorHT->key, itorHT->HTlist, ThreadList->out);
    itorHT++;
  }
  pthread_mutex_unlock(&mutex);
  return NULL;
}
