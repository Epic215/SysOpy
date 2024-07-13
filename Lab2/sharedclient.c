#include <stdio.h>
#include "collatz.h"

int main() {
    printf("%d\n", collatz_conjecture(11));
    printf("%d\n", test_collatz_convergence(11, 15));
}