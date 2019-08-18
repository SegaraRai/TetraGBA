const fs = require('fs');

const { data } = require('./data');


const maxNameLen = 11;
const maxTypeLen = 13;
const typeOverride = {
  'ISRAD': 'isr',
  'SNDBUFAD': 'void*',

  'FIFO_A': 'void*',
  'FIFO_B': 'void*',
  'DMA0SAD': 'const void*',
  'DMA0DAD': 'void*',
  'DMA1SAD': 'const void*',
  'DMA1DAD': 'void*',
  'DMA2SAD': 'const void*',
  'DMA2DAD': 'void*',
  'DMA3SAD': 'const void*',
  'DMA3DAD': 'void*',
};



function pad(str, len, suffix = '') {
  return str + suffix + ' '.repeat(len - str.length);
}


function padAddress(address) {
  return ('0000000' + address.replace(/h$/, '')).substr(-8);
}


function createAddressHpp() {
  const ret = [
    '#ifndef _gba_address_hpp_',
    '#define _gba_address_hpp_',
    '',
    '#include <cstdint>',
    '',
    '',
    'namespace gba::address {',
  ];

  let isFirst = true;
  for (const _line of data.split('\n')) {
    const line = _line.trim();
    if (!line || line[0] === '#') {
      continue;
    }

    const [address, size, rwFlag, name, description] = line.split(/\s+/);
    const valid = /^[\dA-F]+h$/.test(address) && /^\w+$/.test(name) && (size === '2' || size === '4');

    if (!valid) {
      if (_line[0] !== ' ') {
        if (!isFirst) {
          ret.push('');
        }
        isFirst = false;
        ret.push(`  // ${line}`);
      } else {
        ret.push(`  //${' '.repeat(52)}// ${line}`);
      }
      continue;
    }

    ret.push(`  constexpr std::uintptr_t ${pad(name, maxNameLen)} = 0x${padAddress(address)};    // ${line}`);
  }

  ret.push(...[
    '}   // namespace gba::address',
    '',
    '#endif',
    '',
  ]);

  return ret.join('\n');
}


function createTemplatePointerHpp() {
  const ret = [
    '#ifndef _gba_template_pointer_hpp_',
    '#define _gba_template_pointer_hpp_',
    '',
    '#include <cstdint>',
    '',
    '#include "address.hpp"',
    '#include "type.hpp"',
    '',
    '',
    'namespace gba::template_pointer {',
  ];

  let isFirst = true;
  for (const _line of data.split('\n')) {
    const line = _line.trim();
    if (!line || line[0] === '#') {
      continue;
    }

    const [address, size, rwFlag, name, description] = line.split(/\s+/);
    const valid = /^[\dA-F]+h$/.test(address) && /^\w+$/.test(name) && (size === '2' || size === '4');

    if (!valid) {
      if (_line[0] !== ' ') {
        if (!isFirst) {
          ret.push('');
        }
        isFirst = false;
        ret.push(`  // ${line}`);
      } else {
        ret.push(`  //${' '.repeat(86)}// ${line}`);
      }
      continue;
    }

    const type = typeOverride[name] || (size === '2' ? 'std::uint16_t' : 'std::uint32_t');

    ret.push(`  template<typename T=${pad(type, maxTypeLen, '>')}${' '.repeat(50)}    // ${line}`);
    ret.push(`  static const auto ${pad(name, maxNameLen)} = reinterpret_cast<volatile T*>(address::${name});`);
  }

  ret.push(...[
    '}   // namespace gba::template_pointer',
    '',
    '#endif',
    '',
  ]);

  return ret.join('\n');
}


function createPointerHpp() {
  const ret = [
    '#ifndef _gba_pointer_hpp_',
    '#define _gba_pointer_hpp_',
    '',
    '#include <cstdint>',
    '',
    '#include "template_pointer.hpp"',
    '',
    '',
    'namespace gba::pointer {',
  ];

  let isFirst = true;
  for (const _line of data.split('\n')) {
    const line = _line.trim();
    if (!line || line[0] === '#') {
      continue;
    }

    const [address, size, rwFlag, name, description] = line.split(/\s+/);
    const valid = /^[\dA-F]+h$/.test(address) && /^\w+$/.test(name) && (size === '2' || size === '4');

    if (!valid) {
      if (_line[0] !== ' ') {
        if (!isFirst) {
          ret.push('');
        }
        isFirst = false;
        ret.push(`  // ${line}`);
      } else {
        ret.push(`  //${' '.repeat(66)}// ${line}`);
      }
      continue;
    }

    ret.push(`  static const auto ${pad(name, maxNameLen)} = template_pointer::${pad(name, maxNameLen, '<>;')}    // ${line}`);
  }

  ret.push(...[
    '}   // namespace gba::pointer',
    '',
    '#endif',
    '',
  ]);

  return ret.join('\n');
}


