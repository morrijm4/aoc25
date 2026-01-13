#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int *items;
    size_t count;
    size_t capacity;
    char equation;
} Problem;

typedef struct {
    Problem *items;
    size_t count;
    size_t capacity;
} Problems;

#define da_append(arr, item)\
    do {\
        if (arr.capacity <= arr.count) {\
            if (arr.capacity == 0) arr.capacity = 256;\
            else arr.capacity *= 2;\
            arr.items = realloc(arr.items, arr.capacity * sizeof(*arr.items));\
        }\
        arr.items[arr.count++] = item;\
    } while (0)

uint64_t solve(Problem pb) {
    uint64_t total = pb.items[0];

    for (int i = 1; i < pb.count; i++) {
        switch (pb.equation) {
            case '+':
                total += pb.items[i];
                break;
            case '*':
                total *= pb.items[i];
                break;
            default:
                printf("Unknown operator: %c", pb.equation);
                exit(1);
        }
    }

    // printf("%llu\n", total);

    return total;
}

const char *delim = " \n";

int main() {
    char *buf = NULL;
    size_t cap = 0;
    FILE *file = fopen("real.txt", "r");
    Problems problems = {0};

    getline(&buf, &cap, file);

    char *tok = strtok(buf, delim);
    while (tok != NULL) {
        Problem pbl = {0};
        da_append(pbl, atoi(tok));
        da_append(problems, pbl);
        tok = strtok(NULL, delim);
    }


    while (getline(&buf, &cap, file) > 0) {
        int i = 0;
        tok = strtok(buf, delim);

        while (tok != NULL) {
            if (tok[0] == '*' || tok[0] == '+') {
                problems.items[i].equation = tok[0];            
            } else {
                da_append(problems.items[i], atoi(tok));
            }

            tok = strtok(NULL, delim);
            i += 1;
        }
    }

    uint64_t ans = 0;

    for (int i = 0; i < problems.count; i++) {
        ans += solve(problems.items[i]);
    }

    printf("Answer: %llu\n", ans);

    // for (size_t i = 0; i < problems.count; i++) {
    //     Problem pb = problems.items[i];

    //     printf("%d", pb.items[0]);
    //     for (size_t j = 1; j < pb.count; j++) {
    //         printf(" %c %d", pb.equation, pb.items[j]);
    //     }
    //     printf("\n");
    // }

    return EXIT_SUCCESS;
}
