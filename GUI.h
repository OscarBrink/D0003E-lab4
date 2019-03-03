#include <stdint.h>

#include "TinyTimber.h"
#include "avrprint.h"
#include <avr/io.h>

typedef struct {
    Object super;
    uint8_t screenPos;
} GUI;

#define initGUI(screenPos) { initObject(), screenPos }

void flipActivationState(GUI *this, uint8_t arg);
void printData(GUI *this, uint8_t data);

