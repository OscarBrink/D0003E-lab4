#include "OutputHandler.h"

#define PULSE_DELAY USEC(250000 / this->elements[i]->frequency) // each step 2 hz

void invertPort(OutputHandler *this, uint8_t pin) {
    PORTE ^= (1<<pin);
}

void resetPort(OutputHandler *this, uint8_t pin) {
    PORTE &= ~(1<<pin);
}

