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
    fprintf(stderr, "Could not open the story file.\n");
    return EXIT_FAILURE;
  }

  print_parse_story(f);

  if (fclose(f) != 0) {
    fprintf(stderr, "Could not close the story file.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
