#include <stdint.h>
#include <avr/io.h>

#include "TinyTimber.h"
#include "avrprint.h"

//#include "GUI.h"
#include "PulseGenerator.h"

#define MAXELEMENTS 2

typedef struct {
    Object super;
    PulseGenerator *elements[MAXELEMENTS];
    uint8_t activeElement;
} IOHandler;

#define initIOHandler(elem1, elem2) { initObject(), { elem1, elem2 }, 0 }

uint8_t handleInput(IOHandler *this, int arg);
void doNothing(IOHandler *this, int arg);

uint8_t iCtr = 0;

int main(void) {

    PulseGenerator pG1 = initPulseGenerator(PINE6, 0);
    PulseGenerator pG2 = initPulseGenerator(PINE4, 4);

    IOHandler ioH = initIOHandler(&pG1, &pG2);

    INSTALL(&ioH, &handleInput, IRQ_PCINT0);
    INSTALL(&ioH, &handleInput, IRQ_PCINT1);


    return TINYTIMBER(&pG1, &doNothing, 0);
}

uint8_t handleInput(IOHandler *this, int arg) {


    if ( !(PINB & (1<<PINB7)) ) { // Down
        //writeLong(777777);
        SYNC(this->elements[this->activeElement], &decrementFrequency, 0);
    }
    else if ( !(PINB & (1<<PINB6)) ) { // Up
        //writeLong(666666);
        SYNC(this->elements[this->activeElement], &incrementFrequency, 0);
    }
    else if ( !(PINB & (1<<PINB4)) ) { // Center
        SYNC(this->elements[this->activeElement], &flipReset, 0);
        //writeLong(444444);
    }
    else if ( !(PINE & (1<<PINE3)) ) { // Right
        if ( (this->activeElement + 1) < MAXELEMENTS ) {
            this->activeElement++;
        }
        //writeLong(333333);
    }
    else if ( !(PINE & (1<<PINE2)) ) {// Left
        if ( (this->activeElement) > 0 ) {
            this->activeElement--;
        }
        //writeLong(222222);
    }

    // TODO debug
    if (this->elements[this->activeElement]->gui.screenPos == 0) {
        writeChar('0' + this->elements[this->activeElement]->reset, 2);
    } /*else {
        writeChar('0' + this->elements[this->activeElement]->reset, 3);
    }*/
    writeChar('0' + ++iCtr, 3);

    return 0;
}

void doNothing(IOHandler *this, int arg) {}
