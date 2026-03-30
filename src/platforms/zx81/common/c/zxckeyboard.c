#include "platforms/zx81/common/c/zxckeyboard.h"

static uint8_t keyStates[9];


typedef struct {
    uint8_t key;
    uint8_t row;
    uint8_t mask;
} KeyInfo;


// BUGWARN: This table is incomplete
// (and nothing beyond 1-8 / A-G has been properly tested)
static const KeyInfo keyCodes[] = {
    // 1-0: as ASCII
    {49,3,0x01}, {50,3,0x02}, {51,3,0x04}, {52,3,0x08}, {53,3,0x10},
    {54,4,0x10}, {55,4,0x08}, {56,4,0x04}, {57,4,0x02}, {48,4,0x01},

    // QWERTY
    { 65,1,0x01}, //A
    { 66,7,0x10}, //B
    { 67,0,0x08}, //C
    { 68,1,0x04}, //D
    { 69,2,0x04}, //E
    { 70,1,0x08}, //F
    { 71,1,0x10}, //G
    { 72,6,0x10}, //H
    //
    { 73,5,0x04}, //I
    { 74,6,0x08}, //J
    { 75,6,0x04}, //K
    { 76,6,0x02}, //L
    { 77,7,0x04}, //M
    { 78,7,0x08}, //N
    { 79,5,0x02}, //O
    { 80,5,0x01}, //P
    { 81,2,0x01}, //Q
    { 82,2,0x08}, //R
    { 83,1,0x02}, //S
    { 84,2,0x10}, //T
    { 85,5,0x08}, //U
    { 86,0,0x10}, //V
    { 87,2,0x02}, //W
    { 88,0,0x04}, //X
    { 89,5,0x10}, //y
    { 90,0,0x02}, //z
    //
    //
    { 32,7,0x01}, /* space */
    { 13,6,0x01}, /* ENTER */

    { 4, 8,0x02}, /* , */
    { 59, 8,0x03}, /* ; */
    { 45, 8,0x04}, /* - */
    { 61, 8,0x05}, /* = */
    { 47, 8,0x06}  /* / */
};

#define KEYCOUNT (sizeof(keyCodes)/sizeof(keyCodes[0]))

static uint8_t findKey(uint8_t key, uint8_t *row, uint8_t *mask)
{
    for (uint8_t i = 0; i < KEYCOUNT; i++)
    {
        if (keyCodes[i].key == key)
        {
            *row = keyCodes[i].row;
            *mask = keyCodes[i].mask;
            return 1;
        }
    }
    return 0;
}


void zxckeyboard_ctor() {
  for (uint8_t row = 0; row < 9U; row++) {
    keyStates[row] = 0xff;
  }
}


void zxckeyboard_onDown(uint8_t keycode) {
uint8_t row, mask;

    if (findKey(keycode, &row, &mask))
    {
        if (row==8) {
            switch (mask) {
                case 0x01: keyStates[0] &= ~0x01; keyStates[4] &= ~0x01; break;
                case 0x02: keyStates[0] &= ~0x01; keyStates[7] &= ~0x02; break;
                case 0x03: keyStates[0] &= ~0x01; keyStates[0] &= ~0x04; break;
                case 0x04: keyStates[0] &= ~0x01; keyStates[6] &= ~0x08; break;
                case 0x05: keyStates[0] &= ~0x01; keyStates[6] &= ~0x02; break;
                case 0x06: keyStates[0] &= ~0x01; keyStates[0] &= ~0x10; break;
            }
        } else {
            keyStates[row] &= ~(mask);
        }
    }
}


void zxckeyboard_onUp(uint8_t keycode) {
uint8_t row, mask;

    if (findKey(keycode, &row, &mask))
    {
        if (row==8) {
            switch (mask) {
                case 0x01: keyStates[0] |= 0x01; keyStates[4] |= 0x01; break;
                case 0x02: keyStates[0] |= 0x01; keyStates[7] |= 0x02; break;
                case 0x03: keyStates[0] |= 0x01; keyStates[0] |= 0x04; break;
                case 0x04: keyStates[0] |= 0x01; keyStates[6] |= 0x08; break;
                case 0x05: keyStates[0] |= 0x01; keyStates[6] |= 0x02; break;
                case 0x06: keyStates[0] |= 0x01; keyStates[0] |= 0x10; break;
            }
        } else {
            keyStates[row] |= (mask);
        }
    }
}


uint8_t zxckeyboard_getRow(uint8_t row) {
    return keyStates[row];
}
