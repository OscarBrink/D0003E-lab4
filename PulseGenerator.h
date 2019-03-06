#include <stdint.h>

#include "TinyTimber.h"

#include "GUI.h"


typedef struct {
    Object super;
    uint8_t pin;
    uint8_t frequency;
    uint8_t oldFrequency;
    GUI gui;
} PulseGenerator;

#define initPulseGenerator(pin, screenPos) { initObject(), pin, 0, 0, initGUI(screenPos) }

uint8_t incrementFrequency(PulseGenerator *this, uint8_t arg);
uint8_t decrementFrequency(PulseGenerator *this, uint8_t arg);
void reset(PulseGenerator *this, uint8_t arg);
uint8_t getData(PulseGenerator *this, uint8_t arg);
void updateGUI(PulseGenerator *this, uint8_t changeState);
void setState(PulseGenerator *this, uint8_t state);

