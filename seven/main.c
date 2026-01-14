#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

void da_append(Lines *arr, String item) {
    if (arr->capacity <= arr->count) {
        if (arr->capacity == 0) arr->capacity = 256;
        else arr->capacity *= 2;
        arr->items = realloc(arr->items, arr->capacity * sizeof(*arr->items));
    }
    arr->items[arr->count++] = item;
}

uint64_t find_num_breaks(Lines lines, int x, int y, uint64_t **cache) {
    if (cache[x][y])
        return cache[x][y];

    y++;

    assert(lines.count >= y);

    if (lines.count == y) {
        return 1;
    }

    uint64_t acc = 0;
    if (lines.items[y].items[x] == '^') {
        if ((x - 1) >= 0)
            acc += find_num_breaks(lines, x - 1, y, cache);
        if ((x + 1) < lines.items[y].count)
            acc += find_num_breaks(lines, x + 1, y, cache);
    } else {
        lines.items[y].items[x] = '|';
        acc = find_num_breaks(lines, x, y, cache);
    }

    cache[x][y] = acc;
    return acc;
}

int main() {
    char *buf = NULL;
    size_t cap = 0;
    ssize_t len;
    FILE *file = fopen("real.txt", "r");

    Lines lines = {0};
    String line = {0};
    while ((len = getline(&buf, &cap, file)) > 0) {
        line.items = buf;
        line.count = len;
        line.capacity = cap;
        da_append(&lines, line);
        buf = NULL;
        cap = 0;
    }

    size_t x = 0;
    while (x < lines.items[0].count) {
        if (lines.items[0].items[x] == 'S') {
            break;
        }
        x++;
    }

    int w = lines.items[0].count;
    uint64_t **cache = (uint64_t **)malloc(w * sizeof(uint64_t *));
    for (uint64_t i = 0; i < w; i++) {
        cache[i] = (uint64_t *)calloc(lines.count, sizeof(uint64_t));
    }

    printf("Answer %llu\n", find_num_breaks(lines, x, 0, cache));

    // for (size_t i = 0; i < lines.count; i++) {
    //     String line = lines.items[i];
    //     printf("%.*s", (int)line.count, line.items);
    // }

    return EXIT_SUCCESS;
}

