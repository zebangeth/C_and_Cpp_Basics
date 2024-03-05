#include "rand_story.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Invalid input arguments.\n");
    return EXIT_FAILURE;
  }

  FILE *f_words = fopen(argv[1], "r"); 
  FILE *f_story = fopen(argv[2], "r"); 
  if (!f_words || !f_story) {
    fprintf(stderr, "Could not open files.\n");
    if (f_words) fclose(f_words);
    if (f_story) fclose(f_story);
    return EXIT_FAILURE;
  }

  // create and parse categories
  catarray_t * categories = create_catarray();
  if (categories == NULL) {
    fprintf(stderr, "Could not create categories.\n");
    if (fclose(f_words) != 0 || fclose(f_story) != 0) {
      fprintf(stderr, "Could not close files.\n");
      return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
  }
  parse_categories(f_words, categories);

  print_story_with_catwords(f_story, categories);

  if (fclose(f_words) != 0 || fclose(f_story) != 0) {
    fprintf(stderr, "Could not close files.\n");
    return EXIT_FAILURE;
  }

  free_catarray(categories);

  return EXIT_SUCCESS;
}
