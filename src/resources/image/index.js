const fs = require('fs');
const path = require('path');
const { PNG } = require('pngjs');


class ImageBuilder {
  /**
   * @typedef {object} CppVar
   * @property {string} name
   * @property {string} type
   * @property {any} value
   */

  /**
   * @typedef {object} ObjInfo
   * @property {string} id
   * @property {number} x
   * @property {number} y
   * @property {number} width
   * @property {number} height
   * @property {number} palette
   * @property {string} destSize '<w>x<h>' (e.g. 128x128) or 'obj' (minimum obj size) or 'same' (same size)
   * @property {'normal' | 'renew' | 'renew_object'} paletteMode
   * @property {'normal' | 'renew' | 'ignore'} tileMode take effect only for BG tiles
   */

  /**
   * @typedef {object} Size
   * @property {number} width
   * @property {number} height
   */

  /**
   * @typedef {object} Tile
   * @property {number} width
   * @property {number} height
   * @property {string[]} blocks
   */

  /**
   * @typedef {object} TileObject
   * @property {Tile} tile
   * @property {number[] | null} map
   * @property {ObjInfo} info
   * @property {CppVar[]} vars
   */

  /**
   *
   * @param {string} tileData
   * @returns {number}
   */
  static tileDataToArray(tileData) {
    return Array.from(tileData.match(/.{8}/g)).map(str => parseInt(Array.from(str.match(/.{1}/g)).reverse().join(''), 16));
  }

  /**
   *
   * @param {CppVar} cppVar
   * @param {number} indent
   * @returns {string}
   */
  static buildVar(cppVar, indent) {
    const TYPE_DEFINITION = {
      u8: {
        cppTypeName: 'std::uint8_t',
        hex: true,
        pad: '00',
      },
      u16: {
        cppTypeName: 'std::uint16_t',
        hex: true,
        pad: '0000',
      },
      u32: {
        cppTypeName: 'std::uint32_t',
        hex: true,
        pad: '00000000',
      },
      uint: {
        cppTypeName: 'unsigned int',
        hex: false,
        pad: null,
      },
    };

    /** @type {keyof(TYPE_DEFINITION)} */
    const type = cppVar.type;

    const typeInfo = TYPE_DEFINITION[type];
    if (!typeInfo) {
      throw new Error(`unknown type: ${type}`);
    }

    /**
     *
     * @param {number} value
     * @returns {string}
     */
    function convertValue(value) {
      return typeInfo.hex ? `0x${(typeInfo.pad + value.toString(16)).toUpperCase().substr(-typeInfo.pad.length)}` : value.toString();
    }

    const strIndent = ' '.repeat(indent);
    const strIndentInner = '  ' + strIndent;

    if (Array.isArray(cppVar.value)) {
      let str = `${strIndent}constexpr std::array<${typeInfo.cppTypeName}, ${cppVar.value.length}> ${cppVar.name}{\n${strIndentInner}`;
      for (let i = 0; i < cppVar.value.length; i++) {
        str += convertValue(cppVar.value[i]);
        str += i % 16 == 15 ? `,\n${strIndentInner}` : ', ';
      }
      str = `${str.trimRight()}\n${strIndent}};`;
      return str;
    }

    return `${strIndent}constexpr ${typeInfo.cppTypeName} ${cppVar.name} = ${cppVar.value};`;
  }

  /**
   * GBAの15ビットRGBに変換
   * @param {number} r
   * @param {number} g
   * @param {number} b
   * @returns {number}
   */
  static packRGBForGBA(r, g, b) {
    // https://problemkaputt.de/gbatek.htm#lcdvrambitmapbgmodes

    /**
     * 色を変換
     * @param {number} c
     * @returns {number}
     */
    function reduceColor(c) {
      return Math.round(c / 255 * 31);
    }

    return (reduceColor(b) << 10) | (reduceColor(g) << 5) | reduceColor(r);
  }


