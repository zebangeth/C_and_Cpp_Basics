#include <stdio.h>
#include <cstdlib>
#include "readFreq.h"

void printSym(std::ostream & s, unsigned sym) {
  if (sym > 256 ) {
    s << "INV";
  }
  else if (sym == 256) {
    s << "EOF";
  }
  else if (isprint(sym)) {
    char c = sym;
    s << "'" << c << "'";
  }
  else {
    std::streamsize w = s.width(3);
    s << std::hex << sym << std::dec;
    s.width(w);
  }
}
uint64_t * readFrequencies(const char * fname) {
    FILE * f = fopen(fname, "r");
    if (!f) {
        fprintf(stderr, "Failed to open file");
        exit(EXIT_FAILURE);
    }
    uint64_t * freq = new uint64_t[257]();
    int c;
    while ((c = fgetc(f)) != EOF) {
        freq[c]++;
    }
    freq[256] = 1;
    fclose(f);
    return freq;
}
