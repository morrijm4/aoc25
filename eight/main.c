#include <math.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Incorrect
// 10080 (low)
// 3150 (low)

#define PAIRS 1000

typedef struct Point {
    int x; 
    int y; 
    int z; 
} Point;

typedef struct {
    Point *items;
    size_t count;
    size_t capacity;
} PointList;

typedef struct {
    PointList *items;
    size_t count;
    size_t capacity;
} CircuitList;

typedef struct {
    double dist;
    Point a;
    Point b;
} Distance;

const char *delim = ",\n";

int compare_point_list(const void *a, const void *b) {
    PointList lhs = *(const PointList *)a;
    PointList rhs = *(const PointList *)b;

    if (lhs.count < rhs.count) return 1;
    if (lhs.count > rhs.count) return -1;

    return 0;
}

int compare_distance(const void *a, const void *b) {
    Distance lhs = *(const Distance *)a;
    Distance rhs = *(const Distance *)b;

    if (lhs.dist < rhs.dist) return -1;
    if (lhs.dist > rhs.dist) return 1;

    return 0;
}

int eq(Point a, Point b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool in(Point p, PointList ls) {
    for (size_t i = 0; i < ls.count; i++) {
        if (eq(ls.items[i], p)) {
            return true;
        }
    }
    return false;
}


#define da_append(ls, item)\
    do {\
        if (ls.capacity <= ls.count) {\
            if (ls.capacity == 0) ls.capacity = 256;\
            else ls.capacity *= 2;\
            ls.items = realloc(ls.items, ls.capacity * sizeof(*ls.items));\
        }\
        ls.items[ls.count++] = item;\
    } while (0)

double square(double x) {
    return pow(x, 2);
}

double dist(Point a, Point b) {
    double x = square(abs(a.x - b.x));
    double y = square(abs(a.y - b.y));
    double z = square(abs(a.z - b.z));

    return sqrt(x + y + z);
}

void print_point(Point p) {
    printf("(%u, %u, %u)", p.x, p.y, p.z);
}

int run() {
    char *buf = NULL;
    size_t cap = 0;
    ssize_t len;
    FILE *file = fopen("real.txt", "r");

    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    PointList points = {0};

    while ((len = getline(&buf, &cap, file)) > 0) {
        Point pt;
        if (3 != sscanf(buf, "%u,%u,%u\n", &pt.x, &pt.y, &pt.z)) {
            printf("Error parseing input\n");
            return EXIT_FAILURE;
        }
        da_append(points, pt);
    }

    int n = points.count * points.count;
    Distance *distances = (Distance *)calloc(n, sizeof(Distance));

    if (distances == NULL) {
        perror("calloc");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < points.count; i++) {
        for (int j = i; j < points.count; j++) {
            if (i == j) continue;

            Point a = points.items[i];
            Point b = points.items[j];

            distances[i * points.count + j] = (Distance){
                .dist = dist(a, b),
                .a = a,
                .b = b,
            };
        }
    }

    qsort(distances, n, sizeof(*distances), compare_distance);

    size_t i = 0;
    while (i < n && distances[i].dist == 0) {
        i++;
    }

    CircuitList circuts = {0};

    // Print pairs
    for (size_t j = 0; j < PAIRS; j++) {
        Point a = distances[i + j].a;
        Point b = distances[i + j].b;
        printf("%.2f ", distances[i + j].dist);
        print_point(a);
        print_point(b);
        printf("\n");
    }
    printf("\n");
    printf("\n");
    printf("\n");


    // Create circuts
    for (size_t j = 0; j < PAIRS; j++) {
        Point a = distances[i + j].a;
        Point b = distances[i + j].b;

        bool is_a_in_circut = false;
        bool is_b_in_circut = false;

        int circuit_a = -1;
        int circuit_b = -1;

        for (int k = 0; k < circuts.count; k++) {
            if (in(a, circuts.items[k])) {
                is_a_in_circut = true;
                circuit_a = k;
            }

            if (in(b, circuts.items[k])) {
                is_b_in_circut = true;
                circuit_b = k;
            }

            if (is_a_in_circut && is_b_in_circut)
                break;
        }



        if (circuit_a >= 0 && circuit_b >= 0 && circuit_a == circuit_b) {
            // do nothing
        } else if (is_a_in_circut && is_b_in_circut) {
            // Combine circuts
            PointList plb = circuts.items[circuit_b];

            for (int i = 0; i < plb.count; i++) {
                da_append(circuts.items[circuit_a], plb.items[i]);
            }

            free(plb.items);
            memset(circuts.items + circuit_b, 0, sizeof(PointList));
        } else if (is_a_in_circut) {
            // Add a to circut
            assert(circuit_a >= 0);
            da_append(circuts.items[circuit_a], b);
        } else if (is_b_in_circut) {
            // Add b to circut
            assert(circuit_b >= 0);
            da_append(circuts.items[circuit_b], a);
        } else {
            // Add new circut
            PointList ls = {0};
            da_append(ls, a);
            da_append(ls, b);
            da_append(circuts, ls);
        }

        for (int i = 0; i < circuts.count; i++) {
            for (int j = 0; j < circuts.items[i].count; j++) {
                print_point(circuts.items[i].items[j]);
            }
            printf("\n");
        }
        printf("\n\n");
    }

    qsort(circuts.items, circuts.count, sizeof(*circuts.items), compare_point_list);

    for (int i = 0; i < circuts.count; i++) {
        for (int j = 0; j < circuts.items[i].count; j++) {
            print_point(circuts.items[i].items[j]);
        }
        printf("\n");
    }

    size_t a = circuts.items[0].count;
    size_t b = circuts.items[1].count;
    size_t c = circuts.items[2].count;

    printf("\n");
    printf("%lu\n", a);
    printf("%lu\n", b);
    printf("%lu\n", c);
    printf("Answer %lu\n", a * b * c);

    // Clean up
    free(points.items);
    free(distances);
    fclose(file);
    return EXIT_SUCCESS;
}

int main() {
    return run();
}
