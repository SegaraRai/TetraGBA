function checkBit(value, bit) {
  if (typeof value !== 'number') {
    throw new Error(`value is not a number: ${value}`);
  }
  if (value < 0 || value >= (1 << bit)) {
    throw new Error(`value overflow: ${value} (${bit} bit)`);
  }
}


module.exports = {
  address: {
    SOUND1CNT_L: 0x0400060,
    SOUND1CNT_H: 0x0400062,
    SOUND1CNT_X: 0x0400064,
    //
    SOUND2CNT_L: 0x0400068,
    SOUND2CNT_H: 0x040006C,
    //
    SOUND3CNT_L: 0x0400070,
    SOUND3CNT_H: 0x0400072,
    SOUND3CNT_X: 0x0400074,
    //
    SOUND4CNT_L: 0x0400078,
    SOUND4CNT_H: 0x040007C,
  },

  /**
   * @param {number} length
   * @param {number} duty
   * @param {number} envelopeStep
   * @param {number} direction
   * @param {number} initialVolume
   * @returns {number}
   */
  envelope(length, duty, envelopeStep, direction, initialVolume) {
    checkBit(length, 6);
    checkBit(duty, 2);
    checkBit(envelopeStep, 3);
    checkBit(direction, 1);
    checkBit(initialVolume, 4);
    return length | (duty << 6) | (envelopeStep << 8) | (direction << 11) | (initialVolume << 12);
  },

  /**
   * @param {number} length
   * @param {number} duty
   * @param {number} envelopeStep
   * @param {number} direction
   * @param {number} initialVolume
   * @returns {number}
   */
  SOUND1CNT_L(shift, direction, time) {
    checkBit(shift, 3);
    checkBit(direction, 1);
    checkBit(time, 3);
    return shift | (direction << 3) | (time << 4);
  },

  /**
   * @param {number} length
   * @param {number} volume
   * @param {number} volume75
   * @returns {number}
   */
  SOUND3CNT_H(length, volume, volume75) {
    checkBit(length, 8);
    checkBit(volume, 2);
    checkBit(volume75, 1);
    return length | (volume << 13) | (volume75 << 15);
  },

  /**
   * @param {number} ratio
   * @param {number} width
   * @param {number} frequency
   * @returns {number}
   */
  SOUND4CNT_H(ratio, width, frequency) {
    checkBit(ratio, 3);
    checkBit(width, 1);
    checkBit(frequency, 4);
    return ratio | (width << 3) | (frequency << 4);
  },
};
