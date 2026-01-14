#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef struct {
    char *items;
    size_t count;
    size_t capacity;
} String;

typedef struct {
    String *items;
    size_t count;
    size_t capacity;
} Lines;

typedef struct {
    String *items;
    size_t count;
    size_t capacity;
    char equation;
} NumberList;

typedef struct {
    NumberList *items;
    size_t count;
    size_t capacity;
} ProblemList;

#define da_append(arr, item)\
    do {\
        if (arr.capacity <= arr.count) {\
            if (arr.capacity == 0) arr.capacity = 8;\
            else arr.capacity *= 2;\
            arr.items = realloc(arr.items, arr.capacity * sizeof(*arr.items));\
        }\
        arr.items[arr.count++] = item;\
    } while (0)

uint64_t str_to_u64(String str) {
    char *tmp = strndup(str.items, str.count);
    uint64_t val = strtoull(tmp, NULL, 10);
    free(tmp);
    return val;
}

uint64_t solve(NumberList nums) {
    uint64_t res = str_to_u64(nums.items[0]);

    printf("%llu", res);

    for (int i = 1; i < nums.count; i++) {
        uint64_t num = str_to_u64(nums.items[i]);

        printf(" %c %llu", nums.equation, num);

        switch (nums.equation) {
            case '+':
                res += num;
                break;
            case '*':
                res *= num;
                break;
        }
    }

    printf("\n");

    // printf("%llu\n", res);

    return res;
}

int is_whitespace(String str) {
    for (size_t i = 0; i < str.count; i++) {
        if (!isspace(str.items[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

const char *delim = " \n";

int main() {
    char *buf = NULL;
    size_t cap = 0;
    FILE *file = fopen("real.txt", "r");
    ssize_t len;
    Lines lines = {0};

    while ((len = getline(&buf, &cap, file)) > 0) {
        String line = {
            .items = buf,
            .count = len,
            .capacity = cap,
        };

        da_append(lines, line);

        buf = NULL;
        cap = 0;
    }

    ProblemList problems = {0};
    NumberList numbers = {0};

    // Parse numbers
    for (size_t i = 0; i < lines.items[0].count; i++) {
        String num = {0};

        for (size_t j = 0; j < lines.count - 1; j++) {
            da_append(num, lines.items[j].items[i]);
        }

        if (is_whitespace(num)) {
            da_append(problems, numbers);
            memset(&numbers, 0, sizeof(NumberList));
        } else {
            da_append(numbers, num);
        }
    }

    // Parse equations
    char *tok = strtok(lines.items[lines.count - 1].items, delim);

    int i = 0;
    while (tok != NULL) {
        problems.items[i].equation = tok[0];
        i += 1;
        tok = strtok(NULL, delim);
    }

    uint64_t ans = 0;

    for (size_t i = 0; i < problems.count; i++) {
        NumberList numbers = problems.items[i];
        uint64_t res = str_to_u64(numbers.items[0]);

        // printf("%llu", res);
        for (size_t j = 1; j < numbers.count; j++) {
            uint64_t val = str_to_u64(numbers.items[j]);

            // printf(" %c %llu", numbers.equation, val);

            switch (numbers.equation) {
                case '+':
                    res += val;
                    break;
                case '*':
                    res *= val;
                    break;
            }
        }
        
        // printf(" = %llu\n", res);
        ans += res;
    }

    printf("Answer: %llu\n", ans);

    // Clean up
    for (size_t i = 0; i < lines.count; i++) {
        String str = lines.items[i];
        free(str.items);
    }
    free(lines.items);
    fclose(file);

    return EXIT_SUCCESS;
}
