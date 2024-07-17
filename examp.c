#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 3

void print_used(bool used[MAX_SIZE]) {
    printf("Used array: [");
    for (int i = 0; i < MAX_SIZE; ++i) {
        printf("%s", used[i] ? "T" : "F");
        if (i < MAX_SIZE - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

void print_result(int result[MAX_SIZE]) {
    printf("Current result: [");
    for (int i = 0; i < MAX_SIZE; ++i) {
        printf("%d", result[i]);
        if (i < MAX_SIZE - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

void backtrack(int nums[MAX_SIZE], int result[MAX_SIZE], bool used[MAX_SIZE], int depth) {
    // Base case: if depth reaches the length of the array, print the permutation
    if (depth == MAX_SIZE) {
        printf("Found permutation: ");
        for (int i = 0; i < MAX_SIZE; ++i) {
            printf("%d ", result[i]);
        }
        printf("\n");
        return;
    }

    // Recursive case: try each unused number at the current depth
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (!used[i]) {
            // Mark as used and add to the result
            used[i] = true;
            result[depth] = nums[i];

            printf("Trying %d at depth %d\n", nums[i], depth);
            print_used(used);
            print_result(result);

            // Recursively call backtrack for the next depth
            backtrack(nums, result, used, depth + 1);

            printf("Backtracking from %d at depth %d\n", nums[i], depth);
            // Backtrack: unmark and remove from the result
            used[i] = false;
            result[depth] = 0; // optional in this case since we are printing directly

            print_used(used);
            print_result(result);
        }
    }
}

int main() {
    int nums[MAX_SIZE] = {1, 2, 3};
    int result[MAX_SIZE];
    bool used[MAX_SIZE] = {false};

    printf("Finding all permutations of: ");
    for (int i = 0; i < MAX_SIZE; ++i) {
        printf("%d ", nums[i]);
    }
    printf("\n\n");

    printf("Starting backtrack:\n");
    backtrack(nums, result, used, 0);

    return 0;
}
