function solve() {
  for (let x = 0; x < 9; x++) {
    for (let y = 0; y < 9; y++) {
      if (grid[x][y] == 0) {
        for (let n = 1; n < 10; n++) {
          if (legal(x, y, n)) {
            grid[x][y] = n;
            solve();
            grid[x][y] = 0;
          }
        }

        return;
      }
    }
  }
  console.log(grid.join("\n"));
}

solve();
