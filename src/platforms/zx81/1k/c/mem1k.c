#include "emf/types.h"
#include "platforms/zx81/common/zxmemory.h"
#include "platforms/zx81/1k/c/mem1k.h"

// Actually brings in variables
#include "platforms/zx81/1k/common/ram1kchess.inc"

uint8_t memory_fetch8(uint16_t address) { 
  uint8_t opcode = memory_read8(address&0x7fff);
#ifndef CHESS_ONLY_OPTIMISATIONS
  if (address & 0x8000) {
    if (opcode&0x40) {
      // nop : doing other lines of display
    } else {
      opcode = 0;
    }
  }
#endif
  return opcode;
}


uint8_t memory_read8(uint16_t addr) {

if (addr >= 0 && addr < 8192U) {
uint8_t data;
  data = rom[addr - 0];
return data;
} //fi 
if (addr >= 16384U && addr < 17408U) {
uint8_t data;
  data = ram[addr - 16384U];

return data;
} //fi 
return 0; // a bad default
}

uint16_t memory_read16(uint16_t addr) {
//   addr = addr.getUnsigned ? addr.getUnsigned() : addr;
  return ((uint16_t)memory_read8(addr+1))*256 + memory_read8(addr);
}

void memory_write8(uint16_t addr, uint8_t data) {
#if EMF_TIGHT_PRUNE==0
if (addr >= 0 && addr < 8192 ) {
  // NOP - read only memory
} //fi 
#endif
if (addr >= 16384 && addr < 17408 ) {

  ram[addr - 16384] = data;

  // HACK: Screen trap, since we don't have watchers in the C version
  if (addr >= 17202 && addr < 17324) {
    extern void memory_trap8(uint16_t addr, uint8_t data);
    memory_trap8(addr, data);
  }
} //fi 
}
 // write8


void memory_write16(uint16_t addr, uint16_t data) {
//   addr = addr.getUnsigned ? addr.getUnsigned() : addr;
//   data = data.getUnsigned ? data.getUnsigned() : data;
  memory_write8(addr+1, (uint8_t)(data>>8));
  memory_write8(addr+0, (uint8_t)(data&255));
}

