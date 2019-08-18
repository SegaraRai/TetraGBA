const fs = require('fs');
const path = require('path');
const { PNG } = require('pngjs');


if (!process.argv[2]) {
  console.log(`usage: node ${process.argv[1]} image.hex`);
  process.exit(1);
}


const colorOn = Buffer.from([255, 255, 255, 255]);
const colorOff = Buffer.from([0, 0, 0, 0]);
const bytesPerPixel = colorOn.length;

const tileWidth = 8;
const columns = 16;

const bitmapData = new Array(16)
  .fill(0)
  .map((_v, i) => {
    const buffers = [];
    for (let k = 0; k < 4; k++) {
      buffers.unshift((i & (1 << k)) ? colorOn : colorOff);
    }
    return Buffer.concat(buffers);
  });


const hexPath = process.argv[2];
const pngPath = path.join(hexPath, '..', `${path.parse(hexPath).name}.png`);
const jsonPath = path.join(hexPath, '..', `${path.parse(hexPath).name}.json`);


const characters = fs.readFileSync(hexPath, 'utf-8')
  .trim()
  .split('\n')
  .map(line => {
    const [strCodePoint, strBitmap] = line.split(':');
    return {
      codePoint: parseInt(strCodePoint, 16),
      bitmap: [...strBitmap].map(char => parseInt(char, 16)),
    };
  });


const tileHeight = characters[1].bitmap.length * 4 / tileWidth;
const rows = Math.ceil(characters.length / columns);

const width = columns * tileWidth;
const height = rows * tileHeight;


const buffer = Buffer.alloc(width * height * bytesPerPixel, colorOff);
for (const [index, character] of characters.entries()) {
  const baseX = (index % columns) * tileWidth;
  const baseY = Math.floor(index / columns) * tileHeight;

  for (const [charIndex, char] of character.bitmap.entries()) {
    const x = baseX + ((charIndex * 4) % tileWidth);
    const y = baseY + Math.floor((charIndex * 4) / tileWidth);

    bitmapData[char].copy(buffer, (y * width + x) * bytesPerPixel);
  }
}


const png = new PNG({
  width,
  height,
  bitDepth: 8,
  colorType: 6,
  inputColorType: 6,
  inputHasAlpha: true,
});

png.data = buffer;

png.pack().pipe(fs.createWriteStream(pngPath));


const jsonData = {
  tileWidth,
  tileHeight,
  columns,
  rows,
  codePoints: characters.map(c => c.codePoint),
};

fs.writeFileSync(jsonPath, JSON.stringify(jsonData, null, 2));
