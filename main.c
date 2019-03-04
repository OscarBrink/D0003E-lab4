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
void doNothing(IOHandler *this, int arg);

uint8_t iCtr = 0;

int main(void) {

    PulseGenerator pG1 = initPulseGenerator(PINE6, 0);
    PulseGenerator pG2 = initPulseGenerator(PINE4, 4);

    IOHandler ioHandler = initIOHandler(&pG1, &pG2);

    INSTALL(&ioHandler, &handleInput, IRQ_PCINT0);
    INSTALL(&ioHandler, &handleInput, IRQ_PCINT1);


    return TINYTIMBER(&pG1, &doNothing, 0);
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
    
    if ( (PINB>>PINB6 & 1) ^ (this->joyStatus>>JOYU & 1) ) { // Up
        this->joyStatus ^= (1<<JOYU);
        joystick(this, JOYU);
    } else if ( (PINB>>PINB7 & 1) ^ (this->joyStatus>>JOYD & 1) ) { // Down
        this->joyStatus ^= (1<<JOYD);
        joystick(this, JOYD);
    } else if ( (PINE>>PINE2 & 1) ^ (this->joyStatus>>JOYL & 1) ) { // Left
        this->joyStatus ^= (1<<JOYL);
        joystick(this, JOYL);
    } else if ( (PINE>>PINE3 & 1) ^ (this->joyStatus>>JOYR & 1) ) { // Right
        this->joyStatus ^= (1<<JOYR);
        joystick(this, JOYR);
    } else if ( (PINB>>PINB4 & 1) ^ (this->joyStatus>>JOYC & 1) ) { // Center
        this->joyStatus ^= (1<<JOYC);
        writeChar('0' + (this->joyStatus>>JOYC & 1), 4);
        joystick(this, JOYC);
        //writeChar('0' + (PINB>>PINB4 & 1), 4);
        //writeChar('0' + (this->joyStatus>>JOYC & 1), 5);
    }

/*
    if ( !(PINB & (1<<PINB7)) ) { // Down
        SYNC(this->elements[this->activeElement], &decrementFrequency, 0);
    } else if ( !(PINB & (1<<PINB6)) ) { // Up
        SYNC(this->elements[this->activeElement], &incrementFrequency, 0);
    } else if ( !(PINB & (1<<PINB4)) ) { // Center
        SYNC(this->elements[this->activeElement], &flipReset, 0);
    } else if ( !(PINE & (1<<PINE3)) ) { // Right
        if ( (this->activeElement + 1) < MAXELEMENTS ) {
            if (PINE & (1<<PINE3))
                writeChar('9', 5);
            this->activeElement++;
        }
    } else if ( !(PINE & (1<<PINE2)) ) { // Left
        if ( (this->activeElement) > 0 ) {
            this->activeElement--;
        }
    }

*/

    // TODO debug remove
    if (this->elements[this->activeElement]->gui.screenPos == 0) {
        writeChar('0' + this->elements[this->activeElement]->reset, 2);
    } /*else {
        writeChar('0' + this->elements[this->activeElement]->reset, 3);
    }*/

    this->handlingInput = 0;

    return 0;
}

void joystick(IOHandler *this, int direction) {

    // Zero if pressed, non-zero otherwise
    uint8_t status = this->joyStatus & (1<<direction);
    writeChar('0' + (status>>direction), 5);
    if (status>>direction > 1) writeChar('9', 5);

    switch (direction) {
        case JOYU:
            if (!status) { 
                ASYNC(this->elements[this->activeElement], &incrementFrequency, 0);
            } else {
            }
            break;
        case JOYD:
            if (!status) { 
                ASYNC(this->elements[this->activeElement], &decrementFrequency, 0);
            } else {
            }
            break;
        case JOYL:
            if (!status) {
                if ( (this->activeElement) > 0 ) this->activeElement--;
            }
            break;
        case JOYR:
            if (!status) {
                if ( (this->activeElement + 1) < MAXELEMENTS ) this->activeElement++;
            }
            break;
        case JOYC:
            if (!status) {
                ASYNC(this->elements[this->activeElement], &flipReset, 0);
            }
            break;
    }
}


void doNothing(IOHandler *this, int arg) {}


