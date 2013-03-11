#if !defined(__AVR_ATmega328__) && !defined(__AVR_ATmega168__)
#error "Unsupported chip"
#endif

#include "cmd-processor.h"
#include "definitions.h"
#include "commands.h"
#include "pwm.h"
#include "transceiver.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define STARTTIMER  (TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20) // launches TC2 (clk/1024)
#define STOPTIMER   (TCCR2B = 0)
#define RESETTIMER  {TCNT2 = 0; T2_overflow_cnt = 0;}

static uint16_t T2_overflow_cnt;
static uint16_t T2_overflow_max;

ISR(TIMER2_OVF_vect) // Timeout Expired
{
  T2_overflow_cnt++;
  
  if (T2_overflow_cnt >= T2_overflow_max)
  {
    STOPTIMER;
    RESETTIMER;
    pwm::resetToDefaults();
  }
}

namespace cmdProcessor
{

  void init(uint16_t _wfc_timeout)
  {
    TIMSK2 = 1<<TOIE2;

    RESETTIMER;
    T2_overflow_max = _wfc_timeout>>4; // some hack
  }
  
  void processCommand(const uint8_t _buf[], const uint8_t _size)
  {
    deviceCommand::Command cmd = (deviceCommand::Command)(_buf[0]);
    switch (cmd)
    {
      case deviceCommand::SET_DUTY_CYCLE:
        if (_size == 2 * sizeof(uint16_t) + 1)
        {
          uint16_t ch_A = *(uint16_t *)&_buf[1];
          uint16_t ch_B = *(uint16_t *)&_buf[3];
          
          // Debug
          if (ch_A == 1500)
          {
            if (PORTB & 1<<PORTB5)
            PORTB ^= 1<<PORTB5;
            else
            PORTB |= 1<<PORTB5;
          }

          RESETTIMER;
          STARTTIMER;
          pwm::setDutyCycle(ch_A, ch_B);
          transceiver::sendChar(deviceRetCode::OK);
        }
        else
        {
          transceiver::sendChar(deviceRetCode::BAD_ARGS);
        }
        break;
      case deviceCommand::SET_DEFAULTS:
        if (_size == 7 * sizeof(uint16_t) + 1)
        {       
          uint16_t ICR      = *(uint16_t *)&_buf[1];
          uint16_t OCRA     = *(uint16_t *)&_buf[3];
          uint16_t CH_A_MAX = *(uint16_t *)&_buf[5];
          uint16_t CH_A_MIN = *(uint16_t *)&_buf[7];
          uint16_t OCRB     = *(uint16_t *)&_buf[9];
          uint16_t CH_B_MAX = *(uint16_t *)&_buf[11];
          uint16_t CH_B_MIN = *(uint16_t *)&_buf[13];
        
          pwm::setDefaults(ICR, OCRA, CH_A_MAX, CH_A_MIN, OCRB, CH_B_MAX, CH_B_MIN);
          transceiver::sendChar(deviceRetCode::OK);
        
          // Debug
          if (PORTB & 1<<PORTB5)
          PORTB ^= 1<<PORTB5;
          else
          PORTB |= 1<<PORTB5;
        }
        else
        {
          transceiver::sendChar(deviceRetCode::BAD_ARGS);
        }
        break;
      case deviceCommand::SET_WFC_TIMEOUT:
        if (_size == sizeof(uint16_t) + 1)
        {
          STOPTIMER;
          RESETTIMER;
          pwm::resetToDefaults();
          uint16_t wfc_timeout = *(uint16_t *)&_buf[1];
          eeprom_write_word(U16P(eeprom::WFC_TIMEOUT), wfc_timeout);
          T2_overflow_max = wfc_timeout>>4; // some hack
          transceiver::sendChar(deviceRetCode::OK);
        }
        else
        {
          transceiver::sendChar(deviceRetCode::BAD_ARGS);
        }
        break;
      case deviceCommand::RESET_WFC_TIMER:
        if (_size == 1)
        {
          RESETTIMER;
          STARTTIMER;
          transceiver::sendChar(deviceRetCode::OK);
        }
        else
        {
          transceiver::sendChar(deviceRetCode::BAD_ARGS);
        }
        break;
      default:
        transceiver::sendChar(deviceRetCode::UNKNOWN_COMMAND);
    }
  }

} //namespace cmdProcessor
