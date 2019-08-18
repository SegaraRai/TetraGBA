/**
 * @param {number} rate
 * @returns {number[]}
 */
function saw(rate) {
  const arr = [];

  for (let i = 0; i < 32; i++) {
    arr[i] = Math.floor(i * rate / 32 * 16) % 16;
  }

  return arr;
}


module.exports = {
  saw,
};
