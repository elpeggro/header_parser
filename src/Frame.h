#ifndef HEADER_PARSER_SRC_FRAME_H_
#define HEADER_PARSER_SRC_FRAME_H_

#include <cstdint>
#include <cstddef>
#include <string>
#include <tuple>

class Frame {
 public:
  Frame() = default;
  Frame(char type_, size_t start_, size_t end_) : type(type_), weight(0), start(start_), end(end_) {};
  void setWeight(uint32_t weight_) { weight = weight_; }
  char getType() const { return type; }
  uint32_t getWeight() const { return weight; }
  size_t getSize() const { return end - start + 1; }
  std::string getRange() const { return std::to_string(start) + '-' + std::to_string(end); }
  friend bool operator<(const Frame &l, const Frame &r) {
    if (l.weight > r.weight) {
      return true;
    } else if (l.weight < r.weight) {
      return false;
    } else if (l.start < r.start) {
      return true;
    }
    return false;
  }
 private:
  char type;
  uint32_t weight;
  size_t start;
  size_t end;
};

#endif //HEADER_PARSER_SRC_FRAME_H_
