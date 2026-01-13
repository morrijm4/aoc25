#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char *items;
    size_t count;
    size_t capacity;
} String;

String str_init(char *ptr, size_t count, size_t capacity) {
    String s;
    s.items = ptr;
    s.count = count;
    s.capacity = capacity;
    return s;
}

void str_pop(String *s) {
    s->count--;
}

typedef struct {
    String *items;
    size_t count;
    size_t capacity;
} Lines;

int da_append(Lines *arr, String item) {
    if (arr->capacity <= arr->count) {
        if (arr->capacity == 0) 
            arr->capacity = 256;
        else 
            arr->capacity *= 2;

        arr->items = realloc(arr->items, arr->capacity * sizeof(*arr->items));

        if (arr->items == NULL)
            return EXIT_FAILURE;
    }

    arr->items[arr->count++] = item;

    return EXIT_SUCCESS;
}

int main() {
    char *buf = NULL;
    size_t cap = 0;
    FILE *file = fopen("smol.txt", "r");

    Lines lines = {0};
    int n;

    while ((n = getline(&buf, &cap, file)) > 0) {
        String str = str_init(buf, n, cap);
        str_pop(&str);

        if (da_append(&lines, str)) {
            perror("realloc");
            return EXIT_FAILURE;
        }

        buf = NULL;
        cap = 0;
    }

    int res = 0;
    int run;

    do {
        run = 0;

        for (size_t i = 0; i < lines.count; i++) {
            for (size_t j = 0; j < lines.items[i].count; j++) {
                if (lines.items[i].items[j] == '.')
                    continue;

                int count = 0;

                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx == 0 && dy == 0)
                            continue;

                        int x = j + dx;
                        int y = i + dy;

                        if (x < 0 || x >= (int)lines.items[i].count)
                            continue;
                        if (y < 0 || y >= (int)lines.count)
                            continue;

                        if (lines.items[y].items[x] == '@' || lines.items[y].items[x] == 'x') {
                            count += 1;
                        }
                    }
                }

                if (count < 4) {
                    run += 1;
                    lines.items[i].items[j] = '.';
                }
            }
        }

        res += run;
    } while (run != 0);

    // for (size_t i = 0; i < lines.count; i++) {
    //     for (size_t j = 0; j < lines.items[i].count; j++) {
    //         printf("%c", lines.items[i].items[j]);
    //     }
    //     printf("\n");
    // }

    printf("Total count: %d\n", res);

    return EXIT_SUCCESS;
}
