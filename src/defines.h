#ifndef DEFINES_H_
#define DEFINES_H_

/*
 * If this flag is defined, very detailed output is printed to stdout. For each parameter that is read, a message of the
 * form:
 *   offset+bit_offset: prameter_name: parameter_value
 * is printed.
 */
//#define DEBUG

// Use this flag to get only basic information as well as the file structure printed to stdout.
//#define INFO

// This macro specifies how the position for the debug message is formatted.
#include <iomanip>
#define POSITION std::setw(8) << std::setfill('0') << std::uppercase << std::hex << offset << "+" << +bit_offset << ": " << std::dec

#endif //DEFINES_H_
