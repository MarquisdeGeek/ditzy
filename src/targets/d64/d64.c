#ifdef _CMOC_VERSION_
#include <cmoc.h>
#endif

// Taken from the Mandelbrot example in DEMF_TIGHT_PRUNE
#include "d64.h"


unsigned char waitForKey64() {
    unsigned char key;

    asm {
KEYWAIT JSR     [$E000]
        TSTA
        BEQ     KEYWAIT

        STA     :key
    }

    return key;
}


unsigned char getKey64() {
    unsigned char key;

    asm {
        ; Save registers
        // PSHS    D,X,Y,DP,U

        jsr     [$E000]
        STA     :key

        ; Restore registers
        // PULS    D,X,Y,DP,U
    }

    return key;
}


void poke(unsigned short addr, unsigned char data) {
    *(unsigned char *)addr = data;
}

void poke16(unsigned short addr, unsigned short data) {
    *(unsigned char *)(addr+0) = (unsigned char)(data >> 8);
    *(unsigned char *)(addr+1) = (unsigned char)(data & 255);
}


unsigned char peek(short addr) {
    return *(unsigned char *)addr;
}


void setTextMode(/*TODO, mode select*/) {
    asm {
        LDA     #$0
        STA     $FF22

        ; SAM: 000
        STA     $FFC0
        STA     $FFC2
        STA     $FFC4

        ; Prepare screen address bits
        STA     $FFC6+0 // bit 0, set to 0 : addr=$0200
        STA     $FFC8+1 // bit 1, set to 1 : addr=$0400
        STA     $FFCA+0 // bit 2, set to 0 : addr=$0800
        STA     $FFCC+0 // bit 3, set to 0 : addr=$1000
        STA     $FFCE+0 // bit 4, set to 0 : addr=$2000
        STA     $FFD0+0 // bit 5, set to 0 : addr=$4000
        STA     $FFD2+0 // bit 6, set to 0 : addr=$8000
    }
}

void setPMode(/*TODO, mode select*/) {

    asm {
        LDA     #$F8
        STA     $FF22

        ; SAM 110 = 2 color	256 x 192	6144
        STA     $FFC0
        STA     $FFC3
        STA     $FFC5

        ; Clear the 7 screen address bits
        STA     $FFC6+1 // bit 0, set to 1 : addr=$0200
        STA     $FFC8+1 // bit 1, set to 1 : addr=$0400
        STA     $FFCA+0 // bit 2, set to 0 : addr=$0800
        STA     $FFCC+0 // bit 3, set to 0 : addr=$1000
        STA     $FFCE+0 // bit 4, set to 0 : addr=$2000
        STA     $FFD0+0 // bit 5, set to 0 : addr=$4000
        STA     $FFD2+0 // bit 6, set to 0 : addr=$8000
    }
}


void clearScreenGraphics() {
    unsigned char* pGraphics = (unsigned char*)1536;
#if D32_COMPILE_WITH_STDLIB
    memset(pGraphics, 0, 6144);
#else
    for(unsigned short i=0;i<6144U;++i) {
        *pGraphics++ = 0;
    }
#endif
}

void clearScreenText() {
    unsigned char* pText = (unsigned char*)1024;
#if D32_COMPILE_WITH_STDLIB
    memset(pText, 96, 32*16);
#else
    for(short i=0;i<32*16;++i) {
        *pText++ = 96;
    }
#endif
}


void blit(const unsigned char* pScreen) {
#if D32_COMPILE_WITH_STDLIB
    unsigned char* pGraphics = (unsigned char*)1536;
    memcpy(pGraphics, pScreen, 6144);
#endif
}


void plot(tScreenWidth x, tScreenHeight y) {

    unsigned char* pScreen = (unsigned char*)  ((unsigned short)*(char *)0xbc << 8); //inlined peek

    pScreen += x >> 3; // x / 8
    pScreen += y << 5; // y * (256/8);

    *pScreen |= ((unsigned char)(128 >> (x & 7)));
}


void unplot(tScreenWidth x, tScreenHeight y) {

    unsigned char* pScreen = (unsigned char*)  ((unsigned short)*(char *)0xbc << 8); //inlined peek

    pScreen += x >> 3; // x / 8
    pScreen += y << 5; // y * (256/8);

    *pScreen &= ~((unsigned char)(128 >> (x & 7)));
}


// THese are expected to be in multiples of 8, making the screen 0-31, 0-23
void xorBlock(tScreenWidth x8, tScreenHeight y8, int size) {
#if D32_COMPILE_WITH_STDLIB
    unsigned char* pScreen = (unsigned char*)  (256 * *(char *)0xbc); //inlined peek

    pScreen += x8;
    pScreen += y8 * (8 * 32);

    for(int y=0;y<8 * size;++y) {
        for(int x=0;x<size;++x) {
            *(pScreen + x + y*32) ^= 0xff;
        }
    }
#endif
}


// w===8
void xorUDG(tScreenWidth x8, tScreenHeight y8, int w, int h, unsigned char*pData) {
#if D32_COMPILE_WITH_STDLIB
    unsigned char* pScreen = (unsigned char*)  (256 * *(char *)0xbc); //inlined peek

    pScreen += x8;
    pScreen += y8 * (8 * 32);

    for(int y=0;y<h;++y) {
        *(pScreen + y*32) ^= pData[h-y];
    }
#endif
}
