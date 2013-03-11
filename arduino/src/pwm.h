#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED

#include <avr/io.h> // for uint16_t

namespace pwm
{
  void init();
  void loadParameters();
  void saveParameters();
  void setDutyCycle(const uint16_t _ch_A, const uint16_t _ch_B);
  void setDefaults(const uint16_t _ICR,
                   const uint16_t _CH_A_DEF, const uint16_t _CH_A_MAX, const uint16_t _CH_A_MIN,
                   const uint16_t _CH_B_DEF, const uint16_t _CH_B_MAX, const uint16_t _CH_B_MIN);
  void resetToDefaults();
}

#endif //PWM_H_INCLUDED
