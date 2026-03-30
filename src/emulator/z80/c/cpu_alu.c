#include "emf/types.h"

uint16_t alu_complement16(uint16_t v) {

  v = (~v) & 0xffff;

  computeFlags16(v);

return v;
}
uint16_t alu_or16(uint16_t v1, uint16_t v2) {

  result = computeFlags16(v1 | v2);

return result;
}
bool alu_neq16(uint16_t v1, uint16_t v2) {

result = v1 != v2;

return (bool)result;
}
bool alu_neq8(uint8_t v1, uint8_t v2) {

  result = v1 != v2;

return (bool)result;
}
uint8_t alu_sll8(uint8_t v) {

// SLL is undocumented it seems (at least in Zaks:82)
// http://www.z80.info/z80undoc.htm
// suggests it's like SLA, but with 1 in the LSB
  

  wasCarry = v & 0x80 ? 1 : 0;
  v <<= 1;
  v |= 1;
  v = v & 0xff;

  computeFlags8(v);


return v;
}
uint16_t alu_test16(uint16_t v) {

  v = computeFlags16(v);

return v;
}
uint16_t alu_add_u16s8(uint16_t v1, uint8_t v2, uint8_t v3) {


  u32 result = (v1 + (uint16_t)v2 + (uint16_t)v3);
  if (v2 >= 128U) { // handle the negative bit of 8 bit numbers in v2
    result -= 256;
  }
  wasCarry = result > 0xffff ? 1 : 0;

  // 16 bit adds set_'H' on overflow of bit 11 (!?)

  // The MSB is same on both src params, but changed between result and src param1
  u32 lookup = ((v1 & 0x8800) >> 11) | (((v2) & 0x8800) >> 10) | ((result & 0x8800) >> 9);
  wasHalfCarry = flagHalfCarryAdd[(lookup & 7)];
  lookup >>= 4;
  wasOverflow = (lookup == 3 || lookup == 4) ? 1 : 0;

  result &= 0xffff;

  computeFlags16(result);


return result;
}
uint16_t alu_add_u16u16c(uint16_t v1, uint16_t v2, uint16_t v3) {

  u32 result = (v1 + v2 + v3);
  wasCarry = result > 0xffff ? 1 : 0;

  // 16 bit adds set_'H' on overflow of bit 11 (!?)

  // The MSB is same on both src params, but changed between result and src param1
  u16 lookup = ((v1 & 0x8800) >> 11) | (((v2) & 0x8800) >> 10) | ((result & 0x8800) >> 9);
  wasHalfCarry = flagHalfCarryAdd[(lookup & 7)];
  lookup >>= 4;
  wasOverflow = (lookup == 3 || lookup == 4) ? 1 : 0; // TODO: not convinced any Z80 instr checks the 'V' flag fter 16 bit adds

  result &= 0xffff;

  computeFlags16(result);
  aluLastResult = result;

return result;
}
void alu_main_reset() {

return ;
}
bool alu_eq8(uint8_t v1, uint8_t v2) {

  result = v1 == v2;

return (bool)result;
}
uint8_t alu_complement8(uint8_t v) {

  v = (~v) & 0xff;

  computeFlags8(v);

return v;
}
uint8_t alu_rra8(uint16_t v, uint8_t carry) {


  v |= carry ? 0x100 : 0;
  wasCarry = v & 1;
  v >>= 1;
  v &= 0xff;

  computeFlags8((uint8_t)v);


return (uint8_t)v;
}
uint16_t alu_abs16(uint16_t v) {

  result = computeFlags16(v > 0 ? v : -v);

return result;
}
uint8_t alu_sub_u8u8b(uint8_t v1, uint8_t v2, uint8_t v3) {

  u16 result = ((uint16_t)v1 - (uint16_t)v2) - v3;

  wasCarry = result & 0x100 ? 1 : 0;
  wasNegation = true;

  // Did the calculation in the lowest 4 bits spill under

  // The MSB is same on both src params, but changed between result and src param1
  u16 lookup = ((v1 & 0x88) >> 3) | (((v2) & 0x88) >> 2) | ((result & 0x88) >> 1);
  wasHalfCarry = flagHalfCarrySub[(lookup & 7)];
  lookup >>= 4;
  wasOverflow = (lookup == 1 || lookup == 6) ? 1 : 0;

  result &= 0xff;

  computeFlags8(result);


return (uint8_t)result;
}
uint8_t alu_rl8(uint8_t v, uint8_t carry) {


  wasCarry = v & 0x80 ? 1 : 0;
  v <<= 1;
  v |= carry;
  v = v & 0xff;

  computeFlags8(v);


return v;
}
void alu_main_start() {

        for (uint16_t i = 0; i < 256U; ++i) {
            parityLUT8[i] = alu_main_calculateParity(i,8); // we know how the C code expands 'calculateParity' to this full version
        }

return ;
}
uint8_t alu_sla8(uint8_t v) {


  wasCarry = v & 0x80 ? 1 : 0;
  v <<= 1;
  v = v & 0xff;

  computeFlags8(v);



return v;
}
uint16_t alu_sub_u16u16(uint16_t v1, uint16_t v2) {

  result = (v1  - v2) & 0xffff;
  aluLastResult = result;

return result;
}
uint8_t alu_or8(uint8_t v1, uint8_t v2) {

  result = computeFlags8(v1 | v2);

return (uint8_t)result;
}
uint8_t alu_daa(uint8_t v, uint8_t carry, uint8_t subtraction) {


  wasCarry = carry;

  if (subtraction) { // last instr was subtraction	
    if ((v & 0x0f) > 9) {
      v -= 6;
    }
    if ((v & 0xf0) > 0x90) {
      v -= 0x60;
    }
  } else { // post an addition
    if ((v & 0x0f) > 9) {
      v += 6;
    }
    if ((v & 0xf0) > 0x90) {
      v += 0x60;
    }
  }
  v = v & 0xff;
  computeFlags8(v);

return v;
}
uint8_t alu_rrc8(uint8_t v) {


  wasCarry = v & 1 ? 1 : 0;
  v >>= 1;
  v |= wasCarry ? 0x80 : 0;

  computeFlags8(v);


return (uint8_t)(result);
}
bool alu_eq16(uint16_t v1, uint16_t v2) {

result = v1 == v2;

return (bool)(result);
}
uint16_t alu_sub_u16u16b(uint16_t v1, uint16_t v2, uint16_t v3) {

  u32 result = ((u32)v1 - (u32)v2) - (u32)v3;

  // TODO: CHECK: BUGWARN: 
  //   wasCarry = v2>v1 ? 1 : 0;
  wasCarry = v2+v3>v1 ? 1 : 0;

  // wasCarry = result & 0x10000 ? 1 : 0;
  wasNegation = true;

  // 16-bit half carry occurs on bit 11

  // The MSB is same on both src params, but changed between result and src param1
  u32 lookup = ((v1 & 0x8800) >> 11) | (((v2) & 0x8800) >> 10) | ((result & 0x8800) >> 9);
  wasHalfCarry = flagHalfCarrySub[(lookup & 7)];
  lookup >>= 4;
  wasOverflow = (lookup == 1 || lookup == 6) ? 1 : 0;

  result &= 0xffff;

  computeFlags16(result);


return result;
}
uint16_t alu_and16(uint16_t v1, uint16_t v2) {

  result = computeFlags16(v1 & v2);

return result;
}
uint16_t alu_xor16(uint16_t v1, uint16_t v2) {

  result = computeFlags16(v1 ^ v2);

return result;
}
uint8_t alu_and8(uint8_t v1, uint8_t v2) {

  result = computeFlags8(v1 & v2);

return (uint8_t)result;
}
uint8_t alu_add_u8u8(uint8_t v1, uint8_t v2) {

  u8 result = (v1 + v2 );

return (uint8_t)result;
}
uint8_t alu_main_calculateParity(uint8_t v, uint8_t sz) {

    uint8_t bits = 0;

    v = v & 255; /// ensure it's positive, for the table deference

    for (uint8_t i = 0; i < 8; ++i) {
        if (v & (1 << i)) {
            ++bits;
        }
    }
    uint8_t parity = (bits & 1) == 1 ? 0 : 1; // odd parity returns 0

return parity;
}
uint8_t alu_sra8(uint8_t v) {


  wasCarry = v & 1;
  v >>= 1;
  v |= (v & 0x40) << 1;

  computeFlags8(v);



return v;
}
uint8_t alu_lsr8(uint8_t v, uint8_t places) {

  wasCarry = v & 1;
  v >>= places;


return v;
}
uint8_t alu_srl8(uint8_t v) {


  wasCarry = v & 1;
  v >>= 1;
  v = v & 0x7f;

  computeFlags8(v);


return v;
}
bool alu_testBit8(uint8_t bit, uint8_t value) {

  bool isBitSet = value & (1 << bit) ? 1 : 0;

  wasSign = value & 0x80 ? 1 : 0;
  wasZero = isBitSet ? 0 : 1;
  wasOverflow = wasZero;
  wasParity = wasZero; // TODO: sure this isn't getParity8(value);?


return isBitSet;
}
uint8_t alu_rlc8(uint8_t v) {


  wasCarry = v & 0x80 ? 1 : 0;
  v <<= 1;
  v |= wasCarry;
  v = v & 0xff;

  computeFlags8(v);


return v;
}
uint8_t alu_lsl8(uint8_t v, uint8_t places) {

  wasCarry = v & 0x80 ? 1 : 0;
  v <<= places;

return v;
}
uint8_t alu_rr8(uint8_t v, uint8_t carry) {


  wasCarry = v & 1 ? 1 : 0;
  v >>= 1;
  v |= carry ? 0x80 : 0;

  computeFlags8(v);


return v;
}
uint8_t alu_xor8(uint8_t v1, uint8_t v2) {

  result = computeFlags8(v1 ^ v2);

return (uint8_t)result;
}
uint16_t alu_add_u16u16(uint16_t v1, uint16_t v2) {

  result = (v1 + v2) & 0xffff;
  aluLastResult = result;

return result;
}

uint8_t alu_clearBit8(uint8_t bit, uint8_t value) {

  value = value & ~(1 << bit);
  computeFlags8(value);

return value;
}
uint8_t alu_add_u8u8c(uint8_t v1, uint8_t v2, uint8_t v3) {



  u16 result = ((uint16_t)v1 + (uint16_t)v2 + v3);
  wasCarry = result > 0xff ? 1 : 0;

  // Did the calculation in the lowest 4 bits spill over into the upper 4 bits

  // The MSB is same on both src params, but changed between result and src param1
  u16 lookup = ((v1 & 0x88) >> 3) | (((v2) & 0x88) >> 2) | ((result & 0x88) >> 1);
  wasHalfCarry = flagHalfCarryAdd[(lookup & 7)];
  lookup >>= 4;
  wasOverflow = (lookup == 3 || lookup == 4) ? 1 : 0;

  result &= 0xff;

  computeFlags8(result);
  aluLastResult = result;

return (uint8_t)(result);
}
uint8_t alu_setBit8(uint8_t bit, uint8_t value) {

  value = value | (1 << bit);
  computeFlags8(value);

return value;
}
