#include "PulseGenerator.h"


void flipReset(PulseGenerator *this, uint8_t arg) {
    this->reset = !(this->reset);
    ASYNC(&(this->gui), &printData, getData(this, 0));
}

uint8_t incrementFrequency(PulseGenerator *this, uint8_t arg) {

    uint8_t updated = 0;

    if (this->reset) {
        this->frequency = 1;
        this->reset = 0;
        updated = 1;
    } else if ( this->frequency < 99 ) {
        this->frequency++;
        updated = 1;
    }

    if (updated) {
        ASYNC(&(this->gui), &printData, getData(this, 0));
    } 
    return updated;
}

uint8_t decrementFrequency(PulseGenerator *this, uint8_t arg) {

    if ( this->frequency && !(this->reset) ) {
        this->frequency--;
        ASYNC(&(this->gui), &printData, getData(this, 0));
        return 1;
    }

    return 0; // No change has happened
}

uint8_t getData(PulseGenerator *this, uint8_t arg) {
    if (this->reset) {
        return 0;
    } else {
        return this->frequency;
    }
}

