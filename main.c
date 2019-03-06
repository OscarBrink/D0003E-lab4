#include <stdint.h>
#include <avr/io.h>

#include "TinyTimber.h"
#include "avrprint.h"

//#include "GUI.h"
#include "PulseGenerator.h"

#define MAXELEMENTS 2

#define CONT_INPUT_DELAY MSEC(500)

#define JOYC 7
#define JOYU 6
#define JOYD 5
#define JOYL 4
#define JOYR 3

uint8_t debug = 0;

typedef struct {
    Object super;
    PulseGenerator *elements[MAXELEMENTS];
    uint8_t activeElement;
    uint8_t joyStatus;
    uint8_t continuousInputWait;
    Msg continuousInputMsg;
} IOHandler;

#define initIOHandler(elem1, elem2) { initObject(), { elem1, elem2 }, 0, 0xF8, 0, 0 }

void joystick(IOHandler *this, int direction);
uint8_t handleInput(IOHandler *this, int arg);
void startContinuousInput(IOHandler *this, uint8_t direction);
void displayInitialData(IOHandler *this, int arg);

uint8_t iCtr = 0;

int main(void) {

    PulseGenerator pG1 = initPulseGenerator(PINE6, 0);
    PulseGenerator pG2 = initPulseGenerator(PINE4, 4);

    IOHandler ioHandler = initIOHandler(&pG1, &pG2);

    INSTALL(&ioHandler, &handleInput, IRQ_PCINT0);
    INSTALL(&ioHandler, &handleInput, IRQ_PCINT1);

    return TINYTIMBER(&ioHandler, &displayInitialData, 0);
}

void printNum(uint8_t pos, uint8_t data);
void printNum(uint8_t pos, uint8_t data) {
    uint8_t pp = pos;
    writeChar( (data % 100) / 10 + '0', pp);
    pp++;
    writeChar( data % 10 + '0', pp);
}

uint8_t handleInput(IOHandler *this, int arg) {

    writeChar('0' + (PINB>>PINB4 & 1), 3);
    writeChar('0' + this->elements[this->activeElement]->continuousInput, 2);

    if (this->elements[this->activeElement]->continuousInput == 1 && ((PINB>>PINB7) & 1)) { // Down
        this->elements[this->activeElement]->continuousInput = 0;
        return 0;
    } else if (this->elements[this->activeElement]->continuousInput == 2 && ((PINB>>PINB6) & 1)) { // Up
        this->elements[this->activeElement]->continuousInput = 0;
        return 0;
    } else if (this->elements[this->activeElement]->continuousInput) {
        return 1;
    } else if (this->continuousInputWait) { // continuous input scheduled
        ABORT(this->continuousInputMsg);
        this->continuousInputWait = 0;
    }


    if ( !((PINB>>PINB7) & 1) ) { // Down
        ASYNC(this->elements[this->activeElement], &decrementFrequency, 0);
        this->continuousInputWait = 1;
        this->continuousInputMsg = AFTER(CONT_INPUT_DELAY, this, &startContinuousInput, 1);
    } else if ( !((PINB>>PINB6) & 1) ) { // Up
        ASYNC(this->elements[this->activeElement], &incrementFrequency, 0);
        this->continuousInputWait = 1;
        this->continuousInputMsg = AFTER(CONT_INPUT_DELAY, this, &startContinuousInput, 0);
    } else if ( !((PINB>>PINB4) & 1) ) { // Center
        writeChar('0' + (PINB>>PINB4 & 1), 2);
        ASYNC(this->elements[this->activeElement], &reset, 0);
    } else if ( !((PINE>>PINE3) & 1) ) { // Right
        //writeChar('0', 3);
        if ( (this->activeElement + 1) < MAXELEMENTS ) {
            ASYNC(this->elements[this->activeElement], &updateGUI, 1);
            ASYNC(this->elements[++this->activeElement], &updateGUI, 1);
        }
    } else if ( !((PINE>>PINE2) & 1) ) { // Left
        //writeChar('1', 3);
        if ( (this->activeElement) > 0 ) {
            ASYNC(this->elements[this->activeElement], &updateGUI, 1);
            ASYNC(this->elements[--this->activeElement], &updateGUI, 1);
        }
    } /*else if (PINB & (1<<PINB4)) {
        this->joyStatus |= (1<<JOYC);
        return 1;
    }*/

    return 0;

}

void startContinuousInput(IOHandler *this, uint8_t direction) {
    if (direction) { // Down
        this->elements[this->activeElement]->continuousInput = 2;
        ASYNC(this->elements[this->activeElement], &decrementFrequency, 0);
    } else { // Up
        this->elements[this->activeElement]->continuousInput = 1;
        ASYNC(this->elements[this->activeElement], &incrementFrequency, 0);
    }
    this->continuousInputWait = 0;

}

void displayInitialData(IOHandler *this, int arg) {
    for (int i = 0; i < MAXELEMENTS; i++) {
        ASYNC(this->elements[i], &updateGUI, 0);
    }

    ASYNC(this->elements[this->activeElement], &updateGUI, 1);
}

