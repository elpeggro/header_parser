#ifndef HEADER_PARSE_H_
#define HEADER_PARSE_H_

#include <cstdint>
#include <string>
#include <vector>
#include "Frame.h"
/**
 * Tries to parse a NAL unit located at addr + offset. Increments the offset in the process. The parsed NAL unit is
 * placed in the nal_units vector.
 * @param addr Base address of the file.
 * @param offset Offset at which the NAL unit is located.
 */
void parseNALUnit(const uint8_t *addr, size_t &offset);
/**
 * Tries to parse a sequence parameter set located at addr + offset. Increments the offset in the process. The parsed
 * SPS is placed in the spss vector.
 * @param addr Base address of the file.
 * @param offset Offset at which the SPS is located.
 */
void parseSPS(const uint8_t *addr, size_t &offset);
/**
 * Tries to parse a picture parameter set located at addr + offset. Increments the offset in the process. The parsed PPS
 * is placed in the ppss vector.
 * @param addr Base address of the file.
 * @param offset Offset at which the PPS is located.
 */
void parsePPS(const uint8_t *addr, size_t &offset);
/**
 * Tries to parse a slice header located at addr + offset. Increments the offset in the process. The parsed slice header
 * is placed in the slices vector.
 * @param addr Base address of the file.
 * @param offset Offset at which the slice header is located.
 */
void parseSliceHeader(const uint8_t *addr, size_t &offset);
/**
 * Tries to parse a MP4 box located at addr + offset. Increments the offset in the process. The parsed MP4 box is placed
 * in the mp4_boxes vector. Boxes with type other than 'mdat' are consumed completely. The offset points to the next MP4
 * box. For 'mdat' boxes, only the MP4 header is consumed and the offset points to the contained NAL unit.
 * @param addr Base address of the file.
 * @param offset Offset at which the MP4 box is located.
 */
int32_t parseMP4Box(const uint8_t *addr, size_t &offset);
/**
 * Adds the header information that was gathered during the parsing process to the specified MPD file. Note that the
 * XmlHandler class requires a video name that contains *dash somewhere. The reason for this is that our naming suffixes
 * are not always the same, so if the BaseURL element of the MPD has the value
 * bbb_720p_2k35_24f_96sc_300s_dashinit_with_http_header.mp4 it is okay to pass the name
 * bbb_720p_2k35_24f_96sc_300s_dashinit.mp4 to the function, because it compares only up until the *dash keyword.
 * @param file_name Path to the MPD file.
 * @param video_name Video name that should be searched for in the MPD's BaseURL element.
 */
void flushMPDFile(const std::string &file_name, std::string video_name);
void flushMPDFile(const std::string &file_name, std::string video_name, const std::string &weight_file_prefix);

/**
 * Flushes the structure of the bytestream into a CSV file. The file is created at the same location as the video.
 * @param video_name Path to the video file
 */
void flushRanges(const std::string &video_name);
void assignWeights(const std::string &weight_file_prefix, uint32_t segment_no, std::vector<Frame> &frame_list);
#endif //HEADER_PARSE_H_
