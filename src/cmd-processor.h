#ifndef CMD_PROCESSOR_H_INCLUDED
#define CMD_PROCESSOR_H_INCLUDED

#include <avr/io.h> // for uint8_t

namespace cmdProcessor
{
  void init(uint16_t _wfc_timeout); // _wfc_timeout is "Waiting For Command" timeout, in milliseconds (multiple of 16 ms)
  void processCommand(const uint8_t _buf[], const uint8_t _size);
}

#endif //CMD_PROCESSOR_H_INCLUDED
