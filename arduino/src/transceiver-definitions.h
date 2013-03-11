#ifndef TRANSCEIVER_DEFINITIONS_H_INCLUDED
#define TRANSCEIVER_DEFINITIONS_H_INCLUDED

#define BUFSIZE 64

#define STARTTIMER  (TCCR0B = 1<<CS02 | 1<<CS00) // launches TC0 (clk/1024)
#define STOPTIMER   (TCCR0B = 0)
#define RESETTIMER  (TCNT0 = 0)
#define OCR0A_VALUE (TRANSCEIVER_TIMEOUT * F_CPU / 1024000)

enum TransceiverState
{
  UNINITIALIZED,
  WAITING,
  RECEIVING
};

#endif //TRANSCEIVER_DEFINITIONS_H_INCLUDED
