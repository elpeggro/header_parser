#ifndef XMLHANDLER_H_
#define XMLHANDLER_H_

#include <libxml/tree.h>
#include <string>
#include <vector>
#include <cstdint>

/**
 * Helper class that handles all the annoying libxml details for the user. This class is specialized to our use case,
 * i.e., finding a specific BaseURL element and iterating over the SegmentList of that element.
 */
class XmlHandler {
 public:
  XmlHandler();
  ~XmlHandler();
  /**
   * Sets the MPD file and the video name. This function tries to open the MPD file and locate the corresponding BaseURL
   * element. See also flushMPDFile() on how the correct BaseURL element is located. When the BaseURL element is found,
   * the function tries to navigate to the SegmentList element of the current Representation set. When it is successful,
   * the class can be used to add attributes to the SegmentURL elements.
   * @param xml_file Path to the MPD file.
   * @param video_file Name (or prefix) of the video.
   * @return 0 on success. -1 in case an error occurred.
   */
  int32_t setFile(const std::string &xml_file, const std::string &video_file);
  /**
   * Writes the attributes that were added to the MPD file.
   * @return 0 on success. -1 in case an error occurred.
   */
  int32_t save();
  /**
   * Set the internal pointer to the next SegmentURL element, if possible.
   * @return true if pointer was moved. False if end of the SegmentList is reached.
   */
  bool nextSegment();
  /**
   * Add a new attribute with a specified value to the current SegmentURL element. Prints a warning if an existing
   * attribute is overwritten (this can only happen if the program is executed twice for the same video).
   * @param name Name of the attribute.
   * @param value Value of the attribute.
   */
  void addAttribute(const std::string &name, const std::string &value);
  /**
   * Returns the start value of the mediaRange attribute for the current SegmentURL element.
   * @return Range start value.
   */
  uint32_t getRangeStart() { return curr_range_start; }
  /**
   * Returns the end value of the mediaRange attribute for the current SegmentURL element.
   * @return Range end value.
   */
  uint32_t getRangeEnd() { return curr_range_end; }

 private:
  xmlDocPtr doc;
  /// Pointer to the currently selected SegmentURL element. Calls to addAttribute modify this element.
  xmlNodePtr segment_url_node;
  /// MPD file location.
  std::string location;
  uint32_t curr_range_start;
  uint32_t curr_range_end;
  /**
   * Iterates through the siblings of a given node until either a sibling with a matching name is found or until there
   * are no siblings left.
   * @param start Pointer to the node whose siblings should be iterated.
   * @param until_name Name of the node that is searched for.
   */
  void iterateSiblingsUntil(xmlNodePtr &start, const std::string &until_name);
  /**
   * Reads the mediaRange attribute of the current SegmentURL node and updates the internal range fields.
   */
  void updateCurrentRange();

};

#endif //XMLHANDLER_H_
