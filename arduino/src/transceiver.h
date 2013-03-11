#ifndef TRANSCEIVER_H_INCLUDED
#define TRANSCEIVER_H_INCLUDED

#include <avr/io.h>

#define BAUD 38400 // 76800 Ubuntu on BeagleBone cannot set this speed
#define TRANSCEIVER_TIMEOUT 16 // ms 1..16

typedef void (*TransceiverCallback)(const uint8_t _buf[], const uint8_t _size);

namespace transceiver
{
  void init(TransceiverCallback _transceiverCallback);
  void sendChar(const char _c);
}

#endif //TRANSCEIVER_H_INCLUDED
