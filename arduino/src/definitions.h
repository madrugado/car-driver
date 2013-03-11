#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#include <avr/eeprom.h>

#define F_CPU 16000000UL

//#define WFC_TIMEOUT 5000 // Waiting For Command timeout, in milliseconds (multiple of 16 ms)

namespace eeprom
{
  enum Addresses
  {
    PWM_TOP      = 0,

    PWM_CH_A_DEF = PWM_TOP + 2,
    PWM_CH_A_MAX = PWM_CH_A_DEF + 2,
    PWM_CH_A_MIN = PWM_CH_A_MAX + 2,

    PWM_CH_B_DEF = PWM_CH_A_MIN + 2,
    PWM_CH_B_MAX = PWM_CH_B_DEF + 2,
    PWM_CH_B_MIN = PWM_CH_B_MAX + 2,

    WFC_TIMEOUT  = PWM_CH_B_MIN + 2
  };
}

#define U8P(x) (uint8_t *)x
#define U16P(x) (uint16_t *)x

#endif //DEFINITIONS_H_INCLUDED
