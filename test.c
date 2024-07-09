#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define GRID_ROWS 10
#define GRID_COLS 10
#define GRID_DEPTH 2

#define WORDS_LENGTH 5
#define MAX_WORD_LENGTH 10

typedef int bool;
#define true 1
#define false 0

// Get a random point on the grid
int *random_coordinates() {
  int *coordinates = (int *)malloc(2 * sizeof(int));
  coordinates[0] = rand() % GRID_ROWS;
  coordinates[1] = rand() % GRID_COLS;
  return coordinates;
}

// Movement directions for grid
void get_direction(char *direction, int *row, int *col) {
  if (strcmp(direction, "u") == 0) {
    (*row)--;
  } else if (strcmp(direction, "d") == 0) {
    (*row)++;
  } else if (strcmp(direction, "l") == 0) {
    (*col)--;
  } else if (strcmp(direction, "r") == 0) {
    (*col)++;
  } else if (strcmp(direction, "ul") == 0) {
    (*row)--;
    (*col)--;
  } else if (strcmp(direction, "ur") == 0) {
    (*row)--;
    (*col)++;
  } else if (strcmp(direction, "dl") == 0) {
    (*row)++;
    (*col)--;
  } else if (strcmp(direction, "dr") == 0) {
    (*row)++;
    (*col)++;
  }
}

void print_grid(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH]) {
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      printf("%c ", grid[i][j][0]);
    }
    printf("\n");
  }
}
bool check_space(char *word, char *direction, int row, int col,
                 char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH], int n) {
  int length = strlen(word);
  char current_letter = word[n];

  // When to stop recursing
  if (row >= GRID_ROWS || col >= GRID_COLS || row < 0 || col < 0) {
    printf("Reached the end of the grid\n");
    printf("Cannot fit word\n");
    return false;
  }

  if (grid[row][col][0] != 'X' && grid[row][col][0] != current_letter) {
    printf("Encountered a letter\n");
    printf("Cannot fit word\n");
    return false;
  }

  printf("Checking position [%d][%d], grid contains: %c, current letter: %c\n",
         row, col, grid[row][col][0], current_letter);

  if (n >= length) {
    printf("There is space for: %s\n", word);
    return true;
  }

  if (grid[row][col][0] == current_letter) {
    printf("Encountered current letter\n");
  }

  // Adjust col/row based on direction
  get_direction(direction, &row, &col);

  return check_space(word, direction, row, col, grid, n + 1);
}

void place_word(char *word, int index, int row, int col, char *direction,
                char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH]) {
  // Reached the end of the word
  if (index >= strlen(word)) {
    return;
  }

  // Add the letter to grid
  grid[row][col][0] = word[index];
  grid[row][col][1] = '\0';

  // Adjust col/row based on direction
  get_direction(direction, &row, &col);

  place_word(word, index + 1, row, col, direction, grid);
}

char *random_word(char words[WORDS_LENGTH][MAX_WORD_LENGTH], int words_length) {
  return words[rand() % words_length];
}

void solve(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH],
           char words[WORDS_LENGTH][MAX_WORD_LENGTH], int placed_words) {
  // If all words are placed, we are done
  if (placed_words >= WORDS_LENGTH) {
    printf("Solution:\n");
    print_grid(grid);
    return;
  }

  char *directions[] = {"u", "d", "l", "r", "ul", "ur", "dl", "dr"};

  // Get random coordinates for placing the word
  int *coordinates = random_coordinates();
  int row = coordinates[0];
  int col = coordinates[1];
  free(coordinates);

  char *word = random_word(words, WORDS_LENGTH);

  // Try placing the word in each direction
  for (int i = 0; i < sizeof(directions) / sizeof(directions[0]); i++) {
    if (check_space(word, directions[i], row, col, grid, 0)) {
      place_word(word, 0, row, col, directions[i], grid);
      solve(grid, words, placed_words + 1);
      return; // Stop after finding a valid placement
    }
  }

  // If no valid placement found, retry with a new random word and coordinates
  solve(grid, words, placed_words);
}

int main() {
  srand(time(NULL));

  char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH];
  char words[WORDS_LENGTH][MAX_WORD_LENGTH] = {"beat", "atttic", "ids", "illn",
                                               "ego"};

  // Initialize grid with "X"
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      grid[i][j][0] = 'X';
      grid[i][j][1] = '\0';
    }
  }

  solve(grid, words, 0);

  return 0;
}
