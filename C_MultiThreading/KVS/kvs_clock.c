#include "kvs_clock.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int return_evict_index(kvs_clock_t* kvs_clock, int currentLength);
int search(kvs_clock_t* kvs_clock, int currentCapacity, const char *key);

struct clock_cache{
  int bit; 
  char *key; 
  char *value; 
} clock_cache; 

struct kvs_clock {
  // TODO: add necessary variables
  struct clock_cache* clock;
  struct clock_cache* clock_hand; 
  kvs_base_t* kvs_base;
  int capacity;
  int current_capacity; 
  int clock_hand_index; 
};

kvs_clock_t* kvs_clock_new(kvs_base_t* kvs, int capacity) {
  kvs_clock_t* kvs_clock = malloc(sizeof(kvs_clock_t));
  kvs_clock->kvs_base = kvs;
  kvs_clock->clock_hand_index = 0; 
  kvs_clock->current_capacity = 0;
  kvs_clock->capacity = capacity; // NOTICE: Keep Track of
  kvs_clock->clock = calloc(capacity,sizeof(clock_cache));
  kvs_clock->clock_hand = kvs_clock->clock; 
  for( int i = 0; i < capacity; i++ ){
    kvs_clock->clock[i].bit = 0; 
  }
  return kvs_clock;
}

void kvs_clock_free(kvs_clock_t** ptr) {
  for(int i = 0; i < (*ptr)->current_capacity; i++){
    free((*ptr)->clock[i].key);
    free((*ptr)->clock[i].value);
  }
  free(*ptr);
  *ptr = NULL;
}

int kvs_clock_set(kvs_clock_t* kvs_clock, const char* key, const char* value) { 
  int index; 
  if( (index = search(kvs_clock, kvs_clock->current_capacity, (char*)key)) >= 0 ){
    kvs_clock->clock[index].value = strdup(value); 
    kvs_clock->clock[index].bit = 1; 
    return SUCCESS;
  }
  else if( kvs_clock->current_capacity == kvs_clock->capacity ){
    index = return_evict_index(kvs_clock, kvs_clock->current_capacity); 
    free(kvs_clock->clock[index].value);
    free(kvs_clock->clock[index].key);
    kvs_clock->clock[index].key = strdup(key);
    kvs_clock->clock[index].value = strdup(value);
    kvs_clock->clock[index].bit = 0; 
  } 
  else{
    kvs_clock->clock[kvs_clock->current_capacity].key = strdup(key);
    kvs_clock->clock[kvs_clock->current_capacity].value = strdup(value);
    kvs_clock->current_capacity++; 
  }
  return SUCCESS;
}

int kvs_clock_get(kvs_clock_t* kvs_clock, const char* key, char* value) {
  int index; 
  if( (index = search(kvs_clock, kvs_clock->current_capacity, (char*)key)) >= 0){
    kvs_clock->clock_hand[index].bit = 1; 
    return SUCCESS; 
  } 
  else{
    kvs_base_get(kvs_clock->kvs_base, key, value);
    return SUCCESS;
  }
}

int kvs_clock_flush(kvs_clock_t* kvs_clock) {
  for(int i = 0; i < kvs_clock->current_capacity; i++){
    kvs_base_set(kvs_clock->kvs_base, kvs_clock->clock[i].key, kvs_clock->clock[i].value);
  }
  return FAILURE;
}

int search(kvs_clock_t* kvs_clock, int currentCapacity, const char *key){
  for(int i = 0; i < currentCapacity; i++){
    if( !strcmp(kvs_clock->clock[i].key, key) ){
      return i; // reference to found value 
    }
  }
  return -1;
} 

int return_evict_index(kvs_clock_t* kvs_clock, int currentCapacity){
  int index = kvs_clock->clock_hand_index;
  for(int i = 0; i < currentCapacity; i++){
    if( kvs_clock->clock_hand[index].bit == 0 ){
      return index; // reference to found value 
    }
    else{
      kvs_clock->clock_hand[index].bit = 0;
    }
    index++; 
    if( index == currentCapacity ){ index = 0; }
  }
  return -1; //index to input
} 
