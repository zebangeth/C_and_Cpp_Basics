#include <cstdio>
#include <cstdlib>

#include "function.h"

int binarySearchForZero(Function<int, int> *f, int low, int high) {
    if (low >= high) return low;

    while (low + 1 < high) {
        int mid = low + (high - low) / 2;
        int midVal = f->invoke(mid);

        if (midVal <= 0) {
            low = mid;
        } else {
            high = mid;
        }
    }

    // Check if the value at high - 1 is less than or equal to 0, if so return high - 1
    if (f->invoke(high - 1) <= 0) {
        return high - 1;
    }
    return low;
}
