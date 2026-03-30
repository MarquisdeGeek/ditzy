#ifndef EMF_CPU_Z80_C
#define EMF_CPU_Z80_C 1

#include "emf/types.h"
#include "emulator/z80/common/cpu_z80.h"


#ifdef __cplusplus
extern "C" {
#endif


void cpu_ctor();
void cpu_reset();
void cpu_start();
uint8_t cpu_step();

void cpu_fromload(uint16_t newsp);
void cpu_getstate(Z80State *pState);


#ifdef __cplusplus
}
#endif

#endif // EMF_CPU_Z80_C
