#if !defined(__AVR_ATmega328__) && !defined(__AVR_ATmega168__)
#error "Unsupported chip"
#endif

#include "pwm.h"
#include "definitions.h"

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t CH_A_DEF;
uint16_t CH_A_MAX;
uint16_t CH_A_MIN;

uint16_t CH_B_DEF;
uint16_t CH_B_MAX;
uint16_t CH_B_MIN;

namespace pwm
{
  
  void init()
  {
    PORTB &= ~(1<<PORTB1 | 1<<PORTB2);
    DDRB |= 1<<DDB1 | 1<<DDB2;

    loadParameters();

    TCCR1A = 1<<COM1A1 | 1<<COM1B1;
    TCCR1B = 1<<WGM13 | 1<<CS11; // 1<<CS11 launches TC1 (clk/8)
  }
  
  void loadParameters()
  {
    ICR1     = eeprom_read_word(U16P(eeprom::PWM_TOP));

    CH_A_DEF = eeprom_read_word(U16P(eeprom::PWM_CH_A_DEF));
    CH_A_MAX = eeprom_read_word(U16P(eeprom::PWM_CH_A_MAX));
    CH_A_MIN = eeprom_read_word(U16P(eeprom::PWM_CH_A_MIN));

    CH_B_DEF = eeprom_read_word(U16P(eeprom::PWM_CH_B_DEF));
    CH_B_MAX = eeprom_read_word(U16P(eeprom::PWM_CH_B_MAX));
    CH_B_MIN = eeprom_read_word(U16P(eeprom::PWM_CH_B_MIN));

    OCR1A = CH_A_DEF;
    OCR1B = CH_B_DEF;
  }

  void saveParameters()
  {
    eeprom_write_word(U16P(eeprom::PWM_TOP), ICR1);
    
    eeprom_write_word(U16P(eeprom::PWM_CH_A_DEF), CH_A_DEF);
    eeprom_write_word(U16P(eeprom::PWM_CH_A_MAX), CH_A_MAX);
    eeprom_write_word(U16P(eeprom::PWM_CH_A_MIN), CH_A_MIN);
    
    eeprom_write_word(U16P(eeprom::PWM_CH_B_DEF), CH_B_DEF);
    eeprom_write_word(U16P(eeprom::PWM_CH_B_MAX), CH_B_MAX);
    eeprom_write_word(U16P(eeprom::PWM_CH_B_MIN), CH_B_MIN);
  }

  void setDutyCycle(const uint16_t _ch_A, const uint16_t _ch_B)
  {
    if (_ch_A < CH_A_MIN)
    OCR1A = CH_A_MIN;
    else if (_ch_A > CH_A_MAX)
    OCR1A = CH_A_MAX;
    else
    OCR1A = _ch_A;

    if (_ch_B < CH_B_MIN)
    OCR1B = CH_B_MIN;
    else if (_ch_B > CH_B_MAX)
    OCR1B = CH_B_MAX;
    else
    OCR1B = _ch_B;
  }
  
  void setDefaults(const uint16_t _ICR,
  const uint16_t _CH_A_DEF, const uint16_t _CH_A_MAX, const uint16_t _CH_A_MIN,
  const uint16_t _CH_B_DEF, const uint16_t _CH_B_MAX, const uint16_t _CH_B_MIN)
  {
    ICR1 = _ICR;

    CH_A_DEF = _CH_A_DEF;
    CH_A_MAX = _CH_A_MAX;
    CH_A_MIN = _CH_A_MIN;

    CH_B_DEF = _CH_B_DEF;
    CH_B_MAX = _CH_B_MAX;
    CH_B_MIN = _CH_B_MIN;
    
    saveParameters();
  }
  
  void resetToDefaults()
  {
    OCR1A = CH_A_DEF;
    OCR1B = CH_B_DEF;;
  }

} //namespace pwm
