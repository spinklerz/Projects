#include "kvs_fifo.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
struct kvs_fifo_cache{
  char* key; 
  char* value;
}kvs_fifo_cache;
struct kvs_fifo {
  // TODO: add necessary variables
  kvs_base_t* kvs_base;
  struct kvs_fifo_cache* head; 
  int capacity;
  int current_length;
};

void evict_fifo(kvs_fifo_t *kvs_fifo, const char *key, const char *value);
int search_fifo(kvs_fifo_t *kvs_fifo, const char* key);

kvs_fifo_t* kvs_fifo_new(kvs_base_t* kvs, int capacity) {
  kvs_fifo_t* kvs_fifo = malloc(sizeof(kvs_fifo_t));
  kvs_fifo->kvs_base = kvs;
  kvs_fifo->capacity = capacity; // NOTICE: Keep Track of
  struct kvs_fifo_cache* fifo = calloc(capacity, sizeof(kvs_fifo_cache));
  kvs_fifo->head = fifo;
  kvs_fifo->current_length = 0;
  return kvs_fifo;
}

void kvs_fifo_free(kvs_fifo_t** ptr) {
  // TODO: free dynamically allocated memory
  free(*ptr);
  *ptr = NULL;
}

int kvs_fifo_set(kvs_fifo_t* kvs_fifo, const char* key, const char* value) {
  // TODO: implement this function
  int index_val;
  if( (index_val = search_fifo(kvs_fifo, key)) != -1 ){ //update condition
    free(kvs_fifo->head[index_val].value);
    kvs_fifo->head[index_val].value = strdup(value);
    return SUCCESS;
  }
  else if( kvs_fifo->current_length == kvs_fifo->capacity ){
    evict_fifo(kvs_fifo, key, value);
    return SUCCESS; 
  } 
  else{
    kvs_fifo->head[kvs_fifo->current_length].key = strdup(key);
    kvs_fifo->head[kvs_fifo->current_length].value = strdup(value);
    kvs_fifo->current_length++; 
    return SUCCESS;
  }
}

int kvs_fifo_get(kvs_fifo_t* kvs_fifo, const char* key, char* value) {
  int index; 
  if( (index = search_fifo(kvs_fifo, key)) >= 0){
    value = kvs_fifo->head[index].value; 
    return SUCCESS;
  }
  else{
    kvs_base_get(kvs_fifo->kvs_base, key, value);
    kvs_fifo_set(kvs_fifo, key, value);
    return SUCCESS; 
  }
  return FAILURE;
}

int kvs_fifo_flush(kvs_fifo_t* kvs_fifo) {
  for(int i = 0; i < kvs_fifo->current_length; i++){
    kvs_base_set(kvs_fifo->kvs_base, kvs_fifo->head[i].key, kvs_fifo->head[i].value); 
  }
  return SUCCESS;
}

void evict_fifo(kvs_fifo_t *kvs_fifo, const char *key, const char *value){
  memmove(&(kvs_fifo->head[0]), &(kvs_fifo->head[1]), ( kvs_fifo->capacity - 1 ) * sizeof(kvs_fifo_cache));
  free(kvs_fifo->head[kvs_fifo->capacity - 1].key);
  kvs_fifo->head[kvs_fifo->capacity - 1].key = strdup(key);
  free(kvs_fifo->head[kvs_fifo->capacity - 1].value);
  kvs_fifo->head[kvs_fifo->capacity - 1].value = strdup(value);
}

int search_fifo(kvs_fifo_t *kvs_fifo, const char* key){
  struct kvs_fifo_cache* itor = &(kvs_fifo->head[0]); 
  for(int i = 0; i < kvs_fifo->current_length; i++){
    if(!strcmp(itor->key,key)){ 
      return i;
      }
    itor = &(kvs_fifo->head[i]); 
  }
  return -1; 
}
