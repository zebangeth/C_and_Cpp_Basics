#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freq_check(FILE * f) {
  int frq_map[26] = {0}; 
  int c; 

  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      c = tolower(c);
      frq_map[c - 'a'] ++; 
    }
  }

  int most_frq = 0; 
  int most_frq_idx = 0; 

  for (int i = 0; i < 26; i++) {
    if (frq_map[i] > most_frq) {
      most_frq = frq_map[i];
      most_frq_idx = i; 
    }
  }

  // Assuming the most frequent letter is 'e'
  printf("%d\n", (most_frq_idx + 22) % 26);
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Should have 2 arguments\n");
    return EXIT_FAILURE;
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }

  freq_check(f);

  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
