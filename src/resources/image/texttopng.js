const child_process = require('child_process');
const fs = require('fs');
const path = require('path');


const imageMagick = 'magick';
const fallbackIndex = 0;


if (!process.argv[4]) {
  console.log(`usage: node ${process.argv[1]} output.png font.png text`);
  process.exit(1);
}

const outputPngPath = process.argv[2];
const fontPngPath = process.argv[3];
const text = process.argv[4];

const fontJsonPath = path.join(fontPngPath, '..', `${path.parse(fontPngPath).name}.json`);

const jsonData = JSON.parse(fs.readFileSync(fontJsonPath, 'utf-8'));

// magick ( font.png -crop 8x8+8+24 ) ( font.png -crop 8x8+16+32 ) +append output.png
const args = [];
for (const char of text) {
  const codePoint = char.codePointAt(0);
  let index = jsonData.codePoints.indexOf(codePoint);
  if (index < 0) {
    index = fallbackIndex;
  }
  const x = (index % jsonData.columns) * jsonData.tileWidth;
  const y = Math.floor(index / jsonData.columns) * jsonData.tileHeight;
  args.push('(', fontPngPath, '-crop', `${jsonData.tileWidth}x${jsonData.tileHeight}+${x}+${y}`, ')');
}
args.push('+append', '-define', 'png:color-type=3', outputPngPath);

child_process.spawnSync(imageMagick, args);
