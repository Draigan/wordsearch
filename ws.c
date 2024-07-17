#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define GRID_ROWS 4
#define GRID_COLS 4
#define GRID_DEPTH 2
#define WORDS_LENGTH 7
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

// NOTE: if you are testing with different words, dont forget to change the
// "WORDS_LENGTH" macro. Also macros are amazing and I shun other devs for not
// including them in other languages

//  Words to be placed
/* char words[WORDS_LENGTH][MAX_WORD_LENGTH] = { */
/*     "FOOL", "CASTLING", "KING", "SWORD", "SHIELD", "KNIGHT", "DRAGON",
 * "FAIRY"}; */

/* char words[WORDS_LENGTH][MAX_WORD_LENGTH] = { */
/* "FOOL", "COOL", "TOOL", "POOL", "NOODLE", "MOON", "COON", "RACOON"}; */
/* char words[WORDS_LENGTH][MAX_WORD_LENGTH] = {"TOOL", "FOOL", "COOL", "LOL"};
 */
char words[WORDS_LENGTH][MAX_WORD_LENGTH] = {"AAAA", "BBBB", "CCCC", "DDDD"};

/* char words[WORDS_LENGTH][MAX_WORD_LENGTH] = {"COOL",  "FOOL",  "TOOL",
 * "MOOL", */
/*                                              "STOOL", "DROOL", "POOL"}; */

// Possible directions a word can move

// Get random number between 0 and max
int random_number(int max) { return rand() % max; }

// Print possible directions
void print_direct(possibilities *poss) {
  // For each element of the possibilities arrays
  printf("\n");
  for (int i = 0; i < POSS_LENGTH; i++) {
    // Stop when we reach the end of "directions"
    if (directions[i][0] == '\0') {
      return;
    }
    printf("row: %i, col: %i, direction: %s", poss->coordinates[i][0],
           poss->coordinates[i][1], directions[i]);
    printf("\n");
  }
}

