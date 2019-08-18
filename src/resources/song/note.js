const freqA4 = 442.;
const noteNumberA4 = 69;

const noteNumberToFreq = new Array(128).fill(null).map((v, i) => freqA4 * Math.pow(2, (i - noteNumberA4) / 12));


/**
 *
 * @param {number} noteNumber ノート番号
 * @param {number} baseRate 基本レート
 * @return {number} rate
 */
function getRateFromNoteNumberCustom(noteNumber, baseRate) {
  // freq = baseRate / (2048 - rate)
  // rate = 2048 - (baseRate / freq)
  const rate = Math.round(2048 - baseRate / noteNumberToFreq[noteNumber]);
  if (rate < 0 || rate >= 2048) {
    throw new Error(`rate out of range: ${rate} (${noteNumber})`);
  }
  return rate;
}


/**
 * @param {number} noteNumber
 * @returns {number}
 */
function getRateFromNoteNumber(noteNumber) {
  return getRateFromNoteNumberCustom(noteNumber, 131072);
}


/**
 * @param {string} strNoteNumber
 * @returns {number}
 */
function parseNoteNumber(strNoteNumber) {
  const offset = 12;    // C-1 = 0

  const match = strNoteNumber.match(/^([A-G])([b#])?(-?\d+)$/);
  if (!match) {
    throw new Error(`cannot parse noteNumber: ${strNoteNumber}`);
  }

  const octave = parseInt(match[3], 10);
  const modifier = {
    '#': 1,
    b: -1,
  }[match[2]] || 0;
  const base = 'C D EF G A B'.indexOf(match[1]);

  const noteNumber = octave * 12 + base + modifier + offset;
  if (noteNumber < 0 || noteNumber > 127) {
    throw new Error(`invalid noteNumber: ${noteNumber}`);
  }

  return noteNumber;
}


module.exports = {
  getRateFromNoteNumberCustom,
  getRateFromNoteNumber,
  parseNoteNumber,
};
