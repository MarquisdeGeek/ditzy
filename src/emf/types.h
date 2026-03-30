#ifndef EMF_EMF_TYPES_H
#define EMF_EMF_TYPES_H 1

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef unsigned int uint32_t;
typedef unsigned int u32;


#if __cplusplus
// NOP
#else
typedef unsigned char bool;
extern const uint8_t false;
extern const uint8_t true;

#endif

#endif // EMF_EMF_TYPES_H
