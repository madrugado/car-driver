#if !defined(__AVR_ATmega328__) && !defined(__AVR_ATmega168__)
#error "Unsupported chip"
#endif

#include "transceiver.h"
#include "transceiver-definitions.h"
#include "definitions.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>

static TransceiverState transceiverState = UNINITIALIZED;
uint8_t expectedBytesReceived;
uint8_t bytesReceived;
uint8_t buf[BUFSIZE];

TransceiverCallback transceiverCallback;

ISR(TIMER0_COMPA_vect) // Timeout Expired
{
  STOPTIMER;
  transceiverState = WAITING;
  expectedBytesReceived = 0;
  bytesReceived = 0;
}

ISR(USART_RX_vect)
{
  uint8_t receivedValue = UDR0;
  
  // Debug
  //receivedValue -= 48;

  if (transceiverState == WAITING)
  { 
    if (receivedValue > 0 && receivedValue <= BUFSIZE)
    {      
      expectedBytesReceived = receivedValue;
      transceiverState = RECEIVING;
      STARTTIMER;
    }
  }

  else // RECEIVING
  {
    if (bytesReceived < expectedBytesReceived)
    {
      RESETTIMER;
      buf[bytesReceived] = receivedValue;
      bytesReceived++;
      if (bytesReceived == expectedBytesReceived)
      {
        STOPTIMER;
        transceiverCallback(buf, bytesReceived);
        transceiverState = WAITING;
        expectedBytesReceived = 0;
        bytesReceived = 0;
      }
    }
  }
}

ISR(USART_UDRE_vect)
{
  // ToDo: some code
}

ISR(USART_TX_vect)
{
  // ToDo: some code
}

namespace transceiver
{
  void init(TransceiverCallback _transceiverCallback)
  {
    transceiverCallback = _transceiverCallback;
    transceiverState = WAITING;
    expectedBytesReceived = 0;
    bytesReceived = 0;

    // Timer/Counter0
    OCR0A = OCR0A_VALUE;
    TIMSK0 = 1 << OCIE0A;
    TCCR0A = 1<<WGM01; // CTC mode

    // USART0
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    UCSR0B = 1<<TXEN0 | 1<<RXEN0 | 1<<UDRIE0 | 1<<TXCIE0 | 1<<RXCIE0;
    UCSR0C = 1<<UCSZ01 | 1<<UCSZ00; // 8-bit, 1 stop bit, no parity, asynchronous UART
  }
  
  void sendChar(const char _c)
  {
    UDR0 = _c;
  }

} //namespace transceiver
