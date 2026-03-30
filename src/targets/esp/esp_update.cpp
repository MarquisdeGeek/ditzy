#include "emf/emf.h"

#include "emulator/z80/cpp/clock_cpu.h"
#include "emulator/z80/cpp/zx81_z80_emulator.cpp" // YEAH YEAH - I KNOW!


zx81_clock_cpu zx_clock;
zx81_z80_emulator zx_emu;


void zxstart() {
    // Pretend we're returning from a LOAD instruction
    uint16_t kilobytes = 1;
    uint16_t ramtop = 0x4000 + 0x400 * kilobytes;
    uint16_t sp = ramtop - 4; // e.g. 0x7ffc for 16K

    // C++ version
    zx_emu.reset();
    zx_emu.setRegisterValueINTV(0x1e);
    zx_emu.setRegisterValueMEMREFRESH(0xca);
    zx_emu.setRegisterValueSP(sp);
    zx_emu.setRegisterValuePC(0x0207);
    zx_emu.setRegisterValueIX(0x0281);
    zx_emu.setRegisterValueIY(0x4000);
}


void zxend() {
}


void zxupdate() {
    uint32_t fps = 50;
    uint32_t cpuRate = zx_clock.getFrequency();
    uint32_t maxtstates = cpuRate / fps;
    uint32_t tstates = 0;

    do {
        uint32_t cycles = zx_emu.update();
        tstates += cycles;
    } while (tstates < maxtstates);
}
