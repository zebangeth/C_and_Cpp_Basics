#include "rand_story.h"
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Invalid input argument.\n");
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (!f) {
    fprintf(stderr, "Could not open the words file.\n");
    return EXIT_FAILURE; 
  }

  catarray_t * categories = create_catarray(); 
  if (categories == NULL) {
    fprintf(stderr, "Could not create categories.\n");
    if (fclose(f) != 0) {
      fprintf(stderr, "Could not close the words file.\n");
      return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
  }

  parse_categories(f, categories); 
  
  if (fclose(f) != 0) {
    fprintf(stderr, "Could not close the words file.\n");
    return EXIT_FAILURE;
  }

  printWords(categories); 
  
  free_catarray(categories);

  return EXIT_SUCCESS;
}
