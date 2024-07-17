#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define GRID_ROWS 4
#define GRID_COLS 4
#define GRID_DEPTH 2
#define WORDS_LENGTH 4
#define MAX_WORD_LENGTH 10
typedef int bool;
#define true 1
#define false 0
#define POSS_LENGTH GRID_ROWS *GRID_COLS * 8
#define RESET "\033[0m"
#define RED "\033[31m"

typedef struct {
  int length;
  char directions[POSS_LENGTH][3];
  int coordinates[POSS_LENGTH][3];

} possibilities;

// Puzzle grid
char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH];
char *directions[] = {"u", "d", "l", "r", "ul", "ur", "dl", "dr"};

// Number of words placed
int words_placed = 0;

// Array to mark which words have been used
int used_words[WORDS_LENGTH] = {0};

char words[WORDS_LENGTH][MAX_WORD_LENGTH] = {"AAAA", "BBBB", "CCCC", "DDDD"};

// Get random number between 0 and max
int random_number(int max) { return rand() % max; }

// Print possible directions
void print_direct(possibilities *poss) {
  printf("\n");
  for (int i = 0; i < poss->length; i++) {
    printf("row: %i, col: %i, direction: %s", poss->coordinates[i][0],
           poss->coordinates[i][1], poss->directions[i]);
    printf("\n");
  }
}

// Make a GRID_ROWS * GRID_COLS grid of 'X'
void initialize_grid() {
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      strcpy(grid[i][j], "0");
    }
  }
}

int *get_direction(char *direction, int row, int col) {
  if (strcmp(direction, "u") == 0) {
    row -= 1;
  } else if (strcmp(direction, "d") == 0) {
    row += 1;
  } else if (strcmp(direction, "l") == 0) {
    col -= 1;
  } else if (strcmp(direction, "r") == 0) {
    col += 1;
  } else if (strcmp(direction, "ul") == 0) {
    row -= 1;
    col -= 1;
  } else if (strcmp(direction, "ur") == 0) {
    row -= 1;
    col += 1;
  } else if (strcmp(direction, "dl") == 0) {
    row += 1;
    col -= 1;
  } else if (strcmp(direction, "dr") == 0) {
    row += 1;
    col += 1;
  }

  int *directions = (int *)malloc(2 * sizeof(int));
  directions[0] = row;
  directions[1] = col;

  return directions;
}

bool check_space(char *word, char *direction, int row, int col, int n) {
  int length = strlen(word);
  char current_letter = word[n];

  if (n >= length) {
    return true;
  }

  if (row >= GRID_ROWS || col >= GRID_COLS || row < 0 || col < 0) {
    return false;
  }

  if (grid[row][col][0] != '0' && grid[row][col][0] != current_letter) {
    return false;
  }

  int *directions = get_direction(direction, row, col);
  row = directions[0];
  col = directions[1];
  free(directions);

  return check_space(word, direction, row, col, n + 1);
}

void place_word(char *word, int index, int row, int col, char *direction,
                char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH]) {

  if (index >= strlen(word)) {
    return;
  }

  if (index != 0) {
    int *directions = get_direction(direction, row, col);
    row = directions[0];
    col = directions[1];
    free(directions);
  }

  grid[row][col][0] = word[index];
  grid[row][col][1] = '\0';

  return place_word(word, index + 1, row, col, direction, grid);
}

void print_grid(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH]) {
  printf("\n\n\n\n");
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      if (grid[i][j][0] == '0') {
        printf("%s ", grid[i][j]);
      } else {
        printf(RED "%s " RESET, grid[i][j]);
      }
    }
    printf("\n");
  }
  printf("\n\n\n");
}

char *random_word(char words[WORDS_LENGTH][MAX_WORD_LENGTH],
                  int used_words[WORDS_LENGTH]) {
  int index;
  do {
    index = random_number(WORDS_LENGTH);
  } while (used_words[index]);
  return words[index];
}

bool is_all_placed(int used_words[WORDS_LENGTH]) {
  for (int i = 0; i < WORDS_LENGTH; i++) {
    if (used_words[i] == 0) {
      return false;
    }
  }
  return true;
}

void push_coords(possibilities *poss, int row, int col) {
  poss->coordinates[poss->length][0] = row;
  poss->coordinates[poss->length][1] = col;
}

void push_direc(char directions[GRID_ROWS * GRID_COLS][3], char *direction) {
  for (int i = 0; i < GRID_ROWS * GRID_COLS * 8; i++) {
    if (directions[i][0] == '\0') {
      strcpy(directions[i], direction);
      return;
    }
  }
}

possibilities get_all_possible(char *word) {
  possibilities possible = {0};

  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      for (int k = 0; k < 8; k++) {
        if (check_space(word, directions[k], i, j, 0)) {
          push_direc(possible.directions, directions[k]);
          push_coords(&possible, i, j);
          possible.length++;
        }
      }
    }
  }

  return possible;
}

char *eraser(int length) {
  char *eraser = (char *)malloc((length + 1) * sizeof(char));
  for (int i = 0; i < length; i++) {
    eraser[i] = 'X';
  }
  eraser[length] = '\0';
  return eraser;
}

int find_word_index(char *word) {
  int index;
  for (int i = 0; i < WORDS_LENGTH; i++) {
    if (strcmp(word, words[i]) == 0) {
      index = i;
    }
  }
  return index;
}

void solve(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH],
           char words[WORDS_LENGTH][MAX_WORD_LENGTH], int *used_words) {

  printf("Recursing\n");

  if (is_all_placed(used_words)) {
    print_grid(grid);
    return;
  }

  int row;
  int col;
  char *direction;
  char *word = random_word(words, used_words);
  int word_index = find_word_index(word);
  char *word_eraser = eraser(strlen(word));

  possibilities poss = get_all_possible(word);

  if (poss.length == 0) {
    int i = words_placed;
    while (i > 0) {
      i--;
      used_words[i] = 0;
    }
    solve(grid, words, used_words);
    return;
  }

  for (int poss_position = 0; poss_position < poss.length; poss_position++) {
    printf("Trying direction: %s\n", poss.directions[poss_position]);
    direction = poss.directions[poss_position];
    row = poss.coordinates[poss_position][0];
    col = poss.coordinates[poss_position][1];

    place_word(word, 0, row, col, direction, grid);
    used_words[word_index] = 1;
    words_placed++;

    solve(grid, words, used_words);

    place_word(word_eraser, 0, row, col, direction, grid);
    used_words[word_index] = 0;
    words_placed--;
  }

  free(word_eraser);
}

int main() {
  srand(time(NULL));

  initialize_grid();

  solve(grid, words, used_words);

  return 0;
}
