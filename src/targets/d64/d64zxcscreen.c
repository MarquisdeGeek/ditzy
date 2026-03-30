#include "emf/types.h"
#include "emulator/z80/c/zxccpu.h"
#include "platforms/zx81/1k/c/mem1k.h"
#include "platforms/zx81/common/zxmemory.h"
#include "d64zxcscreen.h"

#include "d64.h"

/*
Dragon characters:
0-31 : @A-Z[\]up left (all inv)
32-63: spc !"#$%&'()*+,-./0-9:;<=>? (all inv)
64-127: above, but normal
*/
static const uint8_t zx81_to_dragon[64] = {

/* 00–0F ZX81 graphics → Dragon semigraphics */
96, 135, 139, 131, 141, 133, 137, 129, 143+32, 131+32,140+32,

// '"','$','$',':','?',
64|34, 64|36,64|36, 64|58, 64|63,

/* 10–1F punctuation */
//'(',')','>','<','=','+','-','*',
64|40,64|41, 64|62,64|60, 64|61, 64|43,64|45,64|42,
// '/',';',',','.',
64|47,64|59, 64|44,64|46,

/* 20–27 digits: 48-57*/
64|'0',64|'1',64|'2',64|'3',64|'4',64|'5',64|'6',64|'7',64|'8',64|'9',

/* 26-3f letters */
65,66,67,68,69,70,71,72,
73,74,75,76,77,78,79,80,
81,82,83,84,85,86,87,88,
89,90,
/*
'A','B','C','D','E','F','G','H',
'I','J','K','L','M','N','O','P',
'Q','R','S','T','U','V','W','X',
'Y','Z',
*/

};


#if EMF_GRAPHICS_WITH_DIRTY_GRID
uint8_t currentDisplayMap[768];
#endif

void drawCtor() {
#if EMF_GRAPHICS_WITH_DIRTY_GRID
    for(uint16_t i=0;i<sizeof(currentDisplayMap);++i) {
        currentDisplayMap[i] = 256;
    }
#endif
}



static void drawCharacterText(uint8_t screenX, uint8_t screenY, uint8_t ch) {
uint16_t offset =  (uint16_t)screenX + (uint16_t)screenY * 32;
uint16_t addr = 1024 + offset;
uint8_t* pScreen = (uint8_t*) addr;
uint8_t inverse = ch & 128 ? 64 : 0;

    // No EMF_GRAPHICS_WITH_DIRTY_GRID here, since the checking would be slower
    *pScreen = zx81_to_dragon[ch & 0x3f] - inverse;

#if EMF_GRAPHICS_WITH_DIRTY_GRID
    // But we need to mark it as dirty, so when/if we switch to gfx
    currentDisplayMap[offset] = 255;
#endif
}


// Assumes PMODE 4 (256x192)
static void drawCharacterGfx(uint8_t screenX, uint8_t screenY, uint8_t ch) {
#if EMF_GRAPHICS_WITH_DIRTY_GRID
    uint16_t offset =  (uint16_t)screenX + ((uint16_t)screenY) << 5; // << 5 is * 32;
    if (currentDisplayMap[offset] == ch) {
        return;
    }
    currentDisplayMap[offset] = ch;
#endif

    const uint8_t *pFont = rom;

    pFont += 0x1d00;
    pFont += 8*32;
    pFont += (uint16_t)((ch & 0x7f)) * 8;

    uint16_t px = screenX*8;
    uint16_t py = screenY*8;

    for(uint8_t j=0;j<8;++j) {
        uint8_t b = *pFont++;
        if (ch<128) {
            b = ~b;
        }

        for(int i=7;i>=0;--i,++px) {
            if ((1<<i) & b) {
                plot(px, py);
            } else {
                unplot(px, py);
            }
        }
        px -= 8;
        ++py;
    }
}


void drawZX81DisplayText() {
    drawScreenC(drawCharacterText);
}


void drawZX81DisplayGraphics() {
    drawScreenC(drawCharacterGfx);
}


void drawScreenC(cbDrawChar pDrawChar) {

    uint16_t dfile = memory_read16(16396);
    uint16_t ptr = dfile + 3;// 1K chess hack: we have a few too many HALT(118)s here. Skip them
    uint8_t x = 0;
    for(uint8_t row=0;row<16;) { // !! BUGWARN: D32 has 16 rows in text mode. ZX has 22. Luckily chess only uses 11 of them
        uint8_t byte = memory_read8(ptr);
        if (byte == 118) {
            row++;
            x = 0;
        } else {
            (*pDrawChar)(x, row, byte);
            ++x;
        }
        //
        ++ptr;
    }

}