void print_used() {
  for (int i = 0; i < WORDS_LENGTH; i++) {
    printf("%i ", used_words[i]);
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

/*
 * @Param1 - direction ("u", "d" "l", "r", "ul" "ur", "dl", "dr")
 * @Param2 - row to modify
 * @Param3 - col to modify
 * @return - array[0] is row & array[1] is col
 * */
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

// Recursive function to check if a word fits
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
 * @Param1 - word we want to place
 * @Param2 - index of the char (call it with 0)
 * @Param3 - row where the first letter of the word is placed
 * @Param3 - col as above
 * @Param4 - direction ("u", "d" "l", "r", "ul" "ur", "dl", "dr")
 * @Param5 - Grid to place it in
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

// Print the grid
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

// @return - random word that hasn't been used before
char *random_word(char words[WORDS_LENGTH][MAX_WORD_LENGTH],
                  int used_words[WORDS_LENGTH]) {
  int index;
  do {
    index = random_number(WORDS_LENGTH);
  } while (used_words[index]);
  /* used_words[index] = 1; */

  return words[index];
}

/*
 * @Param1 - array mapping used words 0 is unused 1 is used
 * @return - false if all words are not placed else true
 * */
bool is_all_placed(int used_words[WORDS_LENGTH]) {

  for (int i = 0; i < WORDS_LENGTH; i++) {
    if (used_words[i] == 0) {
      return false;
    }
  }
  return true;
}

void push_coords(possibilities *poss, int row, int col) {
  /* printf("Pushing to row: %i", row); */
  /* printf("Pushing to col: %i", col); */
  poss->coordinates[poss->length][0] = row;
  poss->coordinates[poss->length][1] = col;
}

void push_direc(char directions[GRID_ROWS * GRID_COLS][3], char *direction) {
  for (int i = 0; i < GRID_ROWS * GRID_COLS * 8; i++) {
    if (directions[i][0] == '\0') {
      /* printf("recieved: %s", direction); */
      strcpy(directions[i], direction);
      return;
    }
  }
}

possibilities get_all_possible(char *word) {

  possibilities possible = {0};

  // For each square
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      // There are 8 directions
      for (int k = 0; k < 8; k++) {
        // Check each direction if the word is possible
        if (check_space(word, directions[k], i, j, 0)) {
          // Add it to our output
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

bool any_possible() {
  int i = WORDS_LENGTH;
  do {
    i--;
    if (used_words[i] == 1) {
      continue;
    }
    if (get_all_possible(words[i]).length != 0) {
      return true;
    }

  } while (i);
  return false;
}

void reset_used_words() {
  int i = WORDS_LENGTH;
  do {
    i--;
    used_words[i] = 0;
  } while (i);
}

void shuffle_pos(int pos[GRID_ROWS * GRID_COLS][2]) {
  int i = GRID_ROWS * GRID_COLS;
  do {
    i--;
    int temp[2];
    int rand = random_number(GRID_ROWS * GRID_COLS);
    temp[0] = pos[rand][0];
    temp[1] = pos[rand][1];
    pos[rand][0] = pos[i][0];
    pos[rand][1] = pos[i][1];
    pos[i][0] = temp[0];
    pos[i][0] = temp[1];

  } while (i);
}

// Print the shuffled array
void print_pos(int pos[GRID_ROWS * GRID_COLS][2]) {
  for (int i = 0; i < GRID_ROWS * GRID_COLS; i++) {
    printf("pos[%d] = [%d, %d]\n", i, pos[i][0], pos[i][1]);
  }
}

bool solve(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH],
           char words[WORDS_LENGTH][MAX_WORD_LENGTH], int *used_words,
           int index) {
  print_grid(grid);
  print_used();
  if (is_all_placed(used_words)) {
    printf("Completed Wordsearch: \n");
    print_grid(grid);
    return true;
  }

  char *word = words[index];
  char *word_eraser = eraser(strlen(word));
  /*
    Array of all the different coordinates.
    We can easily shuffle this array for the backtracking.
  */
  int pos[GRID_ROWS * GRID_COLS][2];
  int pos_index = 0;
  for (int y = 0; y < GRID_ROWS; y++) {
    for (int x = 0; x < GRID_COLS; x++) {
      pos[pos_index][0] = y;
      pos[pos_index][1] = x;
      pos_index++;
    }
  }
  printf("Before Shuffle\n");
  print_pos(pos);
  shuffle_pos(pos);
  printf("After Shuffle\n");
  print_pos(pos);

  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      for (int k = 0; k < 8; k++) {
        if (check_space(word, directions[k], i, j, 0)) {
          place_word(words[index], 0, i, j, directions[k], grid);
          used_words[index] = 1;
          if (solve(grid, words, used_words, index + 1)) {
            return true;
          }
          place_word(word_eraser, 0, i, j, directions[k], grid);
          free(word_eraser);
          used_words[index] = 0;
        }
      }
    }
  }
  return false;
}

/* void solve(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH], */
/*            char words[WORDS_LENGTH][MAX_WORD_LENGTH], int *used_words) { */
/**/
/*   if (is_all_placed(used_words)) { */
/*     printf("Completed Wordsearch: \n"); */
/*     print_grid(grid); */
/*     return; */
/*   } */
/**/
/*   int row; */
/*   int col; */
/*   char *direction; */
/*   char *word = random_word(words, used_words); */
/*   // Index so we can mark the word as used or not on the used_words array */
/*   int word_index = find_word_index(word); */
/**/
/*   // Get a random word */
/**/
/*   // Char of the same length as word containing 0's */
/*   char *word_eraser = eraser(strlen(word)); */
/**/
/*   // Get all possible word placements */
/*   possibilities poss = get_all_possible(word); */
/**/
/*   // poss.directions and poss.coordinates corolate with eachother so */
/*   // directions[0] and coordinates[0][0] + coordinates[0][1] will give us
 * data */
/*   // like "d" 0 , 0 meaning at the position 0 , 0 the word can be placed */
/*   // downwards */
/**/
/*   // Get random possibility */
/*   int poss_position = random_number(poss.length); */
/*   printf("direction: %s", poss.directions[poss_position]); */
/*   direction = poss.directions[poss_position]; */
/*   row = poss.coordinates[poss_position][0]; */
/*   col = poss.coordinates[poss_position][1]; */
/**/
/*   // Place the word in a random possibilitiy */
/*   place_word(word, 0, row, col, direction, grid); */
/*   // Mark the word as used */
/*   used_words[word_index] = 1; */
/*   words_placed++; */
/**/
/*   print_grid(grid); */
/**/
/*   // If we reach a deadend, restart */
/*   if (!any_possible() && !is_all_placed(used_words)) { */
/*     printf("There is not any more possibilities in recurse \n"); */
/*     reset_used_words(); */
/*     initialize_grid(); */
/*   } */

// Recurse
/* solve(grid, words, used_words); */

// Backtrack - Refill with X's
/* place_word(word_eraser, 0, row, col, direction, grid); */
/* used_words[word_index] = 0; */
/* words_placed--; */
/* } */

int main() {
  srand(time(NULL));

  // Initialize grid with "0"
  initialize_grid();
  /* grid[0][0][0] = 'X'; */
  /* grid[1][1][0] = 'X'; */
  /* grid[2][2][0] = 'X'; */
  /* grid[3][3][0] = 'X'; */

  solve(grid, words, used_words, 0);
  /* possibilities poss = get_all_possible("CO"); */
  /* print_grid(grid); */
  /* printf("%i", poss.length); */
  /* if (any_possible()) { */
  /*   printf("There are possibilities"); */
  /* } else { */
  /*   printf("There are NOT possibilities"); */
  /* } */
  return 0;
}
