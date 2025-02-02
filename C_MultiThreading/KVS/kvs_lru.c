#include "kvs_lru.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
struct kvs_lru {
  // TODO: add necessary variables
  struct kvs_lru_cache* head; 
  int current_length;  
  kvs_base_t* kvs_base;
  int capacity; 
}kvs_lru;

struct kvs_lru_cache{
  char* key;
  char* value;  
}kvs_lru_cache; 

int lru_search(kvs_lru_t* kvs_lru, char* key);
void evict_lru(kvs_lru_t* kvs_lru, char* key, char* value);
void update_cache(kvs_lru_t* kvs_lru, const char* key,const char* value);

kvs_lru_t* kvs_lru_new(kvs_base_t* kvs, int capacity) {
  kvs_lru_t* cache = malloc(sizeof(kvs_lru_t));
  struct kvs_lru_cache* cache_node = calloc(capacity ,sizeof(kvs_lru_cache));
  cache->head = cache_node;
  cache->capacity = capacity;
  cache->current_length = 0;
  cache->kvs_base = kvs;
  return cache;
}


void kvs_lru_free(kvs_lru_t** ptr) {
  // TODO: free dynamically allocated memory
  if (ptr && *ptr) {
    kvs_lru_t* cache = *ptr;
    for (int i = 0; i < cache->capacity; i++) {
      free(cache->head[i].key);
      free(cache->head[i].value);
    }
    free(cache->head);
    free(cache);
    *ptr = NULL;
  }
}

int kvs_lru_set(kvs_lru_t* kvs_lru, const char* key, const char* value) {
  // TODO: implement this function
  int index_val; 
  if( (index_val = lru_search(kvs_lru, (char*)key)) != -1 ){ //update condition
    free(kvs_lru->head[index_val].value);
    kvs_lru->head[index_val].value = strdup(value);
    update_cache(kvs_lru, key, value);
    //update position 
    return SUCCESS;
  }
  else if( kvs_lru->current_length == kvs_lru->capacity ){
    evict_lru(kvs_lru, (char*)key, (char*)value);
    return SUCCESS; 
  } 
  else{
    kvs_lru->head[kvs_lru->current_length].key = strdup(key);
    kvs_lru->head[kvs_lru->current_length].value = strdup(value);
    kvs_lru->current_length++; 
    return SUCCESS;
  }
}

int kvs_lru_get(kvs_lru_t* kvs_lru, const char* key, char* value) {
  // TODO: implement this function
  int index; 
  if( (index = lru_search(kvs_lru, (char*)key)) >= 0){
    update_cache(kvs_lru, key, value );
    value = kvs_lru->head[index].value;
    return SUCCESS;
  }
  else{
    kvs_base_get(kvs_lru->kvs_base, key, value);
    return SUCCESS; 
  }
  return FAILURE;
}

int kvs_lru_flush(kvs_lru_t* kvs_lru) {
  for(int i = 0; i < kvs_lru->current_length; i++){
    kvs_base_set(kvs_lru->kvs_base, kvs_lru->head[i].key, kvs_lru->head[i].value); 
  }
  return FAILURE;
}

int lru_search(kvs_lru_t* kvs_lru, char* key){
  struct kvs_lru_cache* itor = (kvs_lru->head); 
  for(int i = 0; i < kvs_lru->current_length; i++){
    if(!strcmp(itor->key,key)){ 
      return i;
      }
    itor++; 
  }
  return -1; 
}

void evict_lru(kvs_lru_t* kvs_lru, char* key, char* value){
  free(kvs_lru->head[kvs_lru->capacity - 1].key);
  free(kvs_lru->head[kvs_lru->capacity - 1].value);
  memmove(&kvs_lru->head[1], &kvs_lru->head[0], (kvs_lru->capacity - 1) * sizeof(kvs_lru_cache));
  kvs_lru->head[0].key = strdup(key);
  kvs_lru->head[0].value = strdup(value);
  kvs_lru->current_length = kvs_lru->capacity;
}

void update_cache(kvs_lru_t* kvs_lru, const char* key,const char* value){
  int l = lru_search(kvs_lru, (char*)key);
  char* tempKey = kvs_lru->head[l].key;
  char* tempValue = kvs_lru->head[l].value;
  memmove(&(kvs_lru->head[1]), &(kvs_lru->head[0]), l * sizeof(kvs_lru_cache));
  kvs_lru->head[0].key = tempKey;
  kvs_lru->head[0].value = tempValue;
  free(kvs_lru->head[0].value);
  kvs_lru->head[0].value = strdup(value);
  }
