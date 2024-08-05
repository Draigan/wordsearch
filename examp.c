#include <stdbool.h>
#include <stdio.h>

#define MAX_SIZE 3

void printUsed(bool used[MAX_SIZE]) {
  printf("Used array: [");
  for (int i = 0; i < MAX_SIZE; ++i) {
    printf("%s", used[i] ? "T" : "F");
    if (i < MAX_SIZE - 1) {
      printf(", ");
    }
  }
  printf("]\n");
}

void printResult(int result[MAX_SIZE]) {
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
  if (depth == 0) {
    printf("Starting backtrack:\n");
  }

  if (depth == MAX_SIZE) {
    printf("Found permutation: ");
    for (int i = 0; i < MAX_SIZE; ++i) {
      printf("%d ", result[i]);
    }
    printf("\n");
    return;
  }

  for (int i = 0; i < MAX_SIZE; ++i) {
    if (!used[i]) {
      used[i] = true;
      result[depth] = nums[i];

      printf("Trying %d at depth %d\n", nums[i], depth);
      printUsed(used);
      printResult(result);

      backtrack(nums, result, used, depth + 1);

      printf("Backtracking from %d at depth %d\n", nums[i], depth);
      used[i] = false;
      result[depth] = 0;

      printUsed(used);
      printResult(result);
    }
  }
}

int main() {
  int nums[MAX_SIZE] = {1, 2, 3};
  int result[MAX_SIZE] = {0};
  bool used[MAX_SIZE] = {false};

  backtrack(nums, result, used, 0);

  return 0;
}
