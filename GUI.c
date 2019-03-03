#include "GUI.h"

void flipActivationState(GUI *this, uint8_t arg) {

    LCDDR0 ^= 0x2;
    LCDDR3 ^= 0x1;
}


void printData(GUI *this, uint8_t data) {
    uint8_t pp = this->screenPos;
    writeChar( (data % 100) / 10 + '0', pp);
    pp++;
    writeChar( data % 10 + '0', pp);
}

