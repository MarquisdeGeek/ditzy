#ifndef EMF_PLATFORMS_ZX81_COMMON_IO_C_H
#define EMF_PLATFORMS_ZX81_COMMON_IO_C_H 1


#include "emf/types.h"

#ifdef __cplusplus
extern "C" {
#endif


uint8_t in8(uint16_t addr);
uint8_t in8hilo(uint8_t h, uint8_t a);
void out8(uint16_t addr, uint8_t d);


#ifdef __cplusplus
}
#endif

#endif // EMF_PLATFORMS_ZX81_COMMON_IO_C_H
