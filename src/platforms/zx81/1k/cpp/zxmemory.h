#ifndef EMF_PLATFORMS_ZX81_1K_CPP_MEMORY_H
#define EMF_PLATFORMS_ZX81_1K_CPP_MEMORY_H 1


// Helper methods, to avoid sharing the global memory object
uint8_t fetch8(uint16_t a);

uint8_t read8(uint16_t a);
uint16_t read16(uint16_t a);

void write8(uint16_t a, uint8_t d);
void write16(uint16_t a, uint16_t d);


#endif // EMF_PLATFORMS_ZX81_1K_CPP_MEMORY_H
