#ifndef EMF_PLATFORMS_ZX81_COMMON_KEYBOARD_C_H
#define EMF_PLATFORMS_ZX81_COMMON_KEYBOARD_C_H 1

#include "emf/types.h"

#ifdef __cplusplus
extern "C" {
#endif


void zxckeyboard_ctor();
void zxckeyboard_onDown(uint8_t key);
void zxckeyboard_onUp(uint8_t key);

uint8_t zxckeyboard_getRow(uint8_t row);


#ifdef __cplusplus
}
#endif

#endif // EMF_PLATFORMS_ZX81_COMMON_KEYBOARD_C_H


            