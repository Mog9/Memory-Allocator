#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include "allocator.h"

const int ITERATIONS = 1000000;

long benchmark_my(size_t size) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < ITERATIONS; i++) {
        void* ptr = mymalloc(size);
        myfree(ptr);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
}

long benchmark_std(size_t size) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < ITERATIONS; i++) {
        void* ptr = malloc(size);
        free(ptr);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
}

void print_result(const char* label, long my_ns, long std_ns) {
    std::cout << "\n" << label << "\n";
    std::cout << "mymalloc: " << my_ns / 1e6 << " ms\n";
    std::cout << "malloc: " << std_ns / 1e6 << " ms\n\n";
}

int main() {
    init_pool();
    struct timespec start, end;

    // test 1: single size
    print_result("single size (4 bytes)",
        benchmark_my(sizeof(int)),
        benchmark_std(sizeof(int)));

    // test 2: mixed sizes
    size_t sizes[] = {8, 16, 64, 128, 256, 512};
    long my_ns = 0, std_ns = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < ITERATIONS; i++) {
        size_t s = sizes[i % 6];
        void* ptr = mymalloc(s);
        myfree(ptr);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    my_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < ITERATIONS; i++) {
        size_t s = sizes[i % 6];
        void* ptr = malloc(s);
        free(ptr);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    std_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    print_result("mixed sizes (8-512 bytes)", my_ns, std_ns);

    // test 3: bulk allocate then free
    const int BULK = 1000;
    void* my_ptrs[BULK];
    void* std_ptrs[BULK];

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BULK; i++) my_ptrs[i] = mymalloc(sizeof(int));
    for (int i = 0; i < BULK; i++) myfree(my_ptrs[i]);
    clock_gettime(CLOCK_MONOTONIC, &end);
    my_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BULK; i++) std_ptrs[i] = malloc(sizeof(int));
    for (int i = 0; i < BULK; i++) free(std_ptrs[i]);
    clock_gettime(CLOCK_MONOTONIC, &end);
    std_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    print_result("bulk allocate 1000 then free all", my_ns, std_ns);

    // test 4: random order free
    void* rptrs[BULK];
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BULK; i++) rptrs[i] = mymalloc(sizeof(int));
    for (int i = BULK - 1; i >= 0; i--) myfree(rptrs[i]);
    clock_gettime(CLOCK_MONOTONIC, &end);
    my_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);

    void* srptrs[BULK];
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < BULK; i++) srptrs[i] = malloc(sizeof(int));
    for (int i = BULK - 1; i >= 0; i--) free(srptrs[i]);
    clock_gettime(CLOCK_MONOTONIC, &end);
    std_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
    print_result("reverse order free", my_ns, std_ns);
}