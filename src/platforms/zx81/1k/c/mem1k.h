#ifndef EMF_PLATFORMS_ZX81_1K_C_MEMORY_H
#define EMF_PLATFORMS_ZX81_1K_C_MEMORY_H 1


#include "emf/types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t memory_fetch8(uint16_t addr);

uint8_t memory_read8(uint16_t addr);
void memory_write8(uint16_t addr, uint8_t d);
void memory_write16(uint16_t addr, uint16_t d);
uint16_t memory_read16(uint16_t addr);


#ifdef __cplusplus
}
#endif

#endif // EMF_PLATFORMS_ZX81_1K_C_MEMORY_H
