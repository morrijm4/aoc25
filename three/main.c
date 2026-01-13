#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define N 12
#define LEN 100
// #define LEN 15

uint64_t find_max_joltage(char *line) {
    int max_i[N];
    int i = 0;

    for (int j = 0; j < N; j++) {
        max_i[j] = i;
        // printf("mx: %d\n", max_i[j]);

        while (i < (LEN - (N - j - 1)) && line[i] != '\n') {
            if (line[i] > line[max_i[j]]) {
                max_i[j] = i;
            }

            i += 1;
        }

        i = max_i[j] + 1;
    }

    char tmp[N + 1];
    tmp[N] = '\0';

    for (int i = 0; i < N; i++) {
        tmp[i] = line[max_i[i]];
    }

    return strtoull(tmp, NULL, 10);
}

int main() {
    FILE *file = fopen("real.txt", "r");

    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    char *linep = NULL;
    size_t linecap = 0;
    uint64_t total = 0;

    while (getline(&linep, &linecap, file) > 0) {
        total += find_max_joltage(linep);
    }

    printf("Total output joltage: %llu\n", total);

    return EXIT_SUCCESS;
}
