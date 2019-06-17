#ifndef HEADER_PARSE_H_
#define HEADER_PARSE_H_

#include <cstdint>

/**
 * Tries to parse a NAL unit located at addr + offset. Increments the offset in the process. The parsed NAL unit is
 * placed in the nal_units vector.
 * @param addr Base address of the file.
 * @param offset Offset at which the NAL unit is located.
 */
void parseNALUnit(const uint8_t *addr, uint32_t &offset);
/**
 * Tries to parse a sequence parameter set located at addr + offset. Increments the offset in the process. The parsed
 * SPS is placed in the spss vector.
 * @param addr Base address of the file.
 * @param offset Offset at which the SPS is located.
 */
void parseSPS(const uint8_t *addr, uint32_t &offset);
/**
 * Tries to parse a picture parameter set located at addr + offset. Increments the offset in the process. The parsed PPS
 * is placed in the ppss vector.
 * @param addr Base address of the file.
 * @param offset Offset at which the PPS is located.
 */
void parsePPS(const uint8_t *addr, uint32_t &offset);
/**
 * Tries to parse a slice header located at addr + offset. Increments the offset in the process. The parsed slice header
 * is placed in the slices vector.
 * @param addr Base address of the file.
 * @param offset Offset at which the slice header is located.
 */
void parseSliceHeader(const uint8_t *addr, uint32_t &offset);
/**
 * Tries to parse a MP4 box located at addr + offset. Increments the offset in the process. The parsed MP4 box is placed
 * in the mp4_boxes vector. Boxes with type other than 'mdat' are consumed completely. The offset points to the next MP4
 * box. For 'mdat' boxes, only the MP4 header is consumed and the offset points to the contained NAL unit.
 * @param addr Base address of the file.
 * @param offset Offset at which the MP4 box is located.
 */
int32_t parseMP4Box(const uint8_t *addr, uint32_t &offset);

#endif //HEADER_PARSE_H_
