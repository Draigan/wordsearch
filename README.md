  <p align="center">
   <img style="max-width: 400px;" src="https://github.com/Draigan/wordsearch/blob/main/example.gif?raw=true" width="320" />
</p>

# Wordsearch Generator

## Description
Generate unique word searches based on a list of words. The coolest thing about this is it uses a backtracking algorithm.

## How to use
#### Make sure you have gcc or another c compiler installed
```bash
apt install gcc
```
#### Clone this repo
```bash
git clone https://github.com/Draigan/wordsearch.git
```
#### Open ws.c and add your words to the words array
```c
char words[WORDS_LENGTH][MAX_WORD_LENGTH] = {"FOOL",     "CASTLING", "KING",

                                             "ATTACKER", "SWORD",    "SHIELD",

                                             "KNIGHT",   "DRAGON",   "FAIRY"};
```
#### Edit WORDS_LENGTH macro to match length words array
```c
#define WORDS_LENGTH 9
```
#### Make sure GRID_ROWS & GRID_COLS macros are big enough to accomodate new words length
```c
#define GRID_ROWS 12
#define GRID_COLS 12
```
#### Compile ws.c
```bash
gcc -o ws.c ws
```
#### Make the file executable
```bash
chmod +x ws
```
#### Run the program
```bash
./ws
```
