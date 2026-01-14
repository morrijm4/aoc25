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

int find_num_breaks(Lines lines, int x, int y, bool **seen) {
    if (seen[x][y]) 
        return 0;

    seen[x][y] = true;
    y++;

    assert(lines.count >= y);

    if (lines.count == y) {
        return 0;
    }

    if (lines.items[y].items[x] == '^') {
        int acc = 1;

        if ((x - 1) >= 0)
            acc += find_num_breaks(lines, x - 1, y, seen);
        if ((x + 1) < lines.items[y].count)
            acc += find_num_breaks(lines, x + 1, y, seen);

        return acc;
    } else {
        lines.items[y].items[x] = '|';
        return find_num_breaks(lines, x, y, seen);
    }
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
    bool **seen = (bool **)malloc(w * sizeof(bool *));
    for (int i = 0; i < w; i++) {
        seen[i] = (bool *)calloc(lines.count, sizeof(bool));
    }


    printf("Answer %d\n", find_num_breaks(lines, x, 0, seen));

    for (size_t i = 0; i < lines.count; i++) {
        String line = lines.items[i];
        printf("%.*s", (int)line.count, line.items);
    }

    return EXIT_SUCCESS;
}

