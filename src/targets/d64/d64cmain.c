#ifdef _CMOC_VERSION_
#include <cmoc.h>
#endif

#include "d64.h"

#include "emf/types.h"
#include "emulator/z80/c/zxccpu.h"
#include "platforms/zx81/common/c/zxckeyboard.h"
#include "d64zxcscreen.h"

// This 'org' param is now set in the build system
// #pragma org 15000
/*
Memory map:

1024 : text screen
1536 : Gfx screen (6144 bytes, to 7680)
7680 : Gfx, page 5
13824-49151 : Program and Variable Storage
15000 : Code starts (last check, size = 33321)
48321 : Code ends
49152 : BASIC, in 64 mode
*/
void memory_trap8(uint16_t addr, uint8_t data);
void prepareScreenMode();

bool modeIsText;

int main(void)
{
    // uint16_t kilobytes = 1;
    // uint16_t ramtop = 0x4000 + 0x400 * kilobytes;
    // uint16_t sp = ramtop - 4; // e.g. 0x7ffc for 16K

    cpu_ctor();
    cpu_fromload(17404); //pre compute SP for 1K
    zxckeyboard_ctor();

    uint8_t key = 0;
    uint8_t previousKey = 0;

    // Draw the full screen
    modeIsText = true;
    drawCtor();
    prepareScreenMode();
    memory_trap8(0,0);

    // Main loop, until escape/break hit
    while(key != 3) {
        for(uint16_t i=0;i<6535U;++i) {
            cpu_step();
        }

        key = getKey64();

#if EMF_DEBUG_KEYPRESSES
        if (key) poke(1533, ~peek(1533));
        poke(1534, previousKey);
        poke(1535, key);
#endif

        if (key) {
            zxckeyboard_onDown(key);
            previousKey = key;
        } else if (previousKey) {
            zxckeyboard_onUp(previousKey);
            previousKey = 0;
        }

        // Graphics mode toggle on the space bar
        if (key == 32) {
            modeIsText = !modeIsText;
            prepareScreenMode();
            memory_trap8(0,0);
        }
    }
    
    return 0;
}


void prepareScreenMode() {
    if (modeIsText) {
        setTextMode();
        clearScreenText();

    } else {
        setPMode();
        clearScreenGraphics();
    }
}


void memory_trap8(uint16_t addr, uint8_t data) {
    // TODO: We can use the addr/data pair to only
    // redraw the specific character(s) which have
    // been changed.
    // If you're happy to use 768 bytes, a simple dirty
    // check can be done with -DEMF_GRAPHICS_WITH_DIRTY_GRID=1

    if (modeIsText) {
        drawZX81DisplayText();

    } else {
        drawZX81DisplayGraphics();
    }
}

