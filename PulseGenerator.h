#ifndef PULSEGENERATOR_H
#define PULSEGENERATOR_H
#include <stdint.h>

#include "TinyTimber.h"

#include "OutputHandler.h"
#include "GUI.h"

typedef struct {
    Object super;
    OutputHandler *outputHandler;
    uint8_t pin;
    uint8_t frequency;
    uint8_t oldFrequency;
    uint8_t continuousInput; // 0 : no input, 1 : increment, 2 : decrement
    GUI gui;
} PulseGenerator;

#define initPulseGenerator(outputHandler, pin, screenPos) { initObject(), outputHandler, pin, 0, 0, 0, initGUI(screenPos) }

void incrementFrequency(PulseGenerator *this, uint8_t arg);
void decrementFrequency(PulseGenerator *this, uint8_t arg);
void reset(PulseGenerator *this, uint8_t arg);
void getData(PulseGenerator *this, uint8_t arg);
void updateGUI(PulseGenerator *this, uint8_t changeState);
void updatePulse(PulseGenerator *this, uint8_t arg);

#endif
