#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "outname.h"

char * computeOutputFileName(const char * inputName) {
  //WRITE ME
  if (inputName == NULL) {
    fprintf(stderr, "Can't open"); 
  }

  size_t size = strlen(inputName) + 8;
  char * output = malloc(size * sizeof(*output)); 

  snprintf(output, size, "%s.counts", inputName); 

  return output;
}
