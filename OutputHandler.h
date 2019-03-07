#ifndef OUTPUTHANDLER_H
#define OUTPUTHANDLER_H

#include "TinyTimber.h"

#include <avr/io.h>
#include <stdint.h>

#define MAXELEMENTS 2

typedef struct {
    Object super;
} OutputHandler;

#define initOutputHandler() { initObject() }

void invertPort(OutputHandler *this, uint8_t pin);
void resetPort(OutputHandler *this, uint8_t pin);

#endif
