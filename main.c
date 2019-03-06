#include <stdint.h>
#include <avr/io.h>

#include "TinyTimber.h"
#include "avrprint.h"

//#include "GUI.h"
#include "PulseGenerator.h"

#define MAXELEMENTS 2

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
    uint8_t handlingInput;
    uint8_t joyStatus;
} IOHandler;

#define initIOHandler(elem1, elem2) { initObject(), { elem1, elem2 }, 0, 0, 0xF8 }

void joystick(IOHandler *this, int direction);
uint8_t handleInput(IOHandler *this, int arg);
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

//    writeChar('0' + (PINB>>PINB4 & 1), 4);
//    writeChar('0' + (this->joyStatus>>JOYC & 1), 5);

//    if (debug) {
//        debug = 0;
//        return 1;
//    }

    if ( !(PINB & (1<<PINB7)) ) { // Down
        ASYNC(this->elements[this->activeElement], &decrementFrequency, 0);
    } else if ( !(PINB & (1<<PINB6)) ) { // Up
        ASYNC(this->elements[this->activeElement], &incrementFrequency, 0);
    } else if ( !(PINB & (1<<PINB4)) ) { // Center
        debug = 1;
        ASYNC(this->elements[this->activeElement], &reset, 0);
    } else if ( !(PINE & (1<<PINE3)) ) { // Right
        //writeChar('0', 3);
        if ( (this->activeElement + 1) < MAXELEMENTS ) {
            ASYNC(this->elements[this->activeElement], &updateGUI, 1);
            ASYNC(this->elements[++this->activeElement], &updateGUI, 1);
        }
    } else if ( !(PINE & (1<<PINE2)) ) { // Left
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

void displayInitialData(IOHandler *this, int arg) {
    for (int i = 0; i < MAXELEMENTS; i++) {
        ASYNC(this->elements[i], &updateGUI, 0);
    }

    ASYNC(this->elements[this->activeElement], &updateGUI, 1);
}

