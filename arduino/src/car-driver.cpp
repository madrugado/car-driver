#include "definitions.h"
#include "pwm.h"
#include "transceiver.h"
#include "cmd-processor.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

inline void init()
{
  uint16_t wfc_timeout = eeprom_read_word(U16P(eeprom::WFC_TIMEOUT));
  cmdProcessor::init(wfc_timeout);
  pwm::init();
  transceiver::init(cmdProcessor::processCommand);

  sei();
}

int main()
{
  init();

  DDRB |= 1<<DDB5;

  while (true)
  {
    _delay_ms(15);
      
    //if (PORTB & 1<<PORTB5)
    //PORTB ^= 1<<PORTB5;
    //else
    //PORTB |= 1<<PORTB5;
  }
}
