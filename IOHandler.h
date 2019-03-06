#include "TinyTimber.h"
#include "PulseGenerator.h"

#define MAXELEMENTS 2

#define CONT_INPUT_DELAY MSEC(500)

typedef struct {
    Object super;
    PulseGenerator *elements[MAXELEMENTS];
    uint8_t activeElement;
    uint8_t joyStatus;
    Msg continuousInputMsg;
} IOHandler;

#define initIOHandler(elem1, elem2) { initObject(), { elem1, elem2 }, 0, 0xF8, 0 }

// Methods
void joystick(IOHandler *this, int direction);
void handleInput(IOHandler *this, int arg);
void startContinuousInput(IOHandler *this, uint8_t direction);
void displayInitialData(IOHandler *this, uint8_t arg);

// Helper functions
uint8_t checkForContinuousInput(IOHandler *this);
