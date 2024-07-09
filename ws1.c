#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define GRID_ROWS 15
#define GRID_COLS 15
#define GRID_DEPTH 2
#define WORDS_LENGTH 8
#define MAX_WORD_LENGTH 10

typedef int bool;
#define true 1
#define false 0

typedef struct {
  int length;
  char directions[GRID_ROWS * GRID_COLS * 8][3];
  int coordinates[GRID_ROWS * GRID_COLS][2];

} possibilities;

// Puzzle grid
char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH];
// Array to mark which words have been used
int used_words[WORDS_LENGTH] = {0};
// Words to be placed
char words[WORDS_LENGTH][MAX_WORD_LENGTH] = {
    "AAA", "BBB", "CCC", "DDDD", "EEEEEE", "FFFFFFF", "GGGGGGGG", "HHHHHHHH"};

char *directions[] = {"u", "d", "l", "r", "ul", "ur", "dl", "dr"};
int words_placed = 0;

int random_number(int max) { return rand() % max; }

// Get a random point on the grid
int *random_coordinates() {
  int row = random_number(GRID_ROWS);
  int col = random_number(GRID_COLS);
  int *coordinates = (int *)malloc(2 * sizeof(int));
  coordinates[0] = row;
  coordinates[1] = col;

  return coordinates;
}

// Movement directions for grid
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

  // When to stop recursing
  if (row >= GRID_ROWS || col >= GRID_COLS || row < 0 || col < 0) {
    printf("Reached the end of the grid\n");
    printf("cant fit word \n");
    return false;
  }

  if (grid[row][col][0] != 'X' && grid[row][col][0] != current_letter) {
    printf("encountered a letter\n");
    printf("cant fit word \n");
    return false;
  }

  // Reached the end of the word
  if (n >= length) {
    printf("There is space for: %s \n", word);
    return true;
  }
  printf("Checking position [%d][%d], grid contains: %c, current letter: %c\n",
         row, col, grid[row][col][0], current_letter);

  if (grid[row][col][0] == current_letter) {
    printf("encountered current letter\n");
  }

  // Adjust col/row based on direction
  int *directions = get_direction(direction, row, col);
  row = directions[0];
  col = directions[1];
  free(directions); // Don't forget to free the allocated memory

  return check_space(word, direction, row, col, n + 1);
}

void place_word(char *word, int index, int row, int col, char *direction,
                char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH]) {
  // Reached the end of the word
  if (index >= strlen(word)) {
    return;
  }

  int *directions = get_direction(direction, row, col);
  // Basically +1 or -1 to row / col
  row = directions[0];
  col = directions[1];

  // Add the letter to grid
  grid[row][col][0] = word[index];
  grid[row][col][1] = '\0';
  /* strncpy(grid[row][col], &word[index], 1); */
  free(directions);
  return place_word(word, index + 1, row, col, direction, grid);
}

void print_grid(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH]) {

  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      printf("%s ", grid[i][j]);
    }
    printf("\n");
  }
}

char *random_word(char words[WORDS_LENGTH][MAX_WORD_LENGTH],
                  int used_words[WORDS_LENGTH]) {
  int index;
  do {
    index = random_number(WORDS_LENGTH);
  } while (used_words[index]);
  used_words[index] = 1;

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

void shuffle_directions(char **directions) {

  for (int i = 7; i >= 0; i--) {
    // random number 0 to length
    int j = random_number(8);
    char *temp = directions[i];
    directions[i] = directions[j];
    directions[j] = temp;
  }
}

void push_to_poss_directions(char directions[GRID_ROWS * GRID_COLS][3],
                             char *direction) {
  for (int i = 0; i < GRID_ROWS * GRID_COLS * 8; i++) {
    if (directions[i][0] == '\0') {
      printf("recieved: %s", direction);
      strcpy(directions[i], direction);
      return;
    }
  }
}

possibilities get_all_possible(char *word) {

  possibilities possible;
  // Set all the directions to '\0'
  memset(possible.directions, 0, sizeof(possible.directions));

  // For each square
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      // There are 8 directions
      for (int k = 0; k < 8; k++) {
        // Check each direction if the word is possible
        if (check_space(word, directions[k], i, j, 0)) {
          // Add it to our output
          printf("passing: %s to push\n", directions[k]);
          push_to_poss_directions(possible.directions, directions[k]);
        }
      }
    }
  }

  return possible;
}

