#include <string>

#include "emf/emf.h"
#include "platforms/zx81/common/zxmemory.h"


// Memory controller : 
// std.memory

class zx81_memory {
public:

// Brings in ram[] object
#include "platforms/zx81/1k/common/ram1kchess.inc"
// u8 ram[16 * 1024];

zx81_memory() {
  /* patch DISPLAY-5 to a return? */
  // rom[0x02b5] = 0xc9;
}
/*
**
** Basic methods
**
*/
void enableBlock(std::string name) {
//    enabledBlockList[name] = true;
}

void disableBlock(std::string name) {
//    enabledBlockList[name] = false;
}

bool isValidAddress(uint16_t addr) {
//    addr = addr.getUnsigned ? addr.getUnsigned() : addr;
//    addr &= 0xffff;
// if (addr >= 0 && addr < 8192 && enabledBlockList['rom']) { return true; }
// if (addr >= 16384 && addr < 17408 && enabledBlockList['ram']) { return true; }
return false;
}
 // isValidAddress

void getAddressRanges(void ) {
//    let ranges = [];
//   ranges.push({name:'rom', start:0, size:8192, read:true, write:false, shadow:false, enabled:enabledBlockList['rom']});
//   ranges.push({name:'ram', start:16384, size:1024, read:true, write:true, shadow:false, enabled:enabledBlockList['ram']});
//    return ranges;
}

std::string getLabel(uint16_t addr) {
//    addr = addr.getUnsigned ? addr.getUnsigned() : addr;
//    addr &= 0xffff;
// if (addr >= 0 && addr < 8192) { return rom_labels[addr - 0]; }
return "";
}

uint8_t read8(uint16_t addr) {
if (addr >= 0 && addr < 8192) {
uint8_t data;
  data = rom[addr - 0];
// printf("Also reading from %x (%d)\n", addr, addr);
return data;
} //fi 
if (addr >= 16384 && addr < 17408) {
uint8_t data;
  data = ram[addr - 16384];

return data;
} //fi 
return 0; // a bad default
}
// read8

void write8(uint16_t addr, uint8_t data) {

if (addr >= 0 && addr < 8192 ) {
  // NOP - read only memory
} //fi 
if (addr >= 16384 && addr < 17408 ) {

  ram[addr - 16384] = data;
} //fi 

  // HACK: Screen trap, since we don't have watchers in the C version
#if CHESS_ONLY_OPTIMISATIONS
  if (addr >= 17202 && addr < 17324) {
    extern void memory_trap8cpp(uint16_t addr, uint8_t data);
    memory_trap8cpp(addr, data);
  }
#else
  uint16_t dfile = read16(16396);
  uint16_t vars = read16(16400);
  if (addr >= dfile && addr < vars) {
    extern void memory_trap8cpp(uint16_t addr, uint8_t data);
    memory_trap8cpp(addr, data);
  }

#endif
}
 // write8


/*
**
** Helper methods
**
*/
uint16_t read16(uint16_t addr) {
//   addr = addr.getUnsigned ? addr.getUnsigned() : addr;
  return ((uint16_t)read8(addr+1))*256 + read8(addr);
}
void write16(uint16_t addr, uint16_t data) {
//   addr = addr.getUnsigned ? addr.getUnsigned() : addr;
//   data = data.getUnsigned ? data.getUnsigned() : data;
  write8(addr+1, data>>8);
  write8(addr+0, data&255);
}

/*
**
** State methods
**
*/


};



//

zx81_memory memory;

uint8_t fetch8(uint16_t address) { 
  uint8_t opcode = memory.read8(address&0x7fff);
if (address >= 16384 && address < 17408) {
printf("here: %d", address);
}

  if (address & 0x8000) {
    if (opcode&0x40) {
      // nop : doing other lines of display
    } else {
      opcode = 0;
    }
  }

  return opcode;
}

uint8_t read8(uint16_t a) { return memory.read8(a); }
uint16_t read16(uint16_t a) { return memory.read16(a); }

void write8(uint16_t a, uint8_t d) { memory.write8(a,d); }
void write16(uint16_t a, uint16_t d) { memory.write16(a,d); }