function createRegisterHpp() {
  // cannot use pointer.hpp somehow

  const ret = [
    '#ifndef _gba_register_hpp_',
    '#define _gba_register_hpp_',
    '',
    '#include <cstdint>',
    '',
    '#include "address.hpp"',
    '#include "type.hpp"',
    '',
    '',
    'namespace gba::reg {',
    '  template<typename T, std::uintptr_t Address>',
    '  volatile auto& byAddress = *reinterpret_cast<volatile T*>(Address);',
    '',
  ];

  let isFirst = true;
  for (const _line of data.split('\n')) {
    const line = _line.trim();
    if (!line || line[0] === '#') {
      continue;
    }

    const [address, size, rwFlag, name, description] = line.split(/\s+/);
    const valid = /^[\dA-F]+h$/.test(address) && /^\w+$/.test(name) && (size === '2' || size === '4');

    if (!valid) {
      if (_line[0] !== ' ') {
        if (!isFirst) {
          ret.push('');
        }
        isFirst = false;
        ret.push(`  // ${line}`);
      } else {
        //ret.push(`  //${' '.repeat(60)}// ${line}`);
        ret.push(`  //${' '.repeat(102)}// ${line}`);
      }
      continue;
    }

    const type = typeOverride[name] || (size === '2' ? 'std::uint16_t' : 'std::uint32_t');

    //ret.push(`  static volatile auto& ${pad(name, maxNameLen)} = *pointer::${pad(name, maxNameLen, ';')}    // ${line}`);
    ret.push(`  static volatile auto& ${pad(name, maxNameLen)} = *reinterpret_cast<${pad(type, maxTypeLen, ` volatile*>(address::${pad(name, maxNameLen, ');')}`)}   // ${line}`);
  }

  ret.push(...[
    '}   // namespace gba::reg',
    '',
    '#endif',
    '',
  ]);

  return ret.join('\n');
}


function createTemplateRegisterHpp() {
  const ret = [
    '#ifndef _gba_template_register_hpp_',
    '#define _gba_template_register_hpp_',
    '',
    '#include <cstdint>',
    '',
    '#include "type.hpp"',
    '',
    '',
    'namespace gba::template_reg {',
  ];

  let isFirst = true;
  for (const _line of data.split('\n')) {
    const line = _line.trim();
    if (!line || line[0] === '#') {
      continue;
    }

    const [address, size, rwFlag, name, description] = line.split(/\s+/);
    const valid = /^[\dA-F]+h$/.test(address) && /^\w+$/.test(name) && (size === '2' || size === '4');

    if (!valid) {
      if (_line[0] !== ' ') {
        if (!isFirst) {
          ret.push('');
        }
        isFirst = false;
        ret.push(`  // ${line}`);
      } else {
        ret.push(`  //${' '.repeat(68)}// ${line}`);
      }
      continue;
    }

    const type = typeOverride[name] || (size === '2' ? 'std::uint16_t' : 'std::uint32_t');

    ret.push(`  template<typename T=${pad(type, maxTypeLen, '>')}${' '.repeat(36)}// ${line}`);
    ret.push(`  inline volatile T& ${pad(name, maxNameLen)} = *template_pointer::${name}<T>;`);
  }

  ret.push(...[
    '}   // namespace gba::template_reg',
    '',
    '#endif',
    '',
  ]);

  return ret.join('\n');
}


fs.writeFileSync('../address.hpp', createAddressHpp());
fs.writeFileSync('../template_pointer.hpp', createTemplatePointerHpp());
fs.writeFileSync('../pointer.hpp', createPointerHpp());
fs.writeFileSync('../template_register.hpp', createTemplateRegisterHpp());
fs.writeFileSync('../register.hpp', createRegisterHpp());
