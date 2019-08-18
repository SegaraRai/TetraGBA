const child_process = require('child_process');
const fs = require('fs');
const path = require('path');


class SoundBuilder {
  /**
   * @param {string} filepath
   * @param {number} samplingRate
   */
  constructor(filepath, samplingRate) {
    if (typeof samplingRate !== 'number' || isNaN(samplingRate) || !isFinite(samplingRate)) {
      throw new Error('samplingRate is not a valid number');
    }

    const outPath = path.join(filepath, '..', path.parse(filepath).name + '.cpp');
    const varName = path.parse(filepath).name;

    const prescaler = Math.round(65536 - 16777216 / samplingRate);
    if (prescaler < 0 || prescaler >= 65535) {
      throw new Error('prescaler out of range; please change samplingRate');
    }

    const cp = child_process.spawn(SoundBuilder.FFMPEG, [
      '-i', filepath,
      //
      '-f', 's8',
      '-ac', '1',
      '-ar', samplingRate.toString(),
      //
      'pipe:1',
    ], {
      windowsHide: true,
    });

    const buffers = [];

    cp.stdout.on('data', buffer => {
      buffers.push(buffer);
    });

    /*
    cp.stderr.on('data', log => {
      console.log(log.toString('utf-8'));
    });
    //*/

    cp.once('error', error => {
      throw error;
    });

    cp.once('close', () => {
      const pcm = Buffer.concat(buffers);
      const header = Buffer.alloc(8);

      header.writeUInt32LE(Math.ceil(pcm.length / 16), 0);
      header.writeUInt32LE(prescaler, 4);

      const buffer = Buffer.concat([header, pcm, Buffer.alloc((16 - pcm.length % 16) % 16, 0)]);

      let str = `#include <cstdint>\n\n\nnamespace Sound {\n  std::uint32_t ${varName}[${buffer.length / 4}] = {\n    `;
      for (let i = 0; i < buffer.length / 4; i++) {
        str += '0x' + ('0000000' + buffer.readUInt32LE(i * 4).toString(16).toUpperCase()).substr(-8);
        str += i % 16 == 15 ? ',\n    ' : ', ';
      }
      str = str.trimRight() + '\n  };\n}\n';

      fs.writeFileSync(outPath, str);
    });
  }
}

SoundBuilder.FFMPEG = 'ffmpeg';


if (!process.argv[2] || !process.argv[3]) {
  console.log(`usage: node ${process.argv[1]} sound.wav samplingRate`);
  process.exit(1);
}

new SoundBuilder(process.argv[2], parseInt(process.argv[3], 10));
