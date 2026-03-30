#include "zxkeyboard.h"
#include <string>
#include <unordered_map>
#include <cstdint>


ZXKeyboard zx81_keyboard;


void zxkeyboard_onDown(uint8_t key) {
    zx81_keyboard.onKeyDown(key);
}

void zxkeyboard_onUp(uint8_t key) {
    zx81_keyboard.onKeyUp(key);
}



struct KeyInfo {
    int row;
    uint8_t mask;
};

static std::unordered_map<int, KeyInfo> keyCodes = {
    {49,  {3, 0x01}}, /* 1 */
    {50,  {3, 0x02}}, /* 2 */
    {51,  {3, 0x04}}, /* 3 */
    {52,  {3, 0x08}}, /* 4 */
    {53,  {3, 0x10}}, /* 5 */
    {54,  {4, 0x10}}, /* 6 */
    {55,  {4, 0x08}}, /* 7 */
    {56,  {4, 0x04}}, /* 8 */
    {57,  {4, 0x02}}, /* 9 */
    {48,  {4, 0x01}}, /* 0 */

    {81,  {2, 0x01}}, /* Q */
    {87,  {2, 0x02}}, /* W */
    {69,  {2, 0x04}}, /* E */
    {82,  {2, 0x08}}, /* R */
    {84,  {2, 0x10}}, /* T */
    {89,  {5, 0x10}}, /* Y */
    {85,  {5, 0x08}}, /* U */
    {73,  {5, 0x04}}, /* I */
    {79,  {5, 0x02}}, /* O */
    {80,  {5, 0x01}}, /* P */

    {65,  {1, 0x01}}, /* A */
    {83,  {1, 0x02}}, /* S */
    {68,  {1, 0x04}}, /* D */
    {70,  {1, 0x08}}, /* F */
    {71,  {1, 0x10}}, /* G */
    {72,  {6, 0x10}}, /* H */
    {74,  {6, 0x08}}, /* J */
    {75,  {6, 0x04}}, /* K */
    {76,  {6, 0x02}}, /* L */
    {13,  {6, 0x01}}, /* enter */

    {225, {0, 0x01}}, /* caps: as left shift */
    {229, {0, 0x01}}, /* caps: as right shift */
    {16,  {0, 0x01}}, /* caps */
    {192, {0, 0x01}}, /* backtick as caps */
    {90,  {0, 0x02}}, /* Z */
    {88,  {0, 0x04}}, /* X */
    {67,  {0, 0x08}}, /* C */
    {86,  {0, 0x10}}, /* V */
    {66,  {7, 0x10}}, /* B */
    {78,  {7, 0x08}}, /* N */
    {77,  {7, 0x04}}, /* M */
    {190, {7, 0x02}}, /* . */
    {32,  {7, 0x01}}, /* space */

    {37,  {3, 0x10}}, /* left as 5 */
    {40,  {4, 0x10}}, /* down as 6 */
    {38,  {4, 0x08}}, /* up as 7 */
    {39,  {4, 0x04}}, /* right as 8 */

    {8,   {8, 0x01}}, /* rubout */
    {46,  {8, 0x01}}, /* rubout */
    {188, {8, 0x02}}, /* , */
    {59,  {8, 0x03}}, /* ; */
    {173, {8, 0x04}}, /* - */
    {61,  {8, 0x05}}, /* = */
    {191, {8, 0x06}}  /* / */
};



ZXKeyboard::ZXKeyboard() {
  for (int row = 0; row < 9; row++) {
    keyStates[row] = 0xff;
  }
}


void ZXKeyboard::onKeyDown(uint8_t key) {

    key = toupper(key);

    if (keyCodes.find(key) != keyCodes.end()) {
        KeyInfo keyCode = keyCodes[key];
        // use info.row, info.mask
        if (keyCode.row==8) {
            switch (keyCode.mask) {
            case 0x01: keyStates[0] &= ~0x01; keyStates[4] &= ~0x01; break;
            case 0x02: keyStates[0] &= ~0x01; keyStates[7] &= ~0x02; break;
            case 0x03: keyStates[0] &= ~0x01; keyStates[0] &= ~0x04; break;
            case 0x04: keyStates[0] &= ~0x01; keyStates[6] &= ~0x08; break;
            case 0x05: keyStates[0] &= ~0x01; keyStates[6] &= ~0x02; break;
            case 0x06: keyStates[0] &= ~0x01; keyStates[0] &= ~0x10; break;
            }
        } else {
            keyStates[keyCode.row] &= ~(keyCode.mask);
        }
    }
}

void ZXKeyboard::onKeyUp(uint8_t key) {
    key = toupper(key);

    if (keyCodes.find(key) != keyCodes.end()) {
        KeyInfo keyCode = keyCodes[key];
        // use info.row, info.mask

        if (keyCode.row==8) {
            switch (keyCode.mask) {
            case 0x01: keyStates[0] |= 0x01; keyStates[4] |= 0x01; break;
            case 0x02: keyStates[0] |= 0x01; keyStates[7] |= 0x02; break;
            case 0x03: keyStates[0] |= 0x01; keyStates[0] |= 0x04; break;
            case 0x04: keyStates[0] |= 0x01; keyStates[6] |= 0x08; break;
            case 0x05: keyStates[0] |= 0x01; keyStates[6] |= 0x02; break;
            case 0x06: keyStates[0] |= 0x01; keyStates[0] |= 0x10; break;
            }
        } else {
            keyStates[keyCode.row] |= keyCode.mask;
        }
    }
}

