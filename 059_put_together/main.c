#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"
#include "counts.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  //WRITE ME
  FILE * f = fopen(filename, "r"); 
    if (f == NULL) {
      fprintf(stderr, "Can't open file"); 
      exit(EXIT_FAILURE); 
    }
  counts_t * counts = createCounts();
  char * line = NULL;
  size_t sz;
  while (getline(&line, &sz, f) >= 0) {
    char * newline = strchr(line, '\n');
    if (newline) {
        *newline = '\0'; 
    }
    char * value = lookupValue(kvPairs, line);
    addCount(counts, value);
    free(line);
    line = NULL;
  }
  free(line);
  assert(fclose(f) == 0);
  return counts;

}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
  if (argc < 3) {
    fprintf(stderr, "invalid input"); 
    return EXIT_FAILURE;
  }
 //read the key/value pairs from the file named by argv[1] (call the result kv)
  kvarray_t * kv = readKVs(argv[1]); 
 //count from 2 to argc (call the number you count i)
  for (int i = 2; i < argc; i++) {
    //count the values that appear in the file named by argv[i], using kv as the key/value pair
    //   (call this result c)
    counts_t * c = countFile(argv[i], kv);

    //compute the output file name from argv[i] (call this outName)
    char * outName = computeOutputFileName(argv[i]); 

    //open the file named by outName (call that f)
    FILE * f = fopen(outName, "w");
      if (f == NULL) {
        fprintf(stderr, "invalid input"); 
        return EXIT_FAILURE; 
      }

    //print the counts from c into the FILE f
    printCounts(c, f);

    //close f
    assert(fclose(f) == 0); 

    //free the memory for outName and c
    freeCounts(c); 
    free(outName); 
  }


 //free the memory for kv
  freeKVs(kv); 
  return EXIT_SUCCESS;
}
