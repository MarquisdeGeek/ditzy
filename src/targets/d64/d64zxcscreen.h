#ifndef D64_ZXCSCREEN_H
#define D64_ZXCSCREEN_H 1


#include "emf/types.h"

#ifdef __cplusplus
extern "C" {
#endif


void drawCtor();
void drawZX81DisplayText();
void drawZX81DisplayGraphics();


typedef void (*cbDrawChar)(uint8_t screenX, uint8_t screenY, uint8_t ch);


void drawScreenC(cbDrawChar pDrawChar);


#ifdef __cplusplus
}
#endif

#endif // D64_ZXCSCREEN_H
