#include <stdio.h>
#include <stdlib.h>

size_t  maxSeq(int * array, size_t n) {
    if (n == 0) { 
        return 0; 
    }

    size_t maxLen = 1;
    size_t curLen = 1; 
    int lastNum = array[0]; 
    for (int i = 1; i < n; i ++) {
        if (array[i] > lastNum) {
            curLen ++; 
        } else {
            curLen = 1;            
        }
        if (maxLen < curLen) {
            maxLen = curLen; 
        }
        lastNum = array[i];
    }
    return maxLen; 
}