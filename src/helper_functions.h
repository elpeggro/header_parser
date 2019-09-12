#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_

#include <cstdint>
#include <string>
#include "structs.h"

/**
 * Reads a single bit from the position addr + offset + bit_offset. Increments bit_offset. If a byte border is reached,
 * i.e., bit_offset reaches 8, offset is incremented and bit_offset reset to 0.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @return The read value.
 */
uint8_t readBit(const uint8_t *addr, size_t &offset, uint8_t &bit_offset);
/**
 * Reads a single bit from the position addr + offset + bit_offset. Increments bit_offset. If a byte border is reached,
 * i.e., bit_offset reaches 8, offset is incremented and bit_offset reset to 0. Additionally, prints a debug message to
 * stdout if DEBUG is defined.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @param message Parameter name that is printed together with the read value.
 * @return The read value.
 */
uint8_t readBit(const uint8_t *addr, size_t &offset, uint8_t &bit_offset, const std::string &message);
/**
 * Reads n bits from the position addr + offset + bit_offset. Increments bit_offset. If a byte border is reached,
 * i.e., bit_offset reaches 8, offset is incremented and bit_offset reset to 0.
 *
 * @warning Do not pass a n value greater than 32.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @param n Number of bits to read.
 * @return The read value.
 */
uint32_t readNBits(const uint8_t *addr, size_t &offset, uint8_t &bit_offset, uint32_t n);
/**
 * Reads n bits from the position addr + offset + bit_offset. Increments bit_offset. If a byte border is reached,
 * i.e., bit_offset reaches 8, offset is incremented and bit_offset reset to 0. Additionally, prints a debug message to
 * stdout if DEBUG is defined.
 *
 * @warning Do not pass a n value greater than 32.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @param n Number of bits to read.
 * @param message Parameter name that is printed together with the read value.
 * @return The read value.
 */
uint32_t readNBits(const uint8_t *addr, size_t &offset, uint8_t &bit_offset, uint32_t n, const std::string &message);
/**
 * Reads a byte from the position addr + offset + bit_offset. Increments offset.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @return The read value.
 */
uint8_t readByte(const uint8_t *addr, size_t &offset, uint8_t &bit_offset);
/**
 * Reads a byte from the position addr + offset + bit_offset. Increments offset. Additionally, prints a debug message to
 * stdout if DEBUG is defined.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @param message Parameter name that is printed together with the read value.
 * @return The read value.
 */
uint8_t readByte(const uint8_t *addr, size_t &offset, uint8_t &bit_offset, const std::string &message);
/**
 * Reads four bytes from the position addr + offset + bit_offset and interprets them as a unsigned integer. Increments
 * offset.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @return The read value as an unsigned integer.
 */
uint32_t readUnsignedInt32(const uint8_t *addr, size_t &offset, uint8_t &bit_offset);
/**
 * Reads four bytes from the position addr + offset + bit_offset and interprets them as a unsigned integer. Increments
 * offset. Additionally, prints a debug message to stdout if DEBUG is defined.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @param message Parameter name that is printed together with the read value.
 * @return The read value as an unsigned integer.
 */
uint32_t readUnsignedInt32(const uint8_t *addr, size_t &offset, uint8_t &bit_offset, const std::string &message);
/**
 * Reads a variable number of bits from the position addr + offset + bit_offset as a Exp-Golomb code. Increments
 * bit_offset. If a byte border is reached, i.e., bit_offset reaches 8, offset is incremented and bit_offset reset to 0
 * .
 * Details about the parsing process can be found in ISO/IEC 14496-10:2014 Chapter 9.1.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @return The code number.
 */
uint32_t decodeUnsignedExpGolomb(const uint8_t *addr, size_t &offset, uint8_t &bit_offset);
/**
 * Reads a variable number of bits from the position addr + offset + bit_offset as a Exp-Golomb code. Increments
 * bit_offset. If a byte border is reached, i.e., bit_offset reaches 8, offset is incremented and bit_offset reset to 0.
 * Additionally, prints a debug message to stdout if DEBUG is defined.
 *
 * Details about the parsing process can be found in ISO/IEC 14496-10:2014 Chapter 9.1.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @param message Parameter name that is printed together with the read value.
 * @return The code number.
 */
uint32_t decodeUnsignedExpGolomb(const uint8_t *addr, size_t &offset, uint8_t &bit_offset, const std::string &message);
/**
 * Reads a variable number of bits from the position addr + offset + bit_offset as a Exp-Golomb code. Increments
 * bit_offset. If a byte border is reached, i.e., bit_offset reaches 8, offset is incremented and bit_offset reset to 0.
 * The unsigned code number is then mapped to a signed syntax element value as specified in ISO/IEC 14496-10:2014
 * Chapter 9.1.1.
 *
 * Details about the parsing process can be found in ISO/IEC 14496-10:2014 Chapter 9.1.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @return The read value.
 */
int32_t decodeSignedExpGolomb(const uint8_t *addr, size_t &offset, uint8_t &bit_offset);
/**
 * Reads a variable number of bits from the position addr + offset + bit_offset as a Exp-Golomb code. Increments
 * bit_offset. If a byte border is reached, i.e., bit_offset reaches 8, offset is incremented and bit_offset reset to 0.
 * The unsigned code number is then mapped to a signed syntax element value as specified in ISO/IEC 14496-10:2014
 * Chapter 9.1.1. Additionally, prints a debug message to stdout if DEBUG is defined.
 *
 * Details about the parsing process can be found in ISO/IEC 14496-10:2014 Chapter 9.1.
 *
 * @param addr Base address.
 * @param offset Byte offset from base address.
 * @param bit_offset Bit offset inside byte.
 * @param message Parameter name that is printed together with the read value.
 * @return The read value.
 */
int32_t decodeSignedExpGolomb(const uint8_t *addr, size_t &offset, uint8_t &bit_offset, const std::string &message);
/**
 * Returns the value of the ChromaArrayType pseudo variable. The variable is derived from the contents of the SPS as
 * follows (taken from the semantic description of the separate_colour_plane_flag field in ISO/IEC 14496-10:2014 Chapter
 * 7.4.2.1.1):
 *
 * "Depending on the value of separate_colour_plane_flag, the value of the variable ChromaArrayType is assigned as
 *  follows:
 *
 *    – If separate_colour_plane_flag is equal to 0, ChromaArrayType is set equal to chroma_format_idc.
 *
 *    – Otherwise (separate_colour_plane_flag is equal to 1), ChromaArrayType is set equal to 0"
 *
 * @param sps SPS from which to derive the value.
 * @return Value of the variable.
 */
uint32_t getChromaArrayType(const SPS &sps);
/**
 * Returns the string representation of a NAL unit type code as specified in ISO/IEC 14496-10:2014 Table 7-1.
 *
 * @param nal_unit_type NAL unit type code.
 * @return String representation.
 */
std::string getNALUnitTypeString(uint8_t nal_unit_type);
std::string getShortNALUnitTypeString(uint8_t nal_unit_type);
/**
 * Returns the string representation of a slice type code as specified in ISO/IEC 14496-10:2014 Table 7-6.
 *
 * @param slice_type Slice type code.
 * @return String representation.
 */
std::string getSliceTypeString(uint8_t slice_type);
/**
 * Interprets the four byte integer as a four character string and returns the value. Used to get the name of a MP4 box
 * as a string representation.
 *
 * @param type Integer representation of the name.
 * @return String representation.
 */
std::string getNameString(uint32_t type);

#endif //HELPER_FUNCTIONS_H_
