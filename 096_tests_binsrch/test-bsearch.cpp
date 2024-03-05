#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <climits>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high);

class CountedIntFn : public Function<int, int> {
  protected:
    unsigned remaining;
    Function<int, int> * f;
    const char * mesg;

  public:
    CountedIntFn(unsigned n, Function<int, int> *fn, const char *m) : remaining(n), f(fn), mesg(m) {}

    virtual int invoke(int arg) {
        if (remaining == 0) {
            fprintf(stderr, "Too many function invocations in %s\n", mesg);
            exit(EXIT_FAILURE);
        }
        remaining--;
        return f->invoke(arg);
    }
};

class LinearFunction : public Function<int, int> {
  public:
    virtual int invoke(int arg) {
        return arg * 1;
    }
};

class SinFunction : public Function<int, int> {
  public:
    virtual int invoke(int arg) {
        return 10000000 * (sin(arg/100000.0) - 0.5);
    }
};


void check(Function<int, int> * f, int low, int high, int expected_ans, const char * mesg) {
    unsigned maxInvokes = (high > low) ? (log2(high - low) + 1) : 1;
    
    CountedIntFn * fun = new CountedIntFn(maxInvokes, f, mesg);

    assert(binarySearchForZero(fun, low, high) == expected_ans);
}

int main() {
    LinearFunction * linfun = new LinearFunction();
    SinFunction * sinfun = new SinFunction();
    check(linfun, 1, 5, 1, "lin 1 5 1");
    check(linfun, -10, -1, -2, "lin -10 -1 -2");
    check(linfun, -5, -1, -2, "lin -5 -1 -2");
    check(linfun, -5, 0, -1, "");
    check(linfun, -1, 4, 0, "-1 4 0");
    check(linfun, 5, 5, 5, "");
    check(linfun, -5, -5, -5, "");
    check(linfun, 0, 0, 0, "");
    check(linfun, 0, 5, 0, "");
    check(linfun, 2, 3, 2, "lin 2 3 2");
    check(linfun, -10, 10, 0, "lin -10 10 0");
    check(linfun, -10000, 10000, 0, "lin -10000 10000 0");


    check(sinfun, 0, 150000, 52359, "sin");

    return (EXIT_SUCCESS);
}