  /**
   * RGBAを32ビット値に変換
   * @param {number} r
   * @param {number} g
   * @param {number} b
   * @param {number} a
   * @returns {number}
   */
  static packRGBA(r, g, b, a) {
    return (r << 24) | (g << 16) | (b << 8) | a;
  }


  /**
   * 32ビット値RGBAを分解
   * @param {number} rgba
   * @returns {[number, number, number, number]}
   */
  static unpackRGBA(rgba) {
    return [
      (rgba >> 24) & 255,
      (rgba >> 16) & 255,
      (rgba >> 8) & 255,
      rgba & 255,
    ];
  }

  /**
   *
   * @param {string} sizeSpec
   * @return {Size}
   */
  static parseSize(sizeSpec) {
    const [width, height] = sizeSpec.split('x').map(v => parseInt(v, 10));
    return {
      width,
      height,
    };
  }

  /**
   *
   * @param {string} sizeSpec
   * @param {Size} srcSize
   * @return {Size}
   */
  static calcSize(sizeSpec, srcSize) {
    const { width, height } = srcSize;

    switch (sizeSpec) {
      case 'same':
        return {
          width,
          height,
        };

      case 'obj': {
        const objSize = ImageBuilder.OBJ_SIZES_ORDERED.find(objSize => width <= objSize.width && height <= objSize.height);
        if (!objSize) {
          throw new Error(`no suitable obj size for ${width}x${height}`);
        }
        return {
          width: objSize.width,
          height: objSize.height,
        };
      }

      default:
        return ImageBuilder.parseSize(sizeSpec);
    }
  }

  /**
   *
   * @param {number} paletteIndex
   * @param {number} color
   * @param {boolean} force
   */
  colorToIndex(paletteIndex, color, force = false) {
    const reversePaletteMap = this.reversePaletteMaps[paletteIndex];
    const nextColorIndex = this.nextColorIndices[paletteIndex];

    const colorIndices = reversePaletteMap.get(color);
    if (colorIndices) {
      if (force) {
        if (nextColorIndex >= ImageBuilder.MAX_COLORS_PER_PALETTE) {
          throw new Error(`too many color used in palette ${paletteIndex}`);
        }
        this.nextColorIndices[paletteIndex]++;
        colorIndices.push(nextColorIndex);
        return nextColorIndex;
      }
      return colorIndices[0];
    }

    if (nextColorIndex >= ImageBuilder.MAX_COLORS_PER_PALETTE) {
      throw new Error(`too many color used in palette ${paletteIndex}`);
    }

    reversePaletteMap.set(color, [nextColorIndex]);
    this.nextColorIndices[paletteIndex]++;

    return nextColorIndex;
  }

  /**
   * 8x8ブロックをパレット化
   * @param {number} x
   * @param {number} y
   * @param {number} paletteIndex
   * @param {Map<number, number> | null} renewMap
   * @return {string}
   */
  calcBlock(x, y, paletteIndex, renewMap = null) {
    if (renewMap) {
      renewMap.set(this.transparentColor, '0');
    }
    let hex = '';
    for (let ry = 0; ry < 8; ry++) {
      const ay = y * 8 + ry;
      for (let rx = 0; rx < 8; rx++) {
        const ax = x * 8 + rx;
        const color = this.imageData32[ay * this.png.width + ax];
        let hexColor;
        if (renewMap) {
          hexColor = renewMap.get(color);
          if (!hexColor) {
            hexColor = this.colorToIndex(paletteIndex, color, !renewMap.has(color)).toString(16).toUpperCase();
            renewMap.set(color, hexColor);
          }
        } else {
          hexColor = this.colorToIndex(paletteIndex, color).toString(16).toUpperCase();
        }
        hex += hexColor;
      }
    }
    return hex;
  }

