#include "PulseGenerator.h"

#define PULSE_DELAY USEC(250000 / this->frequency) // each step 2 hz

uint8_t debug = 0;

void incrementFrequency(PulseGenerator *this, uint8_t arg) {

    if ( this->frequency < 99 ) {
        
        if (++this->frequency == 1) ASYNC(this, &updatePulse, 0);

        ASYNC(this, &updateGUI, 0);
        if (this->continuousInput == 1)
            AFTER(MSEC(100), this, &incrementFrequency, 0);
    }

    return;
}

void decrementFrequency(PulseGenerator *this, uint8_t arg) {

    if (this->frequency > 0)  {
        this->frequency--;
        ASYNC(this, &updateGUI, 0);
        if (this->continuousInput == 2)
            AFTER(MSEC(100), this, &decrementFrequency, 0);
    }

    return;
}

void reset(PulseGenerator *this, uint8_t arg) {
    if (this->frequency == 0) {
        this->frequency = this->oldFrequency;
        this->oldFrequency = 0;
    } else {
        this->oldFrequency = this->frequency;
        this->frequency = 0;
    }

    if (this->frequency > 0) ASYNC(this, &updatePulse, 0);
    ASYNC(this, &updateGUI, 0);

    return;
}

void updateGUI(PulseGenerator *this, uint8_t changeState) {
    if (changeState) {
        ASYNC(&(this->gui), &flipActivationState, 0);
    } else {
        ASYNC(&(this->gui), &printData, this->frequency);
    }

    return;
}

void updatePulse(PulseGenerator *this, uint8_t arg) {
    if (this->frequency > 0) {
        ASYNC(this->outputHandler, &invertPort, this->pin);
        AFTER(PULSE_DELAY, this, &updatePulse, 0); // Schedule next pulse change
    } else {
        ASYNC(this->outputHandler, &resetPort, this->pin);
    }
}

