const pos = [];

for (let i = 0; i < 3; i++) {
  for (let j = 0; j < 3; j++) {
    pos.push({ x: i, y: j });
  }
}

console.log(pos);

for (const { x, y } of pos) {
  console.log("x: " + x);
  console.log("y: " + y);
}

for (const index in pos) {
  console.log(pos[index]);
}
