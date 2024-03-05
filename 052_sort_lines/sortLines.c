#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering of the strings
//in qsort. You do not need to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void process_file(FILE * stream) {
  char ** lines = NULL;  // 存储读取的行的指针数组
  size_t numOfLines = 0; // 已读取的行数
  char * currLine = NULL; // 当前读取的行
  size_t len;

  while (getline(&currLine, &len, stream) >= 0) {
    lines = realloc(lines, (numOfLines + 1) * sizeof(*lines));
    lines[numOfLines] = currLine;
    currLine = NULL;
    numOfLines++;
  }
  free(currLine);

  sortData(lines, numOfLines);

  for (size_t i = 0; i < numOfLines; i++) {
    printf("%s", lines[i]);
    free(lines[i]);
  }
  free(lines);
}

int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
  if (argc == 1) {
    FILE * f = stdin; 
    if (!f) {
      fprintf(stderr, "File is null"); 
      exit(EXIT_FAILURE); 
    } 
    process_file(stdin); 
  } else {
    for (size_t i = 1; i < argc; i++) {
      FILE * f = fopen(argv[i], "r");
      if (!f) {
        fprintf(stderr, "File is null");
        return EXIT_FAILURE;
      }
      process_file(f);
      
      if (fclose(f) != 0) {
        fprintf(stderr, "Error closing file");
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}
