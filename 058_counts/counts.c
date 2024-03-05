#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counts.h"
counts_t * createCounts(void) {
  //WRITE ME
  counts_t * res = malloc(sizeof(*res)); 
  res->array = NULL; 
  res->size = 0; 
  res->unknown = 0; 
  return res;
}
void addCount(counts_t * c, const char * name) {
  //WRITE ME
  if (name == NULL) {
    c->unknown++;
  } else {
    for (size_t i = 0; i < c->size; i++) {
    if (strcmp(c->array[i].input_name, name) == 0) {
      c->array[i].count++; 
      return; 
    }
  }
  c->array = realloc(c->array, (c->size + 1) * sizeof(*c->array)); 
  c->array[c->size].input_name = strdup(name); 
  c->array[c->size].count = 1; 
  c->size++;
  }
}

void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
  for (size_t i = 0; i < c->size; i++) {
    fprintf(outFile, "%s: %d\n", c->array[i].input_name, c->array[i].count); 
  }
  if (c->unknown > 0) {
    fprintf(outFile, "<unknown> : %zu\n", c->unknown); 
  }
}

void freeCounts(counts_t * c) {
  //WRITE ME
  for (size_t i = 0; i < c->size; i++) {
    free(c->array[i].input_name); 
  }
  free(c->array); 
  free(c);
}
