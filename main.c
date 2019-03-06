#include <stdint.h>
#include <avr/io.h>

#include "TinyTimber.h"
#include "avrprint.h"

//#include "GUI.h"
#include "IOHandler.h"
//#include "PulseGenerator.h"


int main(void) {

    PulseGenerator pG1 = initPulseGenerator(PE6, 0);
    PulseGenerator pG2 = initPulseGenerator(PE4, 4);

    IOHandler ioHandler = initIOHandler(&pG1, &pG2);

    INSTALL(&ioHandler, &handleInput, IRQ_PCINT0);
    INSTALL(&ioHandler, &handleInput, IRQ_PCINT1);

    return TINYTIMBER(&ioHandler, &displayInitialData, 0);
}

