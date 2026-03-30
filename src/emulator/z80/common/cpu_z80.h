#ifndef EMF_CPU_Z80_COMMON
#define EMF_CPU_Z80_COMMON 1

#include "emf/types.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct Z80State {
    uint16_t pc;
    uint16_t sp;
    uint16_t ix;
    uint16_t iy;

    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;

    uint8_t f;

    uint8_t prime_b;
    uint8_t prime_c;
    uint8_t prime_h;
    uint8_t prime_l;
} Z80State;


#ifdef __cplusplus
}
#endif

#endif // EMF_CPU_Z80_COMMON
