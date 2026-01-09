#include <stdlib.h>
#include <stdio.h>

// Incorrect answers
// 6652
// 6625

#define MAX_NUM 100
// #define DEBUG
// #define STOP

typedef struct Safe {
    int pos;
} Safe;

void safe_init(Safe *s) {
    s->pos = 50;

#ifdef DEBUG
    printf("The dial starts by pointing at %d.\n", s->pos);
#endif
}

// Returns the number of times pass 0
int safe_rotate_left(Safe *s, int len) {
    int is_start_zero = s->pos == 0 ? 1 : 0;

    s->pos -= len;

    if (s->pos > 0)
        return 0;
    if (s->pos == 0)
        return 1;

    // is negative

    int ret = s->pos / -MAX_NUM;
    s->pos %= MAX_NUM;
    s->pos = (MAX_NUM + s->pos) % MAX_NUM;

    if (!is_start_zero) {
        ret += 1;
    }

    return ret;
}

// Returns the number of times pass 0
int safe_rotate_right(Safe *s, int len) {
    s->pos += len;
    int ret = s->pos / MAX_NUM;
    s->pos %= MAX_NUM;
    return ret;
}

// Returns the number of times pass 0, or -1 for error
int safe_rotate(Safe *s, char direction, int len) {
    int ret = -1;

    switch (direction) {
        case 'L':
            ret = safe_rotate_left(s, len);
            break;
        case 'R':
            ret = safe_rotate_right(s, len);
            break;
    }

#ifdef DEBUG
    printf("The dial is rotated %c%d to point at %d. ", direction, len, s->pos);
#endif

    return ret;
}

int main() {
    // Data structures
    Safe safe;
    char buf[16];

    // Input
    char direction;
    int len;

    int count = 0;
    FILE *input = fopen("input.txt", "r");

    safe_init(&safe);

    while (fgets(buf, sizeof(buf), input)) {

        if (sscanf(buf, "%c%d", &direction, &len) != 2) {
            printf("ERROR: mismatch for sscanf\n");
            return EXIT_FAILURE;
        }

        int c = safe_rotate(&safe, direction, len);

#ifdef DEBUG
        printf("Passed zero %d, times.\n", c);
#endif // DEBUG

        if (c == -1) {
            printf("ERROR: unable to rotate safe\n");
            return EXIT_FAILURE;
        }

        count += c;

#ifdef STOP
        while (getchar() != '\n') {}
#endif // STOP

    }

    if (ferror(input)) {
        perror("error reading input file");
        return EXIT_FAILURE;
    }

    if (feof(input)) {
        printf("Successfully parsed input\n");
    }

    printf("Found %d times at 0\n", count);

    return EXIT_SUCCESS;
}