void solve(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH],
           char words[WORDS_LENGTH][MAX_WORD_LENGTH], int *used_words) {
  // If all words are placed, we are done
  if (words_placed == WORDS_LENGTH) {
    print_grid(grid);
    return;
  }

  // Get a random word
  char *word = random_word(words, used_words);
  // Choose a random coord
  int *coordinates = random_coordinates();
  int row = coordinates[0];
  int col = coordinates[1];

  /* if (words_placed == 8) { */
  /*   return; */
  /* } */

  do {
    // Shuffle directions so, for example, it doesnt start with "u" everytime
    shuffle_directions(directions);
    // Get a random word
    char *word = random_word(words, used_words);
    // Choose a random coord
    int *coordinates = random_coordinates();
    int row = coordinates[0];
    int col = coordinates[1];
    // Loop goes through the different directions stopping  when one works
    for (int i = 0; i < sizeof(directions) / sizeof(directions[0]); i++) {
      // If a word fits at the coords with the direction, place it and move on
      if (check_space(word, directions[i], row, col, 0)) {
        place_word(word, 0, row, col, directions[i], grid);
        words_placed += 1;
        printf("Placed words: %i\n", words_placed);
        break;
      }
    }
    free(coordinates);

  } while (words_placed < WORDS_LENGTH);
}
/* void solve(char grid[GRID_ROWS][GRID_COLS][GRID_DEPTH], */
/*            char words[WORDS_LENGTH][MAX_WORD_LENGTH], int *used_words) { */
/*   // If all words are placed, we are done */
/**/
/*   /* if (words_placed == 8) { */
/*   /*   return; */
/*   /* } */
/**/
/*   do { */
/*     // Shuffle directions so, for example, it doesnt start with "u"
   everytime */
/*     shuffle_directions(directions); */
/*     // Get a random word */
/*     char *word = random_word(words, used_words); */
/*     // Choose a random coord */
/*     int *coordinates = random_coordinates(); */
/*     int row = coordinates[0]; */
/*     int col = coordinates[1]; */
/*     // Loop goes through the different directions stopping  when one
   works */
/*     for (int i = 0; i < sizeof(directions) / sizeof(directions[0]); i++)
   { */
/*       // If a word fits at the coords with the direction, place it and
   move on */
/*       if (check_space(word, directions[i], row, col, grid, 0)) { */
/*         place_word(word, 0, row, col, directions[i], grid); */
/*         words_placed += 1; */
/*         printf("Placed words: %i\n", words_placed); */
/*         break; */
/*       } */
/*     } */
/*     free(coordinates); */
/**/
/*   } while (words_placed < WORDS_LENGTH); */
/*   print_grid(grid); */
/* } */

// Print possible directions
void print_direct(char directions[GRID_ROWS * GRID_COLS * 8][3]) {

  for (int i = 0; i < GRID_ROWS * GRID_COLS * 8; i++) {
    if (directions[i][0] == '\0') {
      break;
    }
    if (i % GRID_COLS == 0) {

      printf("\n");
    }
    printf("%s ", directions[i]);
  }
}

int main() {
  srand(time(NULL));

  // Initialize grid with "0"
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLS; j++) {
      strcpy(grid[i][j], "X");
    }
  }

  possibilities possible = get_all_possible("cool");

  /* solve(grid, words, used_words); */
  /* print_grid(grid); */
  /* if (check_space("cool", "d", 0, 0, 0)) { */
  /*   printf("There is space for cool"); */
  /* } */

  return 0;
}
