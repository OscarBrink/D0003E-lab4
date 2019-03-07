#include <stdint.h>

#include "TinyTimber.h"

#include "GUI.h"

typedef struct {
    Object super;
    uint8_t pin;
    uint8_t frequency;
    uint8_t oldFrequency;
    uint8_t continuousInput; // 0 : no input, 1 : increment, 2 : decrement
    GUI gui;
} PulseGenerator;

#define initPulseGenerator(pin, screenPos) { initObject(), pin, 0, 0, 0, initGUI(screenPos) }

void incrementFrequency(PulseGenerator *this, uint8_t arg);
void decrementFrequency(PulseGenerator *this, uint8_t arg);
void reset(PulseGenerator *this, uint8_t arg);
void getData(PulseGenerator *this, uint8_t arg);
void updateGUI(PulseGenerator *this, uint8_t changeState);
void updatePulse(PulseGenerator *this, uint8_t arg);

