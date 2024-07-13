#include <stdio.h>
#include <dlfcn.h>

int main() {
    int (*collatz_conjecture)(int input);
    int (*test_collatz_convergence)(int input, int max_iter);

    void* handle = dlopen("./libsharedcollatz.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Errror: %s\n", dlerror());
        return 1;
    }

    collatz_conjecture = dlsym(handle, "collatz_conjecture");
    test_collatz_convergence = dlsym(handle, "test_collatz_convergence");

    printf("%d\n", collatz_conjecture(11));
    printf("%d\n", test_collatz_convergence(11, 14));

    dlclose(handle);
}
