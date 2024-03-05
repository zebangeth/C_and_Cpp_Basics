#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"

kvpair_t * fetch_KV(char * line) {
    if (!line) {
        fprintf(stderr, "Input line cannot be fetched.\n");
        exit(EXIT_FAILURE);
    }

    char * div_index = strchr(line, '=');
    if (!div_index) {
        fprintf(stderr, "Input line cannot be fetched.\n");
        exit(EXIT_FAILURE);
    }

    if (div_index == line) {
        fprintf(stderr, "No key present in the line.\n");
        exit(EXIT_FAILURE);
    }

    char * end_of_value = strchr(div_index + 1, '\n');
    if (!end_of_value) {
        end_of_value = strchr(div_index + 1, '\0');
    }

    size_t key_len = div_index - line;
    size_t value_len = end_of_value - div_index - 1;

    char * key = strndup(line, key_len);
    char * value = strndup(div_index + 1, value_len);

    kvpair_t * kv = malloc(sizeof(*kv)); 
    kv->key = key;
    kv->value = value;

    return kv;
}


kvarray_t * readKVs(const char * fname) {
  //WRITE ME
  FILE * f = fopen(fname, "r"); 
  if (f == NULL) {
    fprintf(stderr, "Input file is NULL.\n"); 
    exit(EXIT_FAILURE); 
  }

  kvarray_t * out_array = malloc(sizeof(*out_array)); 
  out_array->array = NULL; 
  out_array->size = 0; 

  char * line = NULL; 
  size_t sz; 
  while (getline(&line, &sz, f) >= 0) {
    kvpair_t * kv = fetch_KV(line); 
    
    out_array->size++; 
    out_array->array = realloc(out_array->array, out_array->size * sizeof(*out_array->array)); 
    out_array->array[out_array->size - 1] = kv; 
    free(line); 
    line = NULL; 
  }
  free(line); 
  assert(fclose(f) == 0); 

  return out_array;
}

void freeKVs(kvarray_t * pairs) {
  //WRITE ME
  for (size_t i = 0; i < pairs->size; i++) {
    free(pairs->array[i]->key); 
    free(pairs->array[i]->value); 
    free(pairs->array[i]); 
  }
  free(pairs->array); 
  free(pairs); 
}

void printKVs(kvarray_t * pairs) {
  //WRITE ME
  for (size_t i = 0; i < pairs->size; i++) { 
    printf("key = '%s' value = '%s'\n", pairs->array[i]->key, pairs->array[i]->value); 
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  for (size_t i = 0; i < pairs->size; i++) {
    if (strcmp(pairs->array[i]->key, key) == 0) {
      return pairs->array[i]->value; 
    }
  }
  return NULL; 
}
