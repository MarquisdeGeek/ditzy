#ifndef EMF_PLATFORMS_ZX81_COMMON_IO_CPP_H
#define EMF_PLATFORMS_ZX81_COMMON_IO_CPP_H 1


#ifdef __cplusplus
// extern "C" {
#endif

namespace emf {
uint8_t in8hilo(uint8_t h, uint8_t a);
uint8_t in8(uint16_t addr);
void out8(uint16_t addr, uint8_t d);


}

#ifdef __cplusplus
// }
#endif


#endif // EMF_PLATFORMS_ZX81_COMMON_IO_CPP_H
