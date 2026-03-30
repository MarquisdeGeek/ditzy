#include <cstdio>
#include <string>
#include <deque>

#include "emf/emf.h"

#include "emulator/z80/cpp/clock_cpu.h"
#include "emulator/z80/cpp/zx81_z80_emulator.cpp" // YEAH YEAH - I KNOW!
#include "emulator/z80/c/zxccpu.h"

#include "platforms/zx81/1k/c/mem1k.h"
#include "platforms/zx81/common/c/zxckeyboard.h"
#include "platforms/zx81/common/cpp/zxkeyboard.h"

#include "emf/framework/comparator.h"


zx81_clock_cpu zx_clock;
zx81_z80_emulator zx_emu;

int ticks = 0;
#if EMF_COMPARATOR_BUFFER_SIZE
std::deque<std::string> gComparatorBuffer;
#endif

void zxstart() {
    // Pretend we're returning from a LOAD instruction
    uint16_t kilobytes = 1;
    uint16_t ramtop = 0x4000 + 0x400 * kilobytes;
    uint16_t sp = ramtop - 4; // e.g. 0x7ffc for 16K

    // C++ version
    zx_emu.setRegisterValueINTV(0x1e);
    zx_emu.setRegisterValueMEMREFRESH(0xca);
    zx_emu.setRegisterValueSP(sp);
    zx_emu.setRegisterValuePC(0x0207);
    zx_emu.setRegisterValueIX(0x0281);
    zx_emu.setRegisterValueIY(0x4000);
    zx_emu.reset();
    //
#if CHESS_ONLY_OPTIMISATIONS
    // NOP
#else
    // Auto reboot into BASIC ROM
    zx_emu.setRegisterValuePC(0);
#endif


    // C version
    cpu_ctor();
    cpu_fromload(sp);
    zxckeyboard_ctor();

    // Both
    emf_comparator_ctor();

}


void zxend() {
#if EMF_COMPARATOR_CPU_REPORTS
    emf_comparator_cpu_report_printf();
#endif
}


void zxupdate() {
#if EMF_CPU_DEBUG_STATE
    bool debugComparator = true;
#else
    const bool debugComparator = false; // need a CPU debug state to compare, obvs!
#endif

    uint32_t fps = 50;
    uint32_t cpuRate = zx_clock.getFrequency();
    uint32_t maxtstates = cpuRate / fps;
    uint32_t tstates;
    uint32_t tstates1 = 0;
    uint32_t tstates2 = 0;

    struct Z80State cstate1;
    char s1[256];
#if EMF_CPU_DEBUG_STATE
    struct Z80State cstate2;
    char s2[256];
#endif    

    do {

#if EMF_CPU_DEBUG_STATE
        if (debugComparator) {
            // Our prelude stores the instruction bytes
            char prelude[32];

            zx_emu.getState(&cstate1);
            sprintf(prelude, "%d/%d", read8(cstate1.pc), read8(1+cstate1.pc));
            emf_comparator_sprintf(s1, prelude, &cstate1);

            cpu_getstate(&cstate2);
            sprintf(prelude, "%d/%d", memory_read8(cstate2.pc), memory_read8(1+cstate2.pc));
            emf_comparator_sprintf(s2, prelude, &cstate2);

#if EMF_COMPARATOR_BUFFER_SIZE
            gComparatorBuffer.push_back(std::string(s1));
            gComparatorBuffer.push_back(std::string(s2));

            while (gComparatorBuffer.size() > EMF_COMPARATOR_BUFFER_SIZE) {
                gComparatorBuffer.pop_front();
            }
#endif

#if EMF_CPU_COMPARATOR_TRACE_1
            printf("%d :: %s\n", ticks, s1);
#endif

#if EMF_CPU_COMPARATOR_TRACE_2
            printf("%d :: %s\n", ticks, s2);
#endif
        }
#endif

#if EMF_INC_EMU_C
        uint32_t cycles1 = cpu_step();
        tstates1 += cycles1;
#endif

#if EMF_COMPARATOR_CPU_REPORTS
        u8 op = memory_read8(cstate1.pc);
        u8 ext = memory_read8(cstate1.pc + 1);
        emf_comparator_cpu_report_add(op, ext);
#endif

#if EMF_INC_EMU_CPP
        uint32_t cycles2 = zx_emu.update();
        tstates2 += cycles2;
#endif

#if EMF_CPU_DEBUG_STATE
        if (debugComparator) {
            if (!emf_comparator_areIdentical(&cstate1, &cstate2)) {
                // Write the whole state out, since we might not be using the EMF_CPU_COMPARATOR_TRACE_'s
#if EMF_COMPARATOR_BUFFER_SIZE
                for (auto it = gComparatorBuffer.begin();it!=gComparatorBuffer.end();++it) {
                    printf("%s\n", it->c_str());
                }
                gComparatorBuffer.clear();
#endif

                printf("%d :: ( C )%s\n(CPP)%s\n^^^^ ERROR%s\n\n", ticks, s1, s2, cycles1 == cycles2?"":"(and cycle mismatch)");
            } else if (cycles1 != cycles2) {
                // printf("^^^^ cycle mismatch\n");
            }
        }
#endif

#if EMF_INC_EMU_C
        tstates = tstates1;
#endif
#if EMF_INC_EMU_CPP
        tstates = tstates2;
#endif

    } while (tstates < maxtstates);
}
