#include "InputHandler.h"


void handleJoystickInterrupt(InputHandler *this, int arg) {

    if (checkForContinuousInput(this)) return;

    if ( !((PINB>>PINB7) & 1) ) { // Down
        ASYNC(this->elements[this->activeElement], &decrementFrequency, 0);
        this->continuousInputMsg = AFTER(CONT_INPUT_DELAY, this, &startContinuousInput, 1);
    } else if ( !((PINB>>PINB6) & 1) ) { // Up
        ASYNC(this->elements[this->activeElement], &incrementFrequency, 0);
        this->continuousInputMsg = AFTER(CONT_INPUT_DELAY, this, &startContinuousInput, 0);
    } else if ( !((PINB>>PINB4) & 1) ) { // Center
        ASYNC(this->elements[this->activeElement], &reset, 0);
    } else if ( !((PINE>>PINE3) & 1) ) { // Right
        if ( (this->activeElement + 1) < MAXELEMENTS ) {
            ASYNC(this->elements[this->activeElement], &updateGUI, 1);
            ASYNC(this->elements[++this->activeElement], &updateGUI, 1);
        }
    } else if ( !((PINE>>PINE2) & 1) ) { // Left
        if ( (this->activeElement) > 0 ) {
            ASYNC(this->elements[this->activeElement], &updateGUI, 1);
            ASYNC(this->elements[--this->activeElement], &updateGUI, 1);
        }
    }

    return;
}

uint8_t checkForContinuousInput(InputHandler *this) {
    if (this->elements[this->activeElement]->continuousInput == 1 && ((PINB>>PINB7) & 1)) { // Down
        this->elements[this->activeElement]->continuousInput = 0;
        return 1;
    } else if (this->elements[this->activeElement]->continuousInput == 2 && ((PINB>>PINB6) & 1)) { // Up
        this->elements[this->activeElement]->continuousInput = 0;
        return 1;
    } else if (this->elements[this->activeElement]->continuousInput) {
        return 1;
    } else if (this->continuousInputMsg) { // Continuous input scheduled
        ABORT(this->continuousInputMsg);
        this->continuousInputMsg = 0;
    }
    return 0;
}

void startContinuousInput(InputHandler *this, uint8_t direction) {
    if (direction) { // Down
        this->elements[this->activeElement]->continuousInput = 2;
        ASYNC(this->elements[this->activeElement], &decrementFrequency, 0);
    } else { // Up
        this->elements[this->activeElement]->continuousInput = 1;
        ASYNC(this->elements[this->activeElement], &incrementFrequency, 0);
    }
    this->continuousInputMsg = 0;

    return;
}

void displayInitialData(InputHandler *this, uint8_t arg) {
    for (int i = 0; i < MAXELEMENTS; i++) {
        ASYNC(this->elements[i], &updateGUI, 0);
    }

    ASYNC(this->elements[this->activeElement], &updateGUI, 1);
}

