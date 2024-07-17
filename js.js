// Define the grid size
const GRID_SIZE = 5;
const grid = Array.from({ length: GRID_SIZE }, () =>
  Array(GRID_SIZE).fill("0"),
);

// Directions for placing words
const directions = [
  { dx: 0, dy: 1 }, // Right
  { dx: 1, dy: 0 }, // Down
  { dx: 1, dy: 1 }, // Down-Right
  { dx: 1, dy: -1 }, // Down-Left
];

// Words to place in the grid
const words = ["CAT", "DOG", "FISH"];

// Function to shuffle an array
function shuffle(array) {
  for (let i = array.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [array[i], array[j]] = [array[j], array[i]];
  }
}

function canPlaceWord(grid, word, x, y, direction) {
  for (let i = 0; i < word.length; i++) {
    const newX = x + i * direction.dx;
    const newY = y + i * direction.dy;

    if (
      newX < 0 ||
      newX >= GRID_SIZE ||
      newY < 0 ||
      newY >= GRID_SIZE ||
      (grid[newX][newY] !== "0" && grid[newX][newY] !== word[i])
    ) {
      return false;
    }
  }
  return true;
}

function placeWord(grid, word, x, y, direction) {
  for (let i = 0; i < word.length; i++) {
    const newX = x + i * direction.dx;
    const newY = y + i * direction.dy;
    grid[newX][newY] = word[i];
  }
}

function removeWord(grid, word, x, y, direction) {
  for (let i = 0; i < word.length; i++) {
    const newX = x + i * direction.dx;
    const newY = y + i * direction.dy;
    grid[newX][newY] = "0";
  }
}

function printGrid(grid) {
  grid.forEach((row) => console.log(row.join(" ")));
  console.log();
}

function solve(grid, words, index = 0) {
  if (index === words.length) {
    printGrid(grid);
    return true;
  }

  const word = words[index];
  const positions = [];
  for (let x = 0; x < GRID_SIZE; x++) {
    for (let y = 0; y < GRID_SIZE; y++) {
      positions.push({ x, y });
    }
  }

  shuffle(positions);
  shuffle(directions);

  for (const { x, y } of positions) {
    for (const direction of directions) {
      if (canPlaceWord(grid, word, x, y, direction)) {
        console.log(
          `Placing word '${word}' at (${x}, ${y}) in direction (${direction.dx}, ${direction.dy})`,
        );
        placeWord(grid, word, x, y, direction);

        if (solve(grid, words, index + 1)) {
          return true;
        }

        console.log(`Removing word '${word}' from (${x}, ${y})`);
        removeWord(grid, word, x, y, direction);
      }
    }
  }
  return false;
}

// Initialize the word search
if (solve(grid, words)) {
  console.log("Word search generated successfully!");
} else {
  console.log("Failed to generate word search.");
}
