#include "helper_functions.h"
#include <cmath>
#include <assert.h>
#include <iostream>
#include "defines.h"

uint8_t readBit(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset) {
  uint8_t ret = (addr[offset] >> (7 - bit_offset)) & 0x01;
  bit_offset++;
  if (bit_offset == 8) {
    bit_offset = 0;
    offset++;
  }
  return ret;
}

uint8_t readBit(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset, const std::string &message) {
#ifdef DEBUG
  std::cout << POSITION << message << ": ";
#endif
  uint8_t ret = readBit(addr, offset, bit_offset);
#ifdef DEBUG
  std::cout << +ret << "\n";
#endif
  return ret;
}

uint32_t readNBits(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset, uint32_t n) {
  uint32_t ret = 0;
  assert(n <= 32);
  for (int32_t i = n; i > 0; i--) {
    ret += readBit(addr, offset, bit_offset) << (i - 1);
  }
  return ret;
}

uint32_t readNBits(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset, uint32_t n, const std::string &message) {
#ifdef DEBUG
  std::cout << POSITION << message << ": ";
#endif
  uint32_t ret = readNBits(addr, offset, bit_offset, n);
#ifdef DEBUG
  std::cout << ret << "\n";
#endif
  return ret;
}

uint8_t readByte(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset) {
  uint8_t ret = 0;
  if (bit_offset == 0) {
    ret = addr[offset];
    offset++;
  } else {
    for (int32_t i = 8; i > 0; i--) {
      ret += readBit(addr, offset, bit_offset) << (i - 1);
    }
  }
  return ret;
}

uint8_t readByte(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset, const std::string &message) {
#ifdef DEBUG
  std::cout << POSITION << message << ": ";
#endif
  uint8_t ret = readByte(addr, offset, bit_offset);
#ifdef DEBUG
  std::cout << +ret << "\n";
#endif
  return ret;
}

uint32_t readUnsignedInt32(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset) {
  uint32_t ret = 0;
  if (bit_offset == 0) {
    for (int32_t i = 4; i > 0; i--) {
      ret += addr[offset] << (8 * (i - 1));
      offset++;
    }
  } else {
    uint8_t leading_bits = 8 - bit_offset;
    uint8_t trailing_bits = bit_offset;
    // Read bits until stream is byte-aligned
    for (int32_t i = 0; i < leading_bits; i++) {
      ret += readBit(addr, offset, bit_offset) << (31 - i);
    }
    // Read three full bytes
    for (int32_t j = 3; j > 0; j--) {
      ret += readByte(addr, offset, bit_offset) << ((8 * (j - 1)) + trailing_bits);
    }
    // Read trailing bits from next byte
    for (int32_t k = trailing_bits; k > 0; k--) {
      ret += readBit(addr, offset, bit_offset) << (k - 1);
    }
  }
  return ret;
}

uint32_t readUnsignedInt32(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset, const std::string &message) {
#ifdef DEBUG
  std::cout << POSITION << message << ": ";
#endif
  uint32_t ret = readUnsignedInt32(addr, offset, bit_offset);
#ifdef DEBUG
  std::cout << ret << "\n";
#endif
  return ret;
}

uint32_t decodeUnsignedExpGolomb(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset) {
  uint32_t leading_zero_bits = 0;
  uint32_t code_num = 0;

  bool one_found = false;
  while (!one_found) {
    uint8_t next = readBit(addr, offset, bit_offset);
    if (next) {
      one_found = true;
    } else {
      leading_zero_bits++;
    }
  }

  for (int32_t i = leading_zero_bits; i > 0; i--) {
    code_num += readBit(addr, offset, bit_offset) << (i - 1);
  }
  return code_num + ((1 << leading_zero_bits) - 1);
}

uint32_t decodeUnsignedExpGolomb(const uint8_t *addr,
                                 uint32_t &offset,
                                 uint8_t &bit_offset,
                                 const std::string &message) {
#ifdef DEBUG
  std::cout << POSITION << message << ": ";
#endif
  uint32_t ret = decodeUnsignedExpGolomb(addr, offset, bit_offset);
#ifdef DEBUG
  std::cout << ret << "\n";
#endif
  return ret;
}

int32_t decodeSignedExpGolomb(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset) {
  uint32_t code_num = decodeUnsignedExpGolomb(addr, offset, bit_offset);
  int32_t syntax_element_value = ceil(((double) code_num) / 2.0);
  if (code_num % 2 == 0) {
    syntax_element_value *= -1;
  }
  return syntax_element_value;
}

int32_t decodeSignedExpGolomb(const uint8_t *addr, uint32_t &offset, uint8_t &bit_offset, const std::string &message) {
#ifdef DEBUG
  std::cout << POSITION << message << ": ";
#endif
  int32_t ret = decodeSignedExpGolomb(addr, offset, bit_offset);
#ifdef DEBUG
  std::cout << ret << "\n";
#endif
  return ret;
}

uint32_t getChromaArrayType(const SPS &sps) {
  if (sps.separate_colour_plane_flag) {
    return 0;
  } else {
    return sps.chroma_format_idc;
  }
}

std::string getNALUnitTypeString(uint8_t nal_unit_type) {
  switch (nal_unit_type) {
    case 0: {
      return "Unspecified";
    }
    case 1: {
      return "Coded slice of a non-IDR picture";
    }
    case 2: {
      return "Coded slice data partition A";
    }
    case 3: {
      return "Coded slice data partition B";
    }
    case 4: {
      return "Coded slice data partition C";
    }
    case 5: {
      return "Coded slice of an IDR picture";
    }
    case 6: {
      return "Supplemental enhancement information (SEI)";
    }
    case 7: {
      return "Sequence parameter set";
    }
    case 8: {
      return "Picture parameter set";
    }
    default: {
      return "DUNNO LOL";
    }
  }
}

std::string getShortNALUnitTypeString(uint8_t nal_unit_type) {
  switch (nal_unit_type) {
    case 0: {
      return "U";
    }
    case 1: {
      return "nIDR";
    }
    case 2: {
      return "pA";
    }
    case 3: {
      return "pB";
    }
    case 4: {
      return "pC";
    }
    case 5: {
      return "IDR";
    }
    case 6: {
      return "SEI";
    }
    case 7: {
      return "SPS";
    }
    case 8: {
      return "PPS";
    }
    default: {
      return "DUNNO LOL";
    }
  }
}

std::string getSliceTypeString(uint8_t slice_type) {
  /*
   * From the standard: When slice_type has a value in the range 5..9, it is a requirement of bitstream conformance that
   * all other slices of the current coded picture shall have a value of slice_type equal to the current value of
   * slice_type or equal to the current value of slice_type minus 5.
   */
  switch (slice_type) {
    case 0:
    case 5:{
      return "P";
    }
    case 1:
    case 6:{
      return "B";
    }
    case 2:
    case 7:{
      return "I";
    }
    case 3:
    case 8:{
      return "SP";
    }
    case 4:
    case 9:{
      return "SI";
    }
    default: {
      return "DUNNO LOL";
    }
  }
}

std::string getNameString(uint32_t type) {
  std::string ret;
  for (int32_t i = 4; i > 0; i--) {
    char next = type >> ((i - 1) * 8) & 0xFF;
    ret += next;
  }
  return ret;
}
