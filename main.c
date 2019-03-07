#include <stdint.h>
#include <avr/io.h>

#include "TinyTimber.h"
#include "avrprint.h"

//#include "GUI.h"
#include "InputHandler.h"
//#include "PulseGenerator.h"


int main(void) {

    PulseGenerator pG1 = initPulseGenerator(PE6, 0);
    PulseGenerator pG2 = initPulseGenerator(PE4, 4);

    InputHandler inputHandler = initInputHandler(&pG1, &pG2);

    INSTALL(&inputHandler, &handleJoystickInterrupt, IRQ_PCINT0);
    INSTALL(&inputHandler, &handleJoystickInterrupt, IRQ_PCINT1);

    return TINYTIMBER(&inputHandler, &displayInitialData, 0);
}

