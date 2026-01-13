#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// incorrect answers
// 231560780 (low)

typedef struct {
    uint64_t lhs;
    uint64_t rhs;
} Range;

typedef struct {
    Range *items;
    size_t count;
    size_t capacity;
} Ranges;

typedef struct RangeNode {
    Range item;
    struct RangeNode *next;
} RangeNode;

void list_free(RangeNode *node) {
    RangeNode *tmp;
    while (node != NULL) {
        tmp = node;
        node = node->next;
        free(tmp);
    }
}

RangeNode *list_push(RangeNode *head, Range r) {
    RangeNode *node = (RangeNode *)malloc(sizeof(RangeNode));

    if (node == NULL)
        return NULL;

    node->item = r;
    node->next = head;
    return node;
}

#define TRUE 1
#define FALSE 0

int is_intersection(Range a, Range b) {
    // A: -------
    // B:     =========

    // A:            -------
    // B:     =========

    // A:       -----
    // B:     =========

    // A:     -------
    // B:      =====




    // A:     -------
    // B:             =====

    // A:       -------
    // B: =====

    // is not intersection
    if ((a.rhs + 1) < b.lhs || (b.rhs + 1) < a.lhs)
        return FALSE;

    return TRUE;
}

Range combine(Range a, Range b) {
    Range c;
    c.lhs = a.lhs < b.lhs ? a.lhs : b.lhs;
    c.rhs = a.rhs > b.rhs ? a.rhs : b.rhs;
    return c;
}

int da_append(Ranges *arr, Range item) {
    if (arr->capacity <= arr->count) {
        if (arr->capacity == 0) arr->capacity = 256;
        else arr->capacity *= 2;

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
    FILE *file = fopen("real.txt", "r");

    RangeNode *head = NULL;

    // Parse ranges
    while (getline(&buf, &cap, file) > 0) {
        if (buf[0] == '\n')
            break;

        Range range;

        if (2 != sscanf(buf, "%llu-%llu\n", &range.lhs, &range.rhs)) {
            printf("Failed to parse ranges\n");
            return EXIT_FAILURE;
        };

        head = list_push(head, range);
    }

    int count = 0;

    RangeNode *prev = NULL;
reset:;
    for (RangeNode *i = head; i != NULL; i = i->next) {
        for (RangeNode *j = i; j != NULL; j = j->next) {
            if (i == j) prev = i;
            else {
                if (is_intersection(i->item, j->item)) {
                    i->item = combine(i->item, j->item);

                    // Remove j
                    prev->next = j->next;
                    RangeNode *tmp = j;
                    j = prev;
                    free(tmp);

                    goto reset;
                }
                prev = j;
            }
        }
    }

    // Parse IDs for puzzle 1
    // while (getline(&buf, &cap, file) > 0) {
    //     uint64_t ID;

    //     if (1 != sscanf(buf, "%llu\n", &ID)) {
    //         printf("Failed to parse IDs\n");
    //         return EXIT_FAILURE;
    //     };

    //     RangeNode *curr = head;
    //     while (curr != NULL) {
    //         Range r = curr->item;

    //         if (ID >= r.lhs && ID <= r.rhs) {
    //             count++;
    //             break;
    //         }

    //         curr = curr->next;
    //     }
    // }

    // printf("Number of fresh: %d\n", count);

    // Print ranges
    RangeNode *curr = head;
    while (curr != NULL) {
        Range rng = curr->item;
        printf("[%llu, %llu]\n", rng.lhs, rng.rhs);
        curr = curr->next;
    }

    uint64_t total = 0;

    // Count ranges
    for (RangeNode *curr = head; curr != NULL; curr = curr->next) {
        total += (curr->item.rhs - curr->item.lhs) + 1;
    }

    printf("Total: %llu\n", total);

    if (buf != NULL)
        free(buf);

    list_free(head);
    fclose(file);

    return EXIT_SUCCESS;
}
