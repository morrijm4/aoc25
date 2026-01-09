#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct ListNode {
    uint64_t lhs;
    uint64_t rhs;
    struct ListNode *next;
} ListNode;

// Returns 0 for success, otherwise 1;
int parse_input(char *buf, int n, ListNode **head) {
    int i = 0;
    int j = 0;

    while (j < n) {
        if (buf[j] == ',' || buf[j] == '\n') {
            ListNode *node = (ListNode *)malloc(sizeof(ListNode));

            if (node == NULL) {
                perror("malloc p");
                return EXIT_FAILURE;
            }

            if (2 != sscanf(buf + i, "%llu-%llu", &node->lhs, &node->rhs)) {
                return EXIT_FAILURE;
            }

            node->next = *head;
            *head = node;

            i = j + 1;
        }

        j += 1;
    }

    return EXIT_SUCCESS;
}

int digits(uint64_t n) {
    int len = 1;

    while (n / (int)powf(10, len)) {
        len += 1;
    }

    return len;
}

int is_valid_puzzle1(uint64_t n) {
    int len = digits(n);

    if (len % 2 == 1) {
        return 1;
    }

    int mask = powf(10, (int)(len / 2));
    int lhs = n / mask;
    int rhs = n % mask;

    return lhs != rhs;
}

int repeats(char *num, int len, int window) {
    for (int i = window; i < len; i += window) {
        for (int j = 0; j < window; j++) {
            if (num[i + j] != num[j]) {
                return 0;
            }
        }
    }

    return 1;
}

int is_valid_puzzle2(uint64_t n) {
    int len = digits(n);
    char str[21];
    snprintf(str, sizeof(str), "%llu", n);

    int mid = len / 2;

    for (int i = 1; i <= mid; i++) {
        if (len % i == 0 && repeats(str, len, i)) {
            return 0;
        }
    } 

    return 1;
}

uint64_t solve(ListNode *head) {
    uint64_t ans = 0;

    while (head != NULL) {

        for (uint64_t i = head->lhs; i <= head->rhs; i++) {
            if (!is_valid_puzzle2(i)) {
                ans += i;
            }
        } 

        head = head->next;
    }

    return ans;
}

int puzzle() {
    char *buf = NULL;
    size_t buf_len = 0;
    FILE *f = fopen("real.txt", "r");

    if (f == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    int n = getline(&buf, &buf_len, f);

    if (n == -1) {
        perror("getline");
        return EXIT_FAILURE;
    }

    printf("Read %d bytes\n", n);

    ListNode *head = NULL;

    if (parse_input(buf, buf_len, &head)) {
        printf("ERROR: parsing input");
        return EXIT_FAILURE;
    }

    if (head == NULL) {
        printf("Did not parse anything\n");
        return EXIT_FAILURE;
    }

    printf("Answer: %llu\n", solve(head));

    ListNode *curr = head;
    while (curr != NULL) {
        ListNode *tmp = curr;
        curr = curr->next;
        free(tmp);
    }

    free(buf);
    if (EOF == fclose(f)) {
        perror("fclose");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main() {
    return puzzle();
}
