#include "collatz.h"

int collatz_conjecture(int input){
    if (input%2==0){
        return input/2;
    }
    return 3*input+1;
}

int test_collatz_convergence(int input, int max_iter){
    int i=0;
    while (input != 1 && i<max_iter){
        input= collatz_conjecture(input);
        i++;
    }
    if(input == 1){
        return i;
    }
    return -1;
}