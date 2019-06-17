#include "XmlHandler.h"
#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <regex>

XmlHandler::XmlHandler() {
  xmlInitParser();
  LIBXML_TEST_VERSION
  xmlKeepBlanksDefault(0);
  doc = nullptr;
  segment_url_node = nullptr;
  curr_range_start = 0;
  curr_range_end = 0;
}

XmlHandler::~XmlHandler() {
  if (doc != nullptr) {
    xmlFreeDoc(doc);
  }
  xmlCleanupParser();
}

int32_t XmlHandler::setFile(const std::string &xml_file, const std::string &video_file) {
  std::regex regex(".*dash");
  std::smatch regex_match;
  if (!std::regex_search(video_file, regex_match, regex)) {
    std::cerr << "Failed to find prefix .*dash in video file name. Can not locate BaseURL.\n";
    return -1;
  }
  std::string base_url_prefix = regex_match[0];
  location = xml_file;
  struct stat st{};
  int32_t res = stat(xml_file.c_str(), &st);
  if (res < 0) {
    std::cerr << "Something is wrong with the xml file: " << strerror(errno) << "\n";
    return -1;
  }
  doc = xmlParseFile(xml_file.c_str());
  if (doc == nullptr) {
    std::cerr << "Unable to parse xml file\n";
    return -1;
  }
  xmlXPathContextPtr xpath_ctx = xmlXPathNewContext(doc);
  if (xpath_ctx == nullptr) {
    std::cerr << "Unable to create xpath context\n";
    return -1;
  }
  if (xmlXPathRegisterNs(xpath_ctx, BAD_CAST "mpd", BAD_CAST "urn:mpeg:dash:schema:mpd:2011") < 0) {
    std::cerr << "Failed to register namespace\n";
    xmlXPathFreeContext(xpath_ctx);
    return -1;
  }
  std::string xpath_expr = "//mpd:BaseURL";
  xmlXPathObjectPtr xpath_obj = xmlXPathEval(BAD_CAST xpath_expr.c_str(), xpath_ctx);
  if (xpath_obj == nullptr) {
    std::cerr << "Unable to evaluate xpath expression\n";
    xmlXPathFreeContext(xpath_ctx);
    return -1;
  }
  if (xpath_obj->nodesetval->nodeNr == 0) {
    std::cerr << "Unable to find BaseURL nodes\n";
    xmlXPathFreeObject(xpath_obj);
    xmlXPathFreeContext(xpath_ctx);
    return -1;
  }
  xmlNodePtr navigation_node = nullptr;
  for (int32_t i = 0; i < xpath_obj->nodesetval->nodeNr; i++) {
    xmlNodePtr next = xpath_obj->nodesetval->nodeTab[i];
    auto *value = (char *) (xmlNodeGetContent(next));
    if (strncmp(base_url_prefix.c_str(), value, base_url_prefix.size()) == 0) {
      navigation_node = next;
      break;
    }
  }
  xmlXPathFreeObject(xpath_obj);
  xmlXPathFreeContext(xpath_ctx);
  if (navigation_node == nullptr) {
    std::cerr << "Could not find BaseURL node with matching value: " << base_url_prefix << "\n";
    return -1;
  }
  iterateSiblingsUntil(navigation_node, "SegmentList");
  if (navigation_node == nullptr) {
    std::cerr << "Could not find SegmentList node\n";
    return -1;
  }
  if (navigation_node->children == nullptr) {
    std::cerr << "SegmentList node has no child\n";
    return -1;
  }
  navigation_node = navigation_node->children;
  iterateSiblingsUntil(navigation_node, "SegmentURL");
  if (navigation_node == nullptr) {
    std::cerr << "Could not find SegmentURL node\n";
    return -1;
  }
  segment_url_node = navigation_node;
  updateCurrentRange();
  return 0;
}

int32_t XmlHandler::save() {
  if (doc == nullptr) {
    std::cerr << "save called on invalid file or before setFile was called\n";
    return -1;
  }
  return xmlSaveFormatFileEnc(location.c_str(), doc, "UTF-8", 1);
}

bool XmlHandler::nextSegment() {
  if (segment_url_node->next == nullptr) {
    return false;
  }
  segment_url_node = segment_url_node->next;
  updateCurrentRange();
  return true;
}

void XmlHandler::addAttribute(const std::string &name, const std::string &value) {
  if (xmlHasProp(segment_url_node, BAD_CAST name.c_str()) != nullptr) {
    std::cerr << "Warning: overwriting existing attribute\n";
    xmlSetProp(segment_url_node, BAD_CAST name.c_str(), BAD_CAST value.c_str());
  } else {
    xmlNewProp(segment_url_node, BAD_CAST name.c_str(), BAD_CAST value.c_str());
  }
}

void XmlHandler::iterateSiblingsUntil(xmlNodePtr &start, const std::string &until_name) {
  while (start != nullptr) {
    if (strcmp((char *) start->name, until_name.c_str()) == 0) {
      return;
    }
    start = start->next;
  }
}

void XmlHandler::updateCurrentRange() {
  xmlChar *media_range = xmlGetProp(segment_url_node, BAD_CAST "mediaRange");
  if (media_range == nullptr) {
    std::cerr << "SegmentURL node does not have mediaRange property. Should not happen.\n";
    return;
  }
  std::stringstream ss(std::string((char *) media_range));
  std::string start_token;
  std::string end_token;
  if (!getline(ss, start_token, '-')) {
    std::cerr << "Could not read range start\n";
    return;
  }
  if (!getline(ss, end_token)) {
    std::cerr << "Could not read range end\n";
    return;
  }
  curr_range_start = stoul(start_token);
  curr_range_end = stoul(end_token);
}
