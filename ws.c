#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define GRID_ROWS 12
#define GRID_COLS 12
#define GRID_DEPTH 2
#define WORDS_LENGTH 9
#define MAX_WORD_LENGTH 10
#define POSS_LENGTH GRID_ROWS *GRID_COLS * 8
#define RANDOM_NUMBER(max) (rand() % (max))
#define RESET "\033[0m"
#define RED "\033[31m"

// Puzzle grid
char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH];

// Possible directions
char *directions[] = {"dl", "ur", "l", "r", "ul", "d", "u", "dr"};

// Array to mark which words have been used
int used_words[WORDS_LENGTH] = {0};

// Words to be placed
char words[WORDS_LENGTH][MAX_WORD_LENGTH] = {"FOOL",     "CASTLING", "KING",

                                             "ATTACKER", "SWORD",    "SHIELD",

                                             "KNIGHT",   "DRAGON",   "FAIRY"};

// char words[WORDS_LENGTH][MAX_WORD_LENGTH] = {"AAAA", "BBBB", "CCCC", "DDDD"};

/*
 * For debugging, just prints the used_words array
 * */
void print_used() {
  for (int i = 0; i < WORDS_LENGTH; i++) {
    printf("%i", used_words[i]);
  }
}

/*
 * Initialize the grid with '0's
 * */
void initialize_grid() {
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      strcpy(grid[i][j], "0");
    }
  }
}

/*
 * Returns a the modified coordinates for a direction
 * */
int *get_direction(char *direction, int row, int col) {
  if (strcmp(direction, "u") == 0) {
    row -= 1;
  }
  if (strcmp(direction, "d") == 0) {
    row += 1;
  }
  if (strcmp(direction, "l") == 0) {
    col -= 1;
  }
  if (strcmp(direction, "r") == 0) {
    col += 1;
  }
  if (strcmp(direction, "ul") == 0) {
    row -= 1;
    col -= 1;
  }
  if (strcmp(direction, "ur") == 0) {
    row -= 1;
    col += 1;
  }
  if (strcmp(direction, "dl") == 0) {
    row += 1;
    col -= 1;
  }
  if (strcmp(direction, "dr") == 0) {
    row += 1;
    col += 1;
  }

  int *directions = (int *)malloc(2 * sizeof(int));
  if (directions == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }
  directions[0] = row;
  directions[1] = col;

  return directions;
}

/*
 * Check if a word fits
 * */
bool check_space(char *word, char *direction, int row, int col, int n) {
  int length = strlen(word);
  char current_letter = word[n];

  // Reached the end of the word
  if (n >= length) {
    /* printf("There is space for: %s at n: %i \n", word, n); */
    return true;
  }

  // Out of bounds
  if (row >= GRID_ROWS || col >= GRID_COLS || row < 0 || col < 0) {
    /* printf("Out of bounds at n: %i \n", n); */
    return false;
  }

  // Encountered letter of different type
  if (grid[row][col][0] != '0' && grid[row][col][0] != current_letter) {
    /* printf("encountered letter of different type\n"); */
    return false;
  }

  // Adjust col/row based on direction
  int *directions = get_direction(direction, row, col);
  row = directions[0];
  col = directions[1];
  free(directions);

  return check_space(word, direction, row, col, n + 1);
}

/*
 * Place the word
 * */
void place_word(char *word, int index, int row, int col, char *direction,
                char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH]) {

  // Placed all letters of the word on the grid
  if (index >= strlen(word)) {
    return;
  }

  // Adjust coords +1 or -1 to row / col
  if (index != 0) {
    int *directions = get_direction(direction, row, col);
    row = directions[0];
    col = directions[1];
    free(directions);
  }

  // Add the letter to grid
  grid[row][col][0] = word[index];
  grid[row][col][1] = '\0';
  /* strncpy(grid[row][col], &word[index], 1); */
  // Move on to the next letter
  return place_word(word, index + 1, row, col, direction, grid);
}

/*
 * Print the grid
 * */
void print_grid(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH]) {
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");

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
  printf("\n");
  printf("\n");
  printf("\n");
}

/*
 * Get a word of fixed length containg all '0's
 * */
char *eraser(int length) {
  char *eraser = (char *)malloc((length + 1) * sizeof(char));
  for (int i = 0; i < length; i++) {
    eraser[i] = '0';
  }
  eraser[length] = '\0';
  return eraser;
}

/*
 * Shuffle the possibile positions array
 * This is an array containing all the coordinates
 * in sub arrays with row at [0] and col at [1]
 * */
void shuffle_pos(int pos[GRID_ROWS * GRID_COLS][2]) {
  int i = GRID_ROWS * GRID_COLS;
  do {
    i--;
    int temp[2];
    int rand_index = RANDOM_NUMBER(GRID_ROWS * GRID_COLS);
    temp[0] = pos[rand_index][0];
    temp[1] = pos[rand_index][1];
    pos[rand_index][0] = pos[i][0];
    pos[rand_index][1] = pos[i][1];
    pos[i][0] = temp[0];
    pos[i][0] = temp[1];

  } while (i);
}

/*
 * Generate the wordsearch
 * */
bool solve(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH],
           char words[WORDS_LENGTH][MAX_WORD_LENGTH], int *used_words,
           int index) {
  print_grid(grid);

  /* Gaurd clause for recursion */
  if (index >= WORDS_LENGTH) {
    printf("Completed Wordsearch: \n");
    print_grid(grid);
    return true;
  }
  int row;
  int col;
  char *word = words[index];

  /* For erasing the words with '0's */
  char *word_eraser = eraser(strlen(word));

  /*
   * Array of arrays containing all the
   * different coordinates. We can easily
   * shuffle this array.
   **/
  int pos[GRID_ROWS * GRID_COLS][2];
  int pos_length = GRID_ROWS * GRID_COLS;
  int pos_index = 0;
  for (int y = 0; y < GRID_ROWS; y++) {
    for (int x = 0; x < GRID_COLS; x++) {
      pos[pos_index][0] = y;
      pos[pos_index][1] = x;
      pos_index++;
    }
  }
  /* Shuffle possibilities */
  shuffle_pos(pos);

  /* Main backtracking algo */
  int i = pos_length;
  do {
    i--;
    row = pos[i][0];
    col = pos[i][1];
    for (int k = 0; k < 8; k++) {
      if (check_space(word, directions[k], row, col, 0)) {
        place_word(words[index], 0, row, col, directions[k], grid);
        used_words[index] = 1;
        if (solve(grid, words, used_words, index + 1)) {
          return true;
        }
        place_word(word_eraser, 0, row, col, directions[k], grid);
        used_words[index] = 0;
      }
    }
  } while (i);

  free(word_eraser);
  return false;
}

int main() {

  // Seed srand
  srand(time(NULL));

  // Initialize grid with "0"
  initialize_grid();

  // Generate the wordsearch
  solve(grid, words, used_words, 0);
  return 0;
}
