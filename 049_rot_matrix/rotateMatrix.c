#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rotate(char matrix[10][10]) {
  int n = 10; 
  char temp; 

  // Flip over the diagonal
  for (int r = 0; r < n; r++) {
    for (int c = r; c < n; c++) {
      temp = matrix[r][c];
      matrix[r][c] = matrix[c][r];
      matrix[c][r] = temp;
    }
  }

  // Flip each row
  for (int r = 0; r < n; r++) {
    for (int c = 0; c < n / 2; c++) {
      temp = matrix[r][c]; 
      matrix[r][c] = matrix[r][n - c - 1]; 
      matrix[r][n - c - 1] = temp; 
    }
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid argument\n");
    exit(EXIT_FAILURE);
  }

  FILE *f = fopen(argv[1], "r"); 
  if (!f) {
    fprintf(stderr, "File is NULL\n"); 
    exit(EXIT_FAILURE); 
  }

  char matrix[10][10]; 

  for (int i = 0; i < 10; i++) {
    int count = 0; 
    for (int j = 0; j < 10; j ++) {
      int ch = fgetc(f); 
      if (ch >= 128) {
        fprintf(stderr, "Invalid (non-ASCII char)\n");
        fclose(f);
        exit(EXIT_FAILURE);
      }
      if (ch == EOF) {
        fprintf(stderr, "Matrix less than 10 * 10\n");
        fclose(f);
        exit(EXIT_FAILURE);
      }
      matrix[i][j] = ch;
      count ++; 
    }
    int newline = fgetc(f); 
    if (newline != '\n' || count != 10) {
      fprintf(stderr, "Matrix size or format is invalid\n");
      fclose(f);
      exit(EXIT_FAILURE);
    }
  }

  // Check if there are extra characters after the 10x10 matrix
  if (fgetc(f) != EOF) {
    fprintf(stderr, "Extra characters found after the 10x10 matrix\n");
    fclose(f);
    exit(EXIT_FAILURE);
  }

  rotate(matrix); 

  // print rotated matrix
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }

  fclose(f);
  return EXIT_SUCCESS; 
}
