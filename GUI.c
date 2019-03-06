#include "GUI.h"

void flipActivationState(GUI *this, uint8_t arg) {
    if (this->screenPos == 0) {
        LCDDR0 ^= 0x20;
    } else if (this->screenPos == 4) {
        LCDDR1 ^= 0x40;
    }
}

void printData(GUI *this, uint8_t data) {
    uint8_t pp = this->screenPos;
    writeChar( (data % 100) / 10 + '0', pp);
    pp++;
    writeChar( data % 10 + '0', pp);
}

