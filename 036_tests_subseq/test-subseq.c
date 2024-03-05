#include <stdio.h>
#include <stdlib.h>

size_t  maxSeq(int * array, size_t n);

void run_check(int * array, size_t n, size_t expected) {
    size_t result = maxSeq(array, n);
    if (result != expected) {
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    int arr1[] = {1, 2, 3};
    run_check(arr1, 3, 3); 
    int arr2[] = {3, 2, 1};
    run_check(arr2, 3, 1); 

    run_check(NULL, 0, 0); 
    
    int arr4[] = {1};
    run_check(arr4, 1, 1); 

    int arr5[] = {1, 2, 2, 3, 4};
    run_check(arr5, 5, 3); 

    int arr6[] = {1, 3, 5, 2, 4, 6, 8};
    run_check(arr6, 7, 4);

    int arr7[5];
    run_check(arr7, 5, 1); 

    int arr8[] = {-3, -2, -1, 1};
    run_check(arr8, 4, 4);

    return EXIT_SUCCESS; 
}
