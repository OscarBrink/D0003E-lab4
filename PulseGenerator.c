#include "PulseGenerator.h"

//uint8_t debug = 1;

/*
void flipReset(PulseGenerator *this, uint8_t arg) {
    this->reset = !(this->reset);
    if (debug > 9) debug = 0;
    writeChar('0' + debug++, 3);
    ASYNC(&(this->gui), &printData, getData(this, 0));
}
*/

uint8_t incrementFrequency(PulseGenerator *this, uint8_t arg) {

    if ( this->frequency < 99 ) {
        this->frequency++;
        ASYNC(this, &updateGUI, 0);
        if (this->continuousInput == 1)
            AFTER(MSEC(100), this, &incrementFrequency, 0);
        return 1;
    }

    return 0; // No change has happened
}

uint8_t decrementFrequency(PulseGenerator *this, uint8_t arg) {

    if (this->frequency > 0)  {
        this->frequency--;
        ASYNC(this, &updateGUI, 0);
        if (this->continuousInput == 2)
            AFTER(MSEC(100), this, &decrementFrequency, 0);
        return 1;
    }

    return 0; // No change has happened
}

void reset(PulseGenerator *this, uint8_t arg) {
    if (this->frequency == 0) {
        this->frequency = this->oldFrequency;
        this->oldFrequency = 0;
    } else {
        this->oldFrequency = this->frequency;
        this->frequency = 0;
    }

    ASYNC(this, &updateGUI, 0);
}

void updateGUI(PulseGenerator *this, uint8_t changeState) {
    //writeChar('0', 3);
    if (changeState) {
        //writeChar('1', 2);
        ASYNC(&(this->gui), &flipActivationState, 0);
    } else {
        //writeChar('1', 3);
        ASYNC(&(this->gui), &printData, this->frequency);
    }
}

void setState(PulseGenerator *this, uint8_t state) {
    if (state) {
        
    }
}