  /**
   *
   * @param {ObjInfo} objInfo
   * @return {Tile}
   */
  calcTile(objInfo) {
    /** @type {Map<number, number> | null} */
    let renewMap = objInfo.paletteMode === 'renew_object' ? new Map() : null;

    const size = ImageBuilder.calcSize(objInfo.destSize, {
      width: objInfo.width,
      height: objInfo.height,
    });

    /** @type {string[]} */
    const blocks = [];
    for (let ry = 0; ry < size.height; ry++) {
      for (let rx = 0; rx < size.width; rx++) {
        if (rx >= objInfo.width || ry >= objInfo.height) {
          blocks.push(ImageBuilder.EMPTY_BLOCK);
          continue;
        }
        if (objInfo.paletteMode === 'renew') {
          renewMap = new Map();
        }
        blocks.push(this.calcBlock(objInfo.x + rx, objInfo.y + ry, objInfo.palette, renewMap));
      }
    }

    return {
      ...size,
      blocks,
    };
  }


  /**
   *
   * @param {string} line
   * @returns {TileObject | undefined}
   */
  parseObject(line) {
    if (!line || line.startsWith('#')) {
      return;
    }

    // CSV列名
    const keys = ['id', 'x', 'y', 'width', 'height', 'palette', 'destSize', 'paletteMode', 'tileMode'];
    // 数値のCSV列
    const numberKeyIndexSet = new Set(['x', 'y', 'palette'].map(key => keys.indexOf(key)));

    /** @type {ObjInfo} */
    const objInfo = line.split(',').reduce((acc, value, index) => ({
      ...acc,
      [keys[index]]: numberKeyIndexSet.has(index) ? parseInt(value, 10) : value,
    }), {});

    switch (objInfo.width) {
      case 'full':
        objInfo.width = Math.floor(this.png.width / 8);
        break;

      default:
        objInfo.width = parseInt(objInfo.width, 10);
        break;
    }

    switch (objInfo.height) {
      case 'full':
        objInfo.height = Math.floor(this.png.height / 8);
        break;

      default:
        objInfo.height = parseInt(objInfo.height, 10);
        break;
    }

    const tile = this.calcTile(objInfo);

    return {
      tile,
      info: objInfo,
      vars: [
        {
          name: 'Palette',
          type: 'u8',
          value: objInfo.palette,
        },
        {
          name: 'Width',
          type: 'uint',
          value: objInfo.width,
        },
        {
          name: 'Height',
          type: 'uint',
          value: objInfo.height,
        },
        {
          name: 'MapWidth',
          type: 'uint',
          value: tile.width,
        },
        {
          name: 'MapHeight',
          type: 'uint',
          value: tile.height,
        },
      ],
    };
  }

