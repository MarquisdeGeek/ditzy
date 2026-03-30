#ifndef EMF_PLATFORMS_ZX81_COMMON_KEYBOARD_CPP_H
#define EMF_PLATFORMS_ZX81_COMMON_KEYBOARD_CPP_H 1

#include "emf/emf.h"


class ZXKeyboard {
public:

ZXKeyboard();

void onKeyDown(uint8_t key);
void onKeyUp(uint8_t);


uint8_t getRow(uint8_t row) {
  return keyStates[row];
}

uint8_t keyStates[9];

};


void zxkeyboard_onDown(uint8_t key);
void zxkeyboard_onUp(uint8_t key);



#endif // EMF_PLATFORMS_ZX81_COMMON_KEYBOARD_CPP_H
