#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <x86intrin.h>

// @todo consider to log/ignore outliers
uint64_t measure_cycles(int count, size_t (*func)(const char *), const char *str) {
    uint64_t start = __rdtsc();
    for (int = 0; i < count; ++i) {
        func(str);
    }
    uint64_t end = __rdtsc();

    return end - start;
}

void compare_strlen(int count, const char *str) {
    uint64_t normal_cycles = measure_cycles(count, strlen, str);
    uint64_t optimized_cycles = measure_cycles(count, strlen_optimized, str);

    printf("String length: %zu\n", strlen(str));
    printf("Normal strlen cycles: %lu\n", normal_cycles);
    printf("Optimized strlen cycles: %lu\n", optimized_cycles);
    printf("Speedup: %.2fx\n", (double)normal_cycles / optimized_cycles);
}

char* generate_random_string(size_t length) {
    char *str = (char *) malloc(length + 1);
    for (size_t i = 0; i < length; i++) {
        str[i] = (char) rand();
    }

    str[length] = '\0';

    return str;
}

int main() {
    srand((unsigned int) time(NULL));

    size_t lengths[] = {5, 16, 64, 256, 1024, 4096};
    const size_t num_lengths = sizeof(lengths) / sizeof(lengths[0]);

    for (size_t i = 0; i < num_lengths; i++) {
        size_t length = lengths[i];
        char *random_string = generate_random_string(length);

        printf("Test %zu (length: %zu):\n", i + 1, length);
        compare_strlen(100000, random_string);

        free(random_string);
        printf("\n");
    }

    return 0;
}