  /**
   *
   * @param {string} name
   * @param {string[]} pngPaths
   * @param {'bg' | 'obj'} type
   */
  constructor(name, pngPaths, type) {
    this.name = name;
    this.paths = pngPaths.map(pngPath => ({
      pngPath: pngPath,
      csvPath: path.join(pngPath, '..', `${path.parse(pngPath).name}.csv`),
    }));
    this.outHppPath = path.join(`${this.name}.hpp`);

    switch (type) {
      case 'bg':
        /** @type {Map<string, number[]>} */
        this.reverseTileMap = new Map();
        /** @type {number} */
        this.nextTileIndex = 0;

        /**
         * @param {string} tile
         * @param {boolean} force
         * @returns {number}
         */
        this.tileToIndex = function tileToIndex(tile, force = false) {
          const nextTileIndex = this.nextTileIndex;
          const tileIndices = this.reverseTileMap.get(tile);
          if (tileIndices) {
            if (force) {
              if (nextTileIndex >= ImageBuilder.MAX_TILES) {
                throw new Error('too many tile used');
              }
              tileIndices.push(nextTileIndex);
              this.nextTileIndex++;
              return nextTileIndex;
            }
            return tileIndices[0];
          }
          if (nextTileIndex >= ImageBuilder.MAX_TILES) {
            throw new Error('too many tile used');
          }
          this.reverseTileMap.set(tile, [nextTileIndex]);
          this.nextTileIndex++;
          return nextTileIndex;
        };

        for (let i = 0; i < ImageBuilder.RESERVED_TILES_BG; i++) {
          this.tileToIndex(ImageBuilder.EMPTY_BLOCK, true);
        }
        break;

      case 'obj':
        this.tileIndex = ImageBuilder.RESERVED_TILES_OBJ;
        break;
    }

    /** @type {TileObject[]} */
    this.objects = [];

    for (const { pngPath, csvPath } of this.paths) {
      this.png = PNG.sync.read(fs.readFileSync(pngPath));
      this.csv = fs.readFileSync(csvPath, 'utf-8');

      /** @type {number[]} */
      this.imageData32 = new Array(this.png.width * this.png.height);
      for (let i = 0; i < this.imageData32.length; i++) {
        this.imageData32[i] = ImageBuilder.packRGBA(this.png.data[i * 4], this.png.data[i * 4 + 1], this.png.data[i * 4 + 2], this.png.data[i * 4 + 3]);
      }

      const transparentColor = this.imageData32[this.imageData32.length - 1];
      if (this.transparentColor == null) {
        this.transparentColor = transparentColor;
      } else if (this.transparentColor !== transparentColor) {
        throw new Error('transparentColor conflict');
      }

      if (!this.reversePaletteMaps) {
        /**
         * Color Value => Palette Color Indices
         * @type {Map<number, number[]>[]}
         */
        this.reversePaletteMaps = [];
        /** @type {number[]} */
        this.nextColorIndices = [];
        for (let i = 0; i < ImageBuilder.MAX_PALETTES; i++) {
          this.reversePaletteMaps.push(new Map());
          this.nextColorIndices.push(0);
          this.colorToIndex(i, this.transparentColor);
        }
      }

      const lines = this.csv.split('\n').slice(1).map(line => line.trim());

      /** @type {TileObject[]} */
      const objects = lines.map(line => this.parseObject(line)).filter(x => x);

      switch (type) {
        case 'bg':
          for (const obj of objects) {
            if (obj.info.tileMode === 'ignore') {
              continue;
            }

            const force = obj.info.tileMode === 'renew';

            obj.map = obj.tile.blocks.map(block => this.tileToIndex(block, force));

            obj.vars = obj.vars.concat([
              {
                name: 'FirstTileIndex',
                type: 'u16',
                value: obj.map[0],
              },
              {
                name: 'MapData',
                type: 'u16',
                // https://problemkaputt.de/gbatek.htm#lcdvrambgscreendataformatbgmap
                value: obj.map.map(tileIndex => (obj.info.palette << 12) | tileIndex),
              },
            ]);
          }
          break;

        case 'obj':
          for (const obj of objects) {
            const sizeSpec = `${obj.tile.width}x${obj.tile.height}`;
            const objSize = ImageBuilder.OBJ_SIZE_MAP.get(sizeSpec);
            if (!objSize) {
              throw new Error(`invalid OBJ size: ${sizeSpec}`);
            }

            obj.vars = obj.vars.concat([
              {
                name: 'TileIndex',
                type: 'u16',
                value: this.tileIndex,
              },
              {
                name: 'ObjShape',
                type: 'u16',
                value: `gba::OBJATTR0::OBJSHAPE::${['SQUARE', 'HORIZONTAL', 'VERTICAL'][objSize.shape]}`,
              },
              {
                name: 'ObjSize',
                type: 'u16',
                value: `gba::OBJATTR1::OBJSIZE::SIZE${objSize.size}`,
              },
              {
                name: 'Attribute0Base',
                type: 'u16',
                value: 'gba::OBJATTR0::OBJMODE::NORMAL | gba::OBJATTR0::COLOR16 | ObjShape',
              },
              {
                name: 'Attribute1Base',
                type: 'u16',
                value: 'ObjSize',
              },
              {
                name: 'Attribute2Base',
                type: 'u16',
                value: 'gba::OBJATTR2::TILE(TileIndex) | gba::OBJATTR2::PRIORITY0 | gba::OBJATTR2::PALETTE(Palette)',
              },
            ]);

            this.tileIndex += obj.tile.width * obj.tile.height;
          }
          break;
      }

      this.objects = this.objects.concat(objects);
    }

    // https://problemkaputt.de/gbatek.htm#lcdcolorpalettes
    const palettes = this.reversePaletteMaps.map(reversePaletteMap => {
      const palette = Array.from(reversePaletteMap.entries()).reduce(
        (acc, [color, indices]) => {
          const val = ImageBuilder.packRGBForGBA(...ImageBuilder.unpackRGBA(color));
          for (const index of indices) {
            acc[index] = val;
          }
          return acc;
        }, new Array(ImageBuilder.MAX_COLORS_PER_PALETTE).fill(0));
      palette[0] = 0;
      for (const [index, color] of palette.entries()) {
        if (!color) {
          continue;
        }
        if (palette.includes(color, index + 1)) {
          console.warn(`Duplicate color: ${color}`);
        }
      }
      return palette;
    });

    /** @type {string[]} */
    let tiles;

    switch (type) {
      case 'bg':
        tiles = Array.from(this.reverseTileMap.entries())
          .reduce((acc, [block, indices]) => {
            for (const index of indices) {
              acc[index] = block;
            }
            return acc;
          }, new Array(this.nextTileIndex));
        break;

      case 'obj':
        tiles = new Array(ImageBuilder.RESERVED_TILES_OBJ).fill(ImageBuilder.EMPTY_BLOCK)
          .concat(...this.objects.map(obj => obj.tile.blocks));
        break;
    }

    /** @type {CppVar[]} */
    const vars = [
      {
        name: 'PaletteData',
        type: 'u16',
        value: [].concat(...palettes),
      },
      {
        name: 'TileData',
        type: 'u32',
        value: [].concat(...tiles.map(tile => ImageBuilder.tileDataToArray(tile))),
      },
    ];

    const perVarCode = this.objects.filter(obj => obj.info.id).map(obj => `
  namespace ${obj.info.id} {
${obj.vars.map(v => ImageBuilder.buildVar(v, 4)).join('\n\n')}
  }   // namespace ${obj.info.id}
`.trimRight().replace(/^\n+/, '')).join('\n\n\n');

    let code = `
#pragma once

#include <array>
#include <cstdint>

#include <gba.hpp>


namespace Tile::${this.name} {
${vars.map(v => ImageBuilder.buildVar(v, 2)).join('\n\n')}



${perVarCode}
}   // namespace Tile::${this.name}
`.trim() + '\n';

    fs.writeFileSync(this.outHppPath, code);
  }
}

