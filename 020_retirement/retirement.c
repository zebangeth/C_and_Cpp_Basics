#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
    int months; 
    double contribution; 
    double rate_of_return;
}; 

typedef struct _retire_info retire_info; 


void retirement (int startAge, double initial, 
                retire_info working, retire_info retired) {
    double balance = initial; 
    int age = startAge;
    for (int i = 0; i < working.months; i ++) {
        printf("Age %d month %d you have $%.2f\n", age / 12, age % 12, balance); 
        balance += (balance * working.rate_of_return) + working.contribution;
        age++;
    }
    for (int i = 0; i < retired.months; i ++) {
        printf("Age %d month %d you have $%.2f\n", age / 12, age % 12, balance); 
        balance += (balance * retired.rate_of_return) + retired.contribution;
        age++;
    }
}

int main() {
    retire_info working; 
    working.months = 489; 
    working.contribution = 1000; 
    working.rate_of_return = 0.045 / 12; 

    retire_info retired;
    retired.months = 384; 
    retired.contribution = -4000; 
    retired.rate_of_return = 0.01 / 12;

    retirement (327, 21345, working, retired);
    return EXIT_SUCCESS; 
}