ImageBuilder.OBJ_SIZE_KEYS = ['1x1', '2x1', '1x2', '2x2', '4x1', '1x4', '4x4', '4x2', '2x4', '8x8', '8x4', '4x8'];
ImageBuilder.OBJ_SIZES = ImageBuilder.OBJ_SIZE_KEYS.map(key => ({
  key,
  ...ImageBuilder.parseSize(key),
}));
ImageBuilder.OBJ_SIZES_ORDERED = [...ImageBuilder.OBJ_SIZES]
  .sort((a, b) => a.width * a.height - b.width * b.height);
/** @type {Map<string, {shape: number, size: number}>} */
ImageBuilder.OBJ_SIZE_MAP = ImageBuilder.OBJ_SIZES.reduce((acc, { key }, index) => {
  acc.set(key, {
    shape: index % 3,
    size: Math.floor(index / 3),
  });
  return acc;
}, new Map());
ImageBuilder.MAX_PALETTES = 16;
ImageBuilder.MAX_COLORS_PER_PALETTE = 16;
ImageBuilder.MAX_TILES = 1024;
ImageBuilder.EMPTY_BLOCK = '0'.repeat(64);
ImageBuilder.RESERVED_TILES_BG = 1;
ImageBuilder.RESERVED_TILES_OBJ = 0;


if (!process.argv[4]) {
  console.log(`usage: node ${process.argv[1]} name [bg|obj] image1.png [image2.png [image3.png ...]]`);
  process.exit(1);
}

new ImageBuilder(process.argv[2], process.argv.slice(4), process.argv[3]);
