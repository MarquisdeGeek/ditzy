#include "emf/emf.h"

#include "emulator/z80/common/cpu_z80.h"
#include "emulator/z80/c/zxccpu.h"

#include "platforms/zx81/common/c/zxcio.h"
#include "platforms/zx81/1k/c/mem1k.h"


uint8_t dd_ext();
uint8_t fd_ext();
uint8_t ed_ext();
uint8_t cb_ext();
uint8_t processOpcode();
uint8_t do_z80_nmi();
uint8_t do_z80_interrupt();
uint8_t alu_main_calculateParity(uint8_t v, uint8_t sz) ;
const uint8_t false = 0;
const uint8_t true = 1;

//
#if EMF_CPU_TRACER_SECRET==1
uint8_t memhackblock[20];
#endif

//
emf_u8_t tmp8;
emf_u16_t tmp16;

// let tmp16 = new emf.Number(16);

// // TODO: f = flags, and cc_ are separate but should be unified?!?!
// let f = new emf.Number(8);
emf_u8_t f;

bool cc_bit_0 = 0;
bool cc_bit_1 = 0;
bool cc_bit_2 = 0;
bool cc_bit_3 = 0;
bool cc_bit_4 = 0;
bool cc_bit_5 = 0;
bool cc_bit_6 = 0;
bool cc_bit_7 = 0;


// TODO: XML to contain internal functions (e.g. memory refresh, interupt handlers)
// TODO: XML to differentiate between CPU conventions (imm0) and implementation details (inHalt)
// emfRegisterPair createRegisterPair(emf_u8_t& h, emf_u8_t& l) {
//     return emfRegisterPair(h,l);
    
// }

void setupBusHandlersInternal() {}
void setupBusHandlers() {}
// Options: directMemory = false
// Options: directIORQ = true
// Options: directFetch = false

/*
**
** Register Declarations
**
*/
emf_u8_t a;
emf_u8_t b;
emf_u8_t c;
emf_u8_t d;
emf_u8_t e;
emf_u8_t h;
emf_u8_t l;
emf_u16_t pc;
emf_u16_t sp;
emf_u16_t ix;
emf_u16_t iy ;
emf_u8_t prime_a ;
emf_u8_t prime_b;
emf_u8_t prime_c;
emf_u8_t prime_d;
emf_u8_t prime_e;
emf_u8_t prime_f;
emf_u8_t prime_h;
emf_u8_t prime_l;
emf_u8_t intv;
emf_u8_t memrefresh;
emf_u8_t memr7;

emfRegisterPair af;
emfRegisterPair bc;
emfRegisterPair de;
emfRegisterPair hl;
emfRegisterPair prime_bc;
emfRegisterPair prime_de;
emfRegisterPair prime_hl;

uint8_t fetch8() { return memory_fetch8(emf_u16_get(&pc)); }

void cpu_ctor() {
    emfRegisterPair_init(&af, &a, &f);
    emfRegisterPair_init(&bc, &b, &c);
    emfRegisterPair_init(&de, &d, &e);
    emfRegisterPair_init(&hl, &h, &l);

    emfRegisterPair_init(&prime_bc, &prime_b, &prime_c);
    emfRegisterPair_init(&prime_de, &prime_d, &prime_e);
    emfRegisterPair_init(&prime_hl, &prime_h, &prime_l);


// emfRegisterPair af = createRegisterPair(a, f);
// emfRegisterPair bc = createRegisterPair(b, c);
// emfRegisterPair de = createRegisterPair(d, e);
// emfRegisterPair hl = createRegisterPair(h, l);
// emfRegisterPair prime_bc = createRegisterPair(prime_b, prime_c);
// emfRegisterPair prime_de = createRegisterPair(prime_d, prime_e);
// emfRegisterPair prime_hl = createRegisterPair(prime_h, prime_l);

}
// emfRegisterPair af = createRegisterPair(a, f);
// emfRegisterPair bc = createRegisterPair(b, c);
// emfRegisterPair de = createRegisterPair(d, e);
// emfRegisterPair hl = createRegisterPair(h, l);
// emfRegisterPair prime_bc = createRegisterPair(prime_b, prime_c);
// emfRegisterPair prime_de = createRegisterPair(prime_d, prime_e);
// emfRegisterPair prime_hl = createRegisterPair(prime_h, prime_l);

/*
**
** Internal state
**
*/
bool isBigEndian = 0;
bool inHalt = 0;
uint8_t int_iff0 = 0;
uint8_t int_iff1 = 0;
uint8_t int_iff2 = 0;
uint8_t interruptMode = 0;
bool wasNMIGenerated = 0;
bool wasIRQGenerated = 0;


/*
**
** ALU
**
*/

/*
**
** TODO
**
*/
// The connective tissue between the ALU and CPU:
// * the alu does some calculations that determines results, and side effects (i.e. CC)
// * this then takes those result, and places them in the correct places for the CPU
uint8_t parityLUT8[256];


uint8_t flagHalfCarryAdd[8] = {0, 1, 1, 1, 0, 0, 0, 1};
uint8_t flagHalfCarrySub[8] = {0, 0, 1, 0, 1, 0, 1, 1};

// Common var for many ALU calculations
// TODO: consider whether a local var is better for small memory environments
u32 result;

// utility methods
bool wasCarry;
bool wasNegation;
bool wasOverflow;
bool wasHalfCarry;
bool wasZero;
bool wasSign;
bool wasParity;
//
u32 aluLastResult;




// TODO:
bool unused3() { return false; }
bool unused5() { return false; }
bool subtract() { return false; }

//
bool sign() {
  return wasSign;
}

bool sign16() {
  return wasSign;
}

bool zero() {
  return wasZero;
}

bool halfcarry() {
  return wasHalfCarry;
}

bool overflow() {
  return wasOverflow;
}

bool parity() {
  return wasParity;
}

bool carry() {
  return wasCarry;
}

bool getParity8(int v) {
  return parityLUT8[v];
}

bool getParity16(int v) {
  return parityLUT8[v & 255] ^ parityLUT8[v >> 8];
}




uint8_t computeFlags8(uint8_t r) {
  wasSign = r & 0x80 ? 1 : 0;
  wasZero = r == 0 ? 1 : 0;
  wasParity = getParity8(r);
  return r;
}

uint16_t computeFlags16(uint16_t r) {
  wasSign = r & 0x8000 ? 1 : 0;
  wasZero = r == 0 ? 1 : 0;
  wasParity = getParity16(r);
  return r;
}


/*
**
** ALU hack
**
*/
#include "cpu_alu.c"

void cpu_fromload(uint16_t newsp) {

    emf_u8_assign(&intv, 0x1e);
    emf_u8_assign(&memrefresh, 0xca);
    emf_u16_assign(&pc, 0x0207);
    emf_u16_assign(&sp, newsp);
    emf_u16_assign(&ix, 0x0281);
    emf_u16_assign(&iy, 0x4000);
}


void cpu_getstate(Z80State *pState) {
#if EMF_CPU_DEBUG_STATE
    pState->pc = emf_u16_get(&pc);
    pState->sp = emf_u16_get(&sp);
    pState->ix = emf_u16_get(&ix);
    pState->iy = emf_u16_get(&iy);
    pState->a = emf_u8_get(&a);
    pState->b = emf_u8_get(&b);
    pState->c = emf_u8_get(&c);
    pState->d = emf_u8_get(&d);
    pState->e = emf_u8_get(&e);
    pState->h = emf_u8_get(&h);
    pState->l = emf_u8_get(&l);
    //
    pState->prime_b = emf_u8_get(&prime_b);
    pState->prime_c = emf_u8_get(&prime_c);
    pState->prime_h = emf_u8_get(&prime_h);
    pState->prime_l = emf_u8_get(&prime_l);

#endif
}
/*
**
** Utility methods
**
*/
void cpu_start() {
alu_main_start();
setupBusHandlersInternal();
setupBusHandlers();
return cpu_reset();
}
void cpu_reset() {
  alu_main_reset();
  emf_u8_assign(&a, 0);

  emf_u8_assign(&b, 0);
  emf_u8_assign(&c, 0);
  emf_u8_assign(&d, 0);
  emf_u8_assign(&e, 0);
  emf_u8_assign(&h, 0);
  emf_u8_assign(&l, 0);
  emf_u16_assign(&pc, 519);
  emf_u16_assign(&sp, 17404);
  emf_u16_assign(&ix, 641);
  emf_u16_assign(&iy, 16384);
  emf_u8_assign(&prime_a, 0);
  emf_u8_assign(&prime_b, 0);
  emf_u8_assign(&prime_c, 0);
  emf_u8_assign(&prime_d, 0);
  emf_u8_assign(&prime_e, 0);
  emf_u8_assign(&prime_f, 0);
  emf_u8_assign(&prime_h, 0);
  emf_u8_assign(&prime_l, 0);
  emf_u8_assign(&intv, 30);
  emf_u8_assign(&memrefresh, 202);
  emf_u8_assign(&memr7, 0);
  isBigEndian = (false);
  inHalt = (false);
  int_iff0 = (0);
  int_iff1 = (0);
  int_iff2 = (0);
  interruptMode = (0);
  wasNMIGenerated = (false);
  wasIRQGenerated = (false);
}


bool getFlagC() {
  return cc_bit_0;
}
void clearFlagC() {
  cc_bit_0 = 0;
}
void setFlagC() {
  cc_bit_0 = 1;
}
void affectFlagC() {
  if (carry()) {
    setFlagC();
} else {
    clearFlagC();
  }
}
void changeFlagC(bool newState) {
  if (newState) { // carry
    setFlagC();
} else {
    clearFlagC();
  }
}
bool getFlagN() {
  return cc_bit_1;
}
void clearFlagN() {
  cc_bit_1 = 0;
}
void setFlagN() {
  cc_bit_1 = 1;
}
void affectFlagN() {
  if (subtract()) {
    setFlagN();
} else {
    clearFlagN();
  }
}
void changeFlagN(bool newState) {
  if (newState) { // subtract
    setFlagN();
} else {
    clearFlagN();
  }
}
bool getFlagP() {
  return cc_bit_2;
}
void clearFlagP() {
  cc_bit_2 = 0;
}
void setFlagP() {
  cc_bit_2 = 1;
}
void affectFlagP() {
  if (parity()) {
    setFlagP();
} else {
    clearFlagP();
  }
}
void changeFlagP(bool newState) {
  if (newState) { // parity
    setFlagP();
} else {
    clearFlagP();
  }
}
bool getFlagV() {
  return cc_bit_2;
}
void clearFlagV() {
  cc_bit_2 = 0;
}
void setFlagV() {
  cc_bit_2 = 1;
}
void affectFlagV() {
  if (overflow()) {
    setFlagV();
} else {
    clearFlagV();
  }
}
void changeFlagV(bool newState) {
  if (newState) { // overflow
    setFlagV();
} else {
    clearFlagV();
  }
}
bool getFlagB3() {
  return cc_bit_3;
}
void clearFlagB3() {
  cc_bit_3 = 0;
}
void setFlagB3() {
  cc_bit_3 = 1;
}
void affectFlagB3() {
  if (unused3()) {
    setFlagB3();
} else {
    clearFlagB3();
  }
}
void changeFlagB3(bool newState) {
  if (newState) { // unused3
    setFlagB3();
} else {
    clearFlagB3();
  }
}
bool getFlagH() {
  return cc_bit_4;
}
void clearFlagH() {
  cc_bit_4 = 0;
}
void setFlagH() {
  cc_bit_4 = 1;
}
void affectFlagH() {
  if (halfcarry()) {
    setFlagH();
} else {
    clearFlagH();
  }
}
void changeFlagH(bool newState) {
  if (newState) { // halfcarry
    setFlagH();
} else {
    clearFlagH();
  }
}
bool getFlagB5() {
  return cc_bit_5;
}
void clearFlagB5() {
  cc_bit_5 = 0;
}
void setFlagB5() {
  cc_bit_5 = 1;
}
void affectFlagB5() {
  if (unused5()) {
    setFlagB5();
} else {
    clearFlagB5();
  }
}
void changeFlagB5(bool newState) {
  if (newState) { // unused5
    setFlagB5();
} else {
    clearFlagB5();
  }
}
bool getFlagZ() {
  return cc_bit_6;
}
void clearFlagZ() {
  cc_bit_6 = 0;
}
void setFlagZ() {
  cc_bit_6 = 1;
}
void affectFlagZ() {
  if (zero()) {
    setFlagZ();
} else {
    clearFlagZ();
  }
}
void changeFlagZ(bool newState) {
  if (newState) { // zero
    setFlagZ();
} else {
    clearFlagZ();
  }
}
bool getFlagS() {
  return cc_bit_7;
}
void clearFlagS() {
  cc_bit_7 = 0;
}
void setFlagS() {
  cc_bit_7 = 1;
}
void affectFlagS() {
  if (sign()) {
    setFlagS();
} else {
    clearFlagS();
  }
}
void changeFlagS(bool newState) {
  if (newState) { // sign
    setFlagS();
} else {
    clearFlagS();
  }
}
uint8_t update() {
   // emf.control ensures only 1 step is executed
return  cpu_step();
}

void xferCCBitsToFlagsByte() {
  emf_u8_assign(&f,
    (cc_bit_0 ? 1 : 0) |
    (cc_bit_1 ? 2 : 0) |
    (cc_bit_2 ? 4 : 0) |
    (cc_bit_3 ? 8 : 0) |
    (cc_bit_4 ? 16 : 0) |
    (cc_bit_5 ? 32 : 0) |
    (cc_bit_6 ? 64 : 0) |
    (cc_bit_7 ? 128 : 0) |
    0
  );
}
    
void xferFlagsByteToCCBits() {
  uint8_t v = emf_u8_get(&f);
  cc_bit_0 = (v & 1) ? 1 : 0;
  cc_bit_1 = (v & 2) ? 1 : 0;
  cc_bit_2 = (v & 4) ? 1 : 0;
  cc_bit_3 = (v & 8) ? 1 : 0;
  cc_bit_4 = (v & 16) ? 1 : 0;
  cc_bit_5 = (v & 32) ? 1 : 0;
  cc_bit_6 = (v & 64) ? 1 : 0;
  cc_bit_7 = (v & 128) ? 1 : 0;
}



//
// Special instructions
//
uint8_t alu_rrd8() { // only for (HL)
  uint8_t v = memory_read8(emfRegisterPair_get(&hl));
  uint8_t av = emf_u8_get(&a);
  uint16_t newHL = ((av&0x0f)<<4) | (v>>4);
  uint8_t new_a = (av & 0xf0) | (v&0x0f);
  
  memory_write8(emfRegisterPair_get(&hl),   newHL);
  emf_u8_assign(&a, (new_a));
  
  computeFlags8(new_a);
  return new_a;
}

uint8_t alu_rld8() { // only applies to (hl)
  uint8_t v = memory_read8(emfRegisterPair_get(&hl));
  uint8_t av = emf_u8_get(&a);
  uint16_t newHL = (v<<4) | (av&0x0f);
  uint8_t new_a = (av & 0xf0) | ((v&0xf0)>>4);
  
  memory_write8(emfRegisterPair_get(&hl),  newHL);
  emf_u8_assign(&a, (new_a));
  
  computeFlags8(new_a);
  return new_a;
}

//
// CPU handlers
void halt() {
  inHalt = true;
  // pc.sub(1);// hold still on this instruction, until an NMI hits
  emf_u16_sub(&pc,1);
}
void disableInterrupt() {
  int_iff1=int_iff2=0;
  //interruptEnabled = false;
}
void enableInterrupt() {
  int_iff1=int_iff2=1;
  //interruptEnabled = true;
}
void im0() {
  interruptMode = 0;
}

void im1() {
  interruptMode = 1;
}

void im2() {
  interruptMode = 2;
}

// INTERUPTS : NMI

// returns true if we've handled an interupt
void updateMemoryRefresh() {
  emf_u8_assign(&memrefresh, (emf_u8_get(&memrefresh) + 1) & 0x7f);
}

uint8_t updateInterupts() {
  uint8_t interuptStates = 0;

  if (wasNMIGenerated) {
    interuptStates = do_z80_nmi(); 
    wasNMIGenerated = false;

  } else if (wasIRQGenerated) {
    interuptStates = do_z80_interrupt();
    wasIRQGenerated = false;
  }
  return interuptStates;
}

uint8_t do_z80_interrupt() {
uint8_t tstates = 0;

  if( int_iff1 ) {
	    
	  if( inHalt ) { 
	    emf_u16_add(&pc, 1);
	    inHalt = false;
	  }
	    
	  int_iff1=int_iff2=0;

    emf_u16_sub(&sp, 2);
    memory_write16(emf_u16_get(&sp), emf_u16_get(&pc));

	  updateMemoryRefresh();

	  switch(interruptMode) {
	    case 0:
	    case 1:
	    	emf_u16_assign(&pc, 0x0038);
	    	tstates+=13;
	    	break;
	    case 2: 
		  {
		          // This forms the lower eight-bits of a 16-bit address.
		          uint16_t isr = 0;// TODO  bus.readBlock('int_addr');
		          // I is the upper 8 bits
		          uint16_t ireg = emf_u8_get(&intv);
		          uint16_t vectorAddress = (ireg << 8) | isr;
		          uint16_t intAddress = memory_read16(vectorAddress);
		          //console.log(`IM2 vector to ${vectorAddress}`);
		          //console.log(`IM2 jump to ${intAddress}`);
		          emf_u16_assign(&pc, intAddress);

		    tstates+=19;
		    break;
		  }
	    default: ;
	  }
  }

  return tstates;
}


uint8_t do_z80_nmi() {
uint8_t tstates = 0;
  if (inHalt) { 
    emf_u16_add(&pc, 1);
    inHalt = false;
    tstates += 1;
  }
    
  int_iff2 = int_iff1;
  int_iff1 = 0;

  emf_u16_sub(&sp, 2);
  memory_write16(emf_u16_get(&sp), emf_u16_get(&pc));

  tstates += 11;
  emf_u16_assign(&pc, 0x0066);

  return tstates;
}


uint8_t cpu_step()  {
#ifndef CHESS_ONLY_OPTIMISATIONS
   bool interuptStates = updateInterupts();
   if (interuptStates) {
      return interuptStates;
   }
#endif
   return processOpcode();
}
// Writing from instruction block:
// /emf/res/devices/z80/z80.xml
// Emulation generateProcessMethod
uint8_t processOpcode() {
uint8_t bit;
uint16_t thepc = (emf_u16_get(&pc));
uint8_t opcode = fetch8();
uint8_t cycles = 1;

#ifdef _CMOC_VERSION_
#if EMF_CPU_TRACER_SECRET==1
uint16_t memhack = &memhackblock[0];

poke16(memhack +  0, thepc);
poke(memhack +  2, emf_u8_get(&a));
poke(memhack +  3, emf_u8_get(&b));
poke(memhack +  4, emf_u8_get(&c));
poke(memhack +  5, emf_u8_get(&d));
poke(memhack +  6, emf_u8_get(&e));
poke(memhack +  7, emf_u8_get(&f));
poke(memhack +  8, emf_u8_get(&h));
poke(memhack +  9, emf_u8_get(&l));
poke16(memhack + 10, emf_u16_get(&sp));
poke16(memhack + 12, emf_u16_get(&ix));
poke16(memhack + 14, emf_u16_get(&iy));
poke(memhack + 16, memory_read8(thepc));
poke(memhack + 17, memory_read8(thepc+1));
//
    // Although empty, this check is still compiled. We can therefore break on
    // it to retrieve the state from the secret memory block
  if (thepc==725) {
    // inside keyboard scanning ROM code
  }

#endif

#elif EMF_CPU_TRACER_PRINTF
    printf("Z80 PC = %d(%d/%d) a=%d b=%d c=%d d=%d e=%d f=%d h=%d l=%d sp=%d\n", 
      thepc, memory_read8(thepc), memory_read8(thepc+1),
      emf_u8_get(&a), //a
      emf_u8_get(&b),emf_u8_get(&c),
      emf_u8_get(&d),emf_u8_get(&e),
      emf_u8_get(&f), //a
      emf_u8_get(&h),emf_u8_get(&l),

      emf_u16_get(&sp),
    0);

#endif

switch (opcode) {
case 0: {
cycles += 4;
// nop
// Reference: zaks:82 page 359
;
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 1: {
cycles += 10;
// LD @r,@n
// Reference: zaks:82 page 293
emfRegisterPair_assign(&bc, memory_read16(emf_u16_get(&pc) + (1)));
emf_u16_add(&pc, 3);
 return cycles;
} break;


case 4: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
emf_u8_assign(&b, alu_add_u8u8c(emf_u8_get(&b),1,0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 5: {
cycles += 4;
// DEC @r
// Reference: zaks:82 page 238
emf_u8_assign(&b, alu_sub_u8u8b(emf_u8_get(&b),1,0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 6: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
emf_u8_assign(&b, memory_read8(emf_u16_get(&pc) + 1));
emf_u16_add(&pc, 2);
 return cycles;
} break;
case 7: {
cycles += 4;
// RLCA
// Reference: zaks:82 page 399
emf_u8_assign(&a, alu_rlc8(emf_u8_get(&a)));
clearFlagH();
clearFlagN();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 8: {
cycles += 4;
// EX AF,AF’
// Reference: zaks:82 page 248
emf_u8_assign(&tmp8,  emf_u8_get(&a));
emf_u8_assign(&a, emf_u8_get(&prime_a));
emf_u8_assign(&prime_a, emf_u8_get(&tmp8));
xferCCBitsToFlagsByte();
emf_u8_assign(&tmp8,  emf_u8_get(&f));
emf_u8_assign(&f, emf_u8_get(&prime_f));
emf_u8_assign(&prime_f, emf_u8_get(&tmp8));
xferFlagsByteToCCBits();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 9: {
cycles += 11;
// ADD HL,@r
// Reference: zaks:82 page 203
emfRegisterPair_assign(&hl, alu_add_u16u16c(emfRegisterPair_get(&hl),emfRegisterPair_get(&bc),0));
clearFlagN();
affectFlagH();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 12: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
emf_u8_assign(&c, alu_add_u8u8c(emf_u8_get(&c),1,0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 14: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
emf_u8_assign(&c, memory_read8(emf_u16_get(&pc) + (1)));
emf_u16_add(&pc, 2);
 return cycles;
} break;

case 16: {
cycles += 8;
// DJNZ (emf_u16_get(&pc)+@n)
// Reference: zaks:82 page 245
emf_u8_assign(&b, alu_sub_u8u8b(emf_u8_get(&b),1,0));
if (emf_u8_notEquals(&b, 0)) {
emf_u16_assign(&pc, alu_add_u16s8(emf_u16_get(&pc),memory_read8(emf_u16_get(&pc) + (1)),0));;
cycles += 5;
}
emf_u16_add(&pc, 2);
 return cycles;
} break;
case 17: {
cycles += 10;
// LD @r,@n
// Reference: zaks:82 page 293
emfRegisterPair_assign(&de,  memory_read16(emf_u16_get(&pc) + (1)));
emf_u16_add(&pc, 3);
 return cycles;
} break;
case 18: {
cycles += 7;
// LD (DE),A
// Reference: zaks:82 page 300
memory_write8(emfRegisterPair_get(&de), emf_u8_get(&a));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 20: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
emf_u8_assign(&d, alu_add_u8u8c(emf_u8_get(&d),1,0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 21: {
cycles += 4;
// DEC @r
// Reference: zaks:82 page 238
emf_u8_assign(&d, alu_sub_u8u8b(emf_u8_get(&d),1,0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 22: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
emf_u8_assign(&d, memory_read8(emf_u16_get(&pc) + (1)));
emf_u16_add(&pc, 2);
 return cycles;
} break;
case 23: {
cycles += 4;
// RLA
// Reference: zaks:82 page 398
emf_u8_assign(&a, alu_rl8(emf_u8_get(&a), cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 24: {
cycles += 12;
// JR (emf_u16_get(&pc)+@n)
// Reference: zaks:82 page 290
emf_u16_assign(&pc, alu_add_u16u16(alu_add_u16s8(emf_u16_get(&pc),memory_read8(emf_u16_get(&pc) + (1)),0), 0));
emf_u16_add(&pc, 2);
 return cycles;
} break;
case 25: {
cycles += 11;
// ADD HL,@r
// Reference: zaks:82 page 203
emfRegisterPair_assign(&hl, alu_add_u16u16c(emfRegisterPair_get(&hl),emfRegisterPair_get(&de),0));
clearFlagN();
affectFlagH();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 26: {
cycles += 7;
// LD A,(DE)
// Reference: zaks:82 page 330
emf_u8_assign(&a, memory_read8(emfRegisterPair_get(&de)));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 31: {
cycles += 4;
// RRA
// Reference: zaks:82 page 412
emf_u8_assign(&a, alu_rra8(emf_u8_get(&a), cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 32: {
cycles += 10;
// JR @r,(emf_u16_get(&pc)+@n)
// Reference: zaks:82 page 288
if (cc_bit_6 == 0) {
emf_u16_assign(&pc, alu_add_u16u16(alu_add_u16s8(emf_u16_get(&pc),memory_read8(emf_u16_get(&pc) + (1)),0), 0));
cycles += 5;
}
emf_u16_add(&pc, 2);
 return cycles;
} break;
case 33: {
cycles += 10;
// LD @r,@n
// Reference: zaks:82 page 293
emfRegisterPair_assign(&hl, memory_read16(emf_u16_get(&pc) + (1)));
emf_u16_add(&pc, 3);
 return cycles;
} break;
case 34: {
cycles += 16;
// LD (@n),HL
// Reference: zaks:82 page 323
memory_write16(memory_read16(emf_u16_get(&pc) + (1)), emfRegisterPair_get(&hl));
emf_u16_add(&pc, 3);
 return cycles;
} break;
case 35: {
cycles += 6;
// INC @r
// Reference: zaks:82 page 265
emfRegisterPair_assign(&hl, alu_add_u16u16(emfRegisterPair_get(&hl),1));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 38: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
emf_u8_assign(&h, memory_read8(emf_u16_get(&pc) + (1)));
emf_u16_add(&pc, 2);
 return cycles;
} break;

case 40: {
cycles += 10;
// JR @r,(emf_u16_get(&pc)+@n)
// Reference: zaks:82 page 288
if (cc_bit_6 == 1) {
emf_u16_assign(&pc, alu_add_u16u16(alu_add_u16s8(emf_u16_get(&pc),memory_read8(emf_u16_get(&pc) + (1)),0), 0));
cycles += 5;
}
emf_u16_add(&pc, 2);
 return cycles;
} break;

case 42: {
cycles += 16;
// LD HL,(@n)
// Reference: zaks:82 page 334
emfRegisterPair_assign(&hl, memory_read16((memory_read16(emf_u16_get(&pc) + 1))));
emf_u16_add(&pc, 3);
 return cycles;
} break;
case 43: {
cycles += 6;
// DEC @r
// Reference: zaks:82 page 240
emfRegisterPair_assign(&hl, alu_sub_u16u16(emfRegisterPair_get(&hl),1));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 45: {
cycles += 4;
// DEC @r
// Reference: zaks:82 page 238
emf_u8_assign(&l, alu_sub_u8u8b(emf_u8_get(&l),1,0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 46: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
emf_u8_assign(&l, memory_read8(emf_u16_get(&pc) + (1)));
emf_u16_add(&pc, 2);
 return cycles;
} break;
case 47: {
cycles += 4;
// CPL
// Reference: zaks:82 page 235
emf_u8_assign(&a, alu_complement8(emf_u8_get(&a)));
setFlagH();
setFlagN();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 48: {
cycles += 10;
// JR @r,(emf_u16_get(&pc)+@n)
// Reference: zaks:82 page 288
if (cc_bit_0 == 0) {
emf_u16_assign(&pc, alu_add_u16u16(alu_add_u16s8(emf_u16_get(&pc),memory_read8(emf_u16_get(&pc) + (1)),0), 0));
cycles += 5;
}
emf_u16_add(&pc, 2);
 return cycles;
} break;

case 50: {
cycles += 13;
// LD (@n),A
// Reference: zaks:82 page 319
memory_write8((memory_read16(emf_u16_get(&pc) + (1))), emf_u8_get(&a));
emf_u16_add(&pc, 3);
 return cycles;
} break;

case 52: {
cycles += 11;
// INC (HL)
// Reference: zaks:82 page 267
memory_write8(emfRegisterPair_get(&hl), alu_add_u8u8c(memory_read8(emfRegisterPair_get(&hl)),1,0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 53: {
cycles += 11;
// DEC (HL)
// Reference: zaks:82 page 238
memory_write8(emfRegisterPair_get(&hl), alu_sub_u8u8b(memory_read8(emfRegisterPair_get(&hl)),1,0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 54: {
cycles += 10;
// LD (HL),@n
// Reference: zaks:82 page 301
memory_write8(emfRegisterPair_get(&hl), memory_read8(emf_u16_get(&pc) + (1)));
emf_u16_add(&pc, 2);
 return cycles;
} break;
case 55: {
cycles += 4;
// SCF
// Reference: zaks:82 page 424
;
clearFlagH();
clearFlagN();
setFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 56: {
cycles += 10;
// JR @r,(emf_u16_get(&pc)+@n)
// Reference: zaks:82 page 288
if (cc_bit_0 == 1) {
emf_u16_assign(&pc, alu_add_u16u16(alu_add_u16s8(emf_u16_get(&pc),memory_read8(emf_u16_get(&pc) + (1)),0), 0));
cycles += 5;
}
emf_u16_add(&pc, 2);
 return cycles;
} break;

case 58: {
cycles += 13;
// LD A,(@n)
// Reference: zaks:82 page 317
emf_u8_assign(&a, memory_read8((memory_read16(emf_u16_get(&pc) + 1))));
emf_u16_add(&pc, 3);
 return cycles;
} break;

case 60: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
emf_u8_assign(&a, alu_add_u8u8c(emf_u8_get(&a),1,0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 62: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
emf_u8_assign(&a, memory_read8(emf_u16_get(&pc) + (1)));
emf_u16_add(&pc, 2);
 return cycles;
} break;
case 63: {
cycles += 4;
// CCF
// Reference: zaks:82 page 224
// Reference: The H flag should be set to the old carry, according to http://www.z80.info/z80sflag.htm 
wasHalfCarry=cc_bit_0;
wasCarry=cc_bit_0?0:1;
clearFlagN();
affectFlagH();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 65: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&b, emf_u8_get(&c));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 67: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&b, emf_u8_get(&e));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 68: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&b, emf_u8_get(&h));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 70: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
emf_u8_assign(&b, memory_read8(emfRegisterPair_get(&hl)));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 71: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&b, emf_u8_get(&a));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 72: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&c, emf_u8_get(&b));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 74: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&c, emf_u8_get(&d));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 77: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&c, emf_u8_get(&l));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 78: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
emf_u8_assign(&c, memory_read8(emfRegisterPair_get(&hl)));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 79: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&c, emf_u8_get(&a));
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 85: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&d, emf_u8_get(&l));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 86: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
emf_u8_assign(&d, memory_read8(emfRegisterPair_get(&hl)));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 87: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&d, emf_u8_get(&a));
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 89: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&e, emf_u8_get(&c));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 93: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&e, emf_u8_get(&l));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 94: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
emf_u8_assign(&e, memory_read8(emfRegisterPair_get(&hl)));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 95: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&e, emf_u8_get(&a));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 98: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&h, emf_u8_get(&d));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 103: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&h, emf_u8_get(&a));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 105: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&l, emf_u8_get(&c));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 107: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&l, emf_u8_get(&e));
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 110: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
emf_u8_assign(&l, memory_read8(emfRegisterPair_get(&hl)));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 111: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&l, emf_u8_get(&a));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 112: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
memory_write8(emfRegisterPair_get(&hl), emf_u8_get(&b));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 113: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
memory_write8(emfRegisterPair_get(&hl), emf_u8_get(&c));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 114: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
memory_write8(emfRegisterPair_get(&hl), emf_u8_get(&d));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 115: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
memory_write8(emfRegisterPair_get(&hl), emf_u8_get(&e));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 119: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
memory_write8(emfRegisterPair_get(&hl), emf_u8_get(&a));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 120: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&a, emf_u8_get(&b));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 121: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&a, emf_u8_get(&c));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 122: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&a, emf_u8_get(&d));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 123: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&a, emf_u8_get(&e));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 124: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&a, emf_u8_get(&h));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 125: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
emf_u8_assign(&a, emf_u8_get(&l));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 126: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
emf_u8_assign(&a, memory_read8(emfRegisterPair_get(&hl)));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 128: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
emf_u8_assign(&a, alu_add_u8u8c(emf_u8_get(&a),emf_u8_get(&b),0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 129: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
emf_u8_assign(&a, alu_add_u8u8c(emf_u8_get(&a),emf_u8_get(&c),0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 132: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
emf_u8_assign(&a, alu_add_u8u8c(emf_u8_get(&a),emf_u8_get(&h),0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 133: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
emf_u8_assign(&a, alu_add_u8u8c(emf_u8_get(&a),emf_u8_get(&l),0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 134: {
cycles += 7;
// ADD A,(HL)
// Reference: zaks:82 page 194
emf_u8_assign(&a, alu_add_u8u8c(emf_u8_get(&a),memory_read8(emfRegisterPair_get(&hl)),0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 135: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
emf_u8_assign(&a, alu_add_u8u8c(emf_u8_get(&a),emf_u8_get(&a),0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 144: {
cycles += 4;
// SUB A,@r
// Reference: zaks:82 page 434
emf_u8_assign(&a, alu_sub_u8u8b(emf_u8_get(&a),emf_u8_get(&b),0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 149: {
cycles += 4;
// SUB A,@r
// Reference: zaks:82 page 434
emf_u8_assign(&a, alu_sub_u8u8b(emf_u8_get(&a),emf_u8_get(&l),0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 150: {
cycles += 7;
// SUB A,(HL)
// Reference: zaks:82 page 434
emf_u8_assign(&a, alu_sub_u8u8b(emf_u8_get(&a),memory_read8(emfRegisterPair_get(&hl)),0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 159: {
cycles += 4;
// SBC A,@r
// Reference: zaks:82 page 420
emf_u8_assign(&a, alu_sub_u8u8b(emf_u8_get(&a),emf_u8_get(&a),cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 162: {
cycles += 4;
// AND A,@r
// Reference: zaks:82 page 209
emf_u8_assign(&a, alu_and8(emf_u8_get(&a),emf_u8_get(&d)));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 165: {
cycles += 4;
// AND A,@r
// Reference: zaks:82 page 209
emf_u8_assign(&a, alu_and8(emf_u8_get(&a),emf_u8_get(&l)));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 166: {
cycles += 7;
// AND A,(HL)
// Reference: zaks:82 page 209
emf_u8_assign(&a, alu_and8(emf_u8_get(&a),memory_read8(emfRegisterPair_get(&hl))));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 167: {
cycles += 4;
// AND A,@r
// Reference: zaks:82 page 209
emf_u8_assign(&a, alu_and8(emf_u8_get(&a),emf_u8_get(&a)));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 174: {
cycles += 7;
// XOR A,(HL)
// Reference: zaks:82 page 436
emf_u8_assign(&a, alu_xor8(emf_u8_get(&a),memory_read8(emfRegisterPair_get(&hl))));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 175: {
cycles += 4;
// XOR A,@r
// Reference: zaks:82 page 436
emf_u8_assign(&a, alu_xor8(emf_u8_get(&a),emf_u8_get(&a)));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 176: {
cycles += 4;
// OR A,@r
// Reference: zaks:82 page 360
emf_u8_assign(&a, alu_or8(emf_u8_get(&a),emf_u8_get(&b)));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 184: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
alu_sub_u8u8b(emf_u8_get(&a),emf_u8_get(&b),0);
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 185: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
alu_sub_u8u8b(emf_u8_get(&a),emf_u8_get(&c),0);
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 189: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
  alu_sub_u8u8b(emf_u8_get(&a),emf_u8_get(&l),0);
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 190: {
cycles += 7;
// CP A,(HL)
// Reference: zaks:82 page 225
  alu_sub_u8u8b(emf_u8_get(&a),memory_read8(emfRegisterPair_get(&hl)),0);
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 191: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
  alu_sub_u8u8b(emf_u8_get(&a),emf_u8_get(&a),0);
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 192: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_6 == 0) {
emf_u16_assign(&pc, memory_read16(emf_u16_get(&sp))-1);emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),2)); ;
cycles += 6;
}
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 193: {
cycles += 10;
// POP @r
// Reference: zaks:82 page 373
emfRegisterPair_assign(&bc, memory_read16(emf_u16_get(&sp)));
emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),2));
if (opcode == 0xf1) xferFlagsByteToCCBits();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 195: {
cycles += 10;
// JP (@n)
// Reference: zaks:82 page 284
emf_u16_assign(&pc, alu_add_u16u16(memory_read16(emf_u16_get(&pc) + (1)), -3));
emf_u16_add(&pc, 3);
 return cycles;
} break;
case 196: {
cycles += 10;
// CALL @r,@n
// Reference: zaks:82 page 219
// Reference: This edition describes the P CC as 100, instead of 110
if (cc_bit_6 == 0) {
emf_u16_assign(&sp, alu_sub_u16u16(emf_u16_get(&sp),2));memory_write16(emf_u16_get(&sp),alu_add_u16u16(emf_u16_get(&pc),3));emf_u16_assign(&pc, alu_sub_u16u16(memory_read16(emf_u16_get(&pc) + (1)), 3));;
cycles += 7;
}
emf_u16_add(&pc, 3);
 return cycles;
} break;
case 197: {
cycles += 10;
// PUSH @r
// Reference: zaks:82 page 379
if (opcode == 0xf5) xferCCBitsToFlagsByte();
emf_u16_assign(&sp, alu_sub_u16u16(emf_u16_get(&sp),2));
memory_write16(emf_u16_get(&sp),emfRegisterPair_get(&bc));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 198: {
cycles += 7;
// ADD A,@n
// Reference: zaks:82 page 200
emf_u8_assign(&a, alu_add_u8u8c(emf_u8_get(&a),memory_read8(emf_u16_get(&pc) + (1)),0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 2);
 return cycles;
} break;
case 199: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),-2));
memory_write16(emf_u16_get(&sp),alu_add_u16u16(emf_u16_get(&pc),1));
emf_u16_assign(&pc, alu_add_u16u16(0x00, -1));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 200: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_6 == 1) {
emf_u16_assign(&pc, memory_read16(emf_u16_get(&sp))-1);emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),2)); ;
cycles += 6;
}
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 201: {
cycles += 10;
// RET
// Reference: zaks:82 page 388
emf_u16_assign(&pc, memory_read16(emf_u16_get(&sp))-1);
emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),2));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 202: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_6 == 1) {
emf_u16_assign(&pc, alu_add_u16u16(memory_read16(emf_u16_get(&pc) + (1)),-3));
}
emf_u16_add(&pc, 3);
 return cycles;
} break;
case 203: {
cycles += 0;
// CB
// Reference:  page 
// (extended instruction: This should return cb_ext();
emf_u16_add(&pc, 1);
return cb_ext();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 204: {
cycles += 10;
// CALL @r,@n
// Reference: zaks:82 page 219
// Reference: This edition describes the P CC as 100, instead of 110
if (cc_bit_6 == 1) {
emf_u16_assign(&sp, alu_sub_u16u16(emf_u16_get(&sp),2));
memory_write16(emf_u16_get(&sp),alu_add_u16u16(emf_u16_get(&pc),3));
emf_u16_assign(&pc, alu_sub_u16u16(memory_read16(emf_u16_get(&pc) + (1)), 3));;
cycles += 7;
}
emf_u16_add(&pc, 3);
 return cycles;
} break;
case 205: {
cycles += 17;
// CALL (@n)
// Reference: zaks:82 page 222
emf_u16_assign(&sp, alu_sub_u16u16(emf_u16_get(&sp),2));
memory_write16(emf_u16_get(&sp),alu_add_u16u16(emf_u16_get(&pc),3));
emf_u16_assign(&pc, alu_sub_u16u16(memory_read16(emf_u16_get(&pc) + (1)),3));
emf_u16_add(&pc, 3);
 return cycles;
} break;

case 208: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_0 == 0) {
emf_u16_assign(&pc, memory_read16(emf_u16_get(&sp))-1);emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),2)); ;
cycles += 6;
}
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 209: {
cycles += 10;
// POP @r
// Reference: zaks:82 page 373
emfRegisterPair_assign(&de,  memory_read16(emf_u16_get(&sp)));
emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),2));
if (opcode == 0xf1) xferFlagsByteToCCBits();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 210: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_0 == 0) {
emf_u16_assign(&pc, alu_add_u16u16(memory_read16(emf_u16_get(&pc) + (1)),-3));
}
emf_u16_add(&pc, 3);
 return cycles;
} break;
case 211: {
cycles += 11;
// OUT (@n),A
// Reference: zaks:82 page 368
out8(memory_read8(emf_u16_get(&pc) + (1)), emf_u8_get(&a));
emf_u16_add(&pc, 2);
 return cycles;
} break;

case 213: {
cycles += 10;
// PUSH @r
// Reference: zaks:82 page 379
if (opcode == 0xf5) xferCCBitsToFlagsByte();
emf_u16_assign(&sp, alu_sub_u16u16(emf_u16_get(&sp),2));
memory_write16(emf_u16_get(&sp),emfRegisterPair_get(&de));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 214: {
cycles += 7;
// SUB @n
// Reference: zaks:82 page 434
emf_u8_assign(&a, alu_sub_u8u8b(emf_u8_get(&a),memory_read8(emf_u16_get(&pc) + (1)),0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 2);
 return cycles;
} break;

case 216: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_0 == 1) {
emf_u16_assign(&pc, memory_read16(emf_u16_get(&sp))-1);emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),2)); ;
cycles += 6;
}
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 217: {
cycles += 4;
// EXX
// Reference: zaks:82 page 256
emf_u16_assign(&tmp16, emfRegisterPair_get(&bc));
emfRegisterPair_assign(&bc, emfRegisterPair_get(&prime_bc));
emfRegisterPair_assign(&prime_bc, emf_u16_get(&tmp16));

emf_u16_assign(&tmp16, emfRegisterPair_get(&de));
emfRegisterPair_assign(&de, emfRegisterPair_get(&prime_de));
emfRegisterPair_assign(&prime_de, emf_u16_get(&tmp16));

emf_u16_assign(&tmp16, emfRegisterPair_get(&hl));
emfRegisterPair_assign(&hl, emfRegisterPair_get(&prime_hl));
emfRegisterPair_assign(&prime_hl, emf_u16_get(&tmp16));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 218: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_0 == 1) {
emf_u16_assign(&pc, alu_add_u16u16(memory_read16(emf_u16_get(&pc) + (1)),-3));
}
emf_u16_add(&pc, 3);
 return cycles;
} break;
case 219: {
cycles += 11;
// IN A,(@n)
// Reference: zaks:82 page 263
emf_u8_assign(&a, in8hilo(emf_u8_get(&a), memory_read8(emf_u16_get(&pc) + (1))));


// emf_u8_assign(&a, in8(((uint16_t)emf_u8_get(&a)*(uint16_t)256) | (uint16_t)(memory_read8(emf_u16_get(&pc) + (1)))));
computeFlags8(emf_u8_get(&a));
emf_u16_add(&pc, 2);
 return cycles;
} break;

case 223: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),-2));
memory_write16(emf_u16_get(&sp),alu_add_u16u16(emf_u16_get(&pc),1));
emf_u16_assign(&pc, alu_add_u16u16(0x18, -1));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 225: {
cycles += 10;
// POP @r
// Reference: zaks:82 page 373
emfRegisterPair_assign(&hl, memory_read16(emf_u16_get(&sp)));
emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),2));
if (opcode == 0xf1) xferFlagsByteToCCBits();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 227: {
cycles += 19;
// EX (emf_u16_get(&sp)),HL
// Reference: zaks:82 page 250
emf_u16_assign(&tmp16, emfRegisterPair_get(&hl));
emfRegisterPair_assign(&hl, memory_read16(emf_u16_get(&sp)));
memory_write16(emf_u16_get(&sp),emf_u16_get(&tmp16));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 229: {
cycles += 10;
// PUSH @r
// Reference: zaks:82 page 379
if (opcode == 0xf5) xferCCBitsToFlagsByte();
emf_u16_assign(&sp, alu_sub_u16u16(emf_u16_get(&sp),2));
memory_write16(emf_u16_get(&sp),emfRegisterPair_get(&hl));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 230: {
cycles += 7;
// AND @n
// Reference: zaks:82 page 209
emf_u8_assign(&a, alu_and8(emf_u8_get(&a),memory_read8(emf_u16_get(&pc) + (1))));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 2);
 return cycles;
} break;

case 231: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),-2));
memory_write16(emf_u16_get(&sp),alu_add_u16u16(emf_u16_get(&pc),1));
emf_u16_assign(&pc, alu_add_u16u16(0x20, -1));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 235: {
cycles += 4;
// EX DE,HL
// Reference: zaks:82 page 249
emf_u16_assign(&tmp16, emfRegisterPair_get(&hl));
emfRegisterPair_assign(&hl, emfRegisterPair_get(&de));
emfRegisterPair_assign(&de, emf_u16_get(&tmp16));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 237: {
cycles += 0;

// ED
// Reference:  page 
// (extended instruction: This should return ed_ext();
emf_u16_add(&pc, 1);
return ed_ext();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 239: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),-2));
memory_write16(emf_u16_get(&sp),alu_add_u16u16(emf_u16_get(&pc),1));
emf_u16_assign(&pc, alu_add_u16u16(0x28, -1));
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 241: {
cycles += 10;
// POP @r
// Reference: zaks:82 page 373
emfRegisterPair_assign(&af, memory_read16(emf_u16_get(&sp)));
emf_u16_assign(&sp, alu_add_u16u16(emf_u16_get(&sp),2));
if (opcode == 0xf1) xferFlagsByteToCCBits();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 242: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_7 == 0) {
emf_u16_assign(&pc, alu_add_u16u16(memory_read16(emf_u16_get(&pc) + (1)),-3));
}
emf_u16_add(&pc, 3);
 return cycles;
} break;


case 245: {
cycles += 10;
// PUSH @r
// Reference: zaks:82 page 379
if (opcode == 0xf5) xferCCBitsToFlagsByte();
emf_u16_assign(&sp, alu_sub_u16u16(emf_u16_get(&sp),2));
memory_write16(emf_u16_get(&sp),emfRegisterPair_get(&af));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 246: {
cycles += 7;
// OR A,@n
// Reference: zaks:82 page 360
emf_u8_assign(&a, alu_or8(emf_u8_get(&a),memory_read8(emf_u16_get(&pc) + (1))));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 2);
 return cycles;
} break;

case 253: {
cycles += 0;
// FD
// Reference:  page 
// (extended instruction: This should return fd_ext();
emf_u16_add(&pc, 1);
return fd_ext();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 254: {
cycles += 7;
// CP @n
// Reference: zaks:82 page 225
  alu_sub_u8u8b(emf_u8_get(&a),memory_read8(emf_u16_get(&pc) + (1)),0);
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 2);
 return cycles;
} break;

}
// Even if no instruction was found (maybe, because there's holes in the instructions map), we still want to indicate that time has passed
return 1;
}
// Writing from instruction block:
// /emf/res/devices/z80/z80_dd.xml
// Emulation generateProcessMethod
uint8_t dd_ext() {
// Even if no instruction was found (maybe, because there's holes in the instructions map), we still want to indicate that time has passed
return 1;
}
// Writing from instruction block:
// /emf/res/devices/z80/z80_cb.xml
// Emulation generateProcessMethod
uint8_t cb_ext() {
uint8_t bit;
uint8_t opcode = fetch8();
uint8_t cycles = 1;

switch (opcode) {
case 0: {
cycles += 8;
// RLC @r
// Reference: zaks:82 page 400
emf_u8_assign(&b, alu_rlc8(emf_u8_get(&b)));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 16: {
cycles += 8;
// RL @r
// Reference: zaks:82 page 396
emf_u8_assign(&b, alu_rl8(emf_u8_get(&b), cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 20: {
cycles += 8;
// RL @r
// Reference: zaks:82 page 396
emf_u8_assign(&h, alu_rl8(emf_u8_get(&h), cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 25: {
cycles += 8;
// RR @r
// Reference: zaks:82 page 410
emf_u8_assign(&c, alu_rr8(emf_u8_get(&c), cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 40: {
cycles += 8;
// SRA @r
// Reference: zaks:82 page 430
emf_u8_assign(&b, alu_sra8(emf_u8_get(&b)));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 56: {
cycles += 8;
// SRL @r
// Reference: zaks:82 page 432
emf_u8_assign(&b, alu_srl8(emf_u8_get(&b)));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 69: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
  alu_testBit8(0,emf_u8_get(&l));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 110: {
cycles += 12;
// BIT @r, (HL)
// Reference: zaks:82 page 211
  alu_testBit8(5,(memory_read8(emfRegisterPair_get(&hl))));
clearFlagN();
setFlagH();
affectFlagZ();
affectFlagS();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 112: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
  alu_testBit8(6,emf_u8_get(&b));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 120: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
  alu_testBit8(7,emf_u8_get(&b));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;



case 127: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
  alu_testBit8(7,emf_u8_get(&a));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 169: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
emf_u8_assign(&c, alu_clearBit8(5,emf_u8_get(&c)));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 182: {
cycles += 15;
// RES @r, (HL)
// Reference: zaks:82 page 385
memory_write8(emfRegisterPair_get(&hl),  alu_clearBit8(6,memory_read8(emfRegisterPair_get(&hl))));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 233: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
emf_u8_assign(&c, alu_setBit8(5,emf_u8_get(&c)));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 241: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
emf_u8_assign(&c, alu_setBit8(6,emf_u8_get(&c)));
emf_u16_add(&pc, 1);
 return cycles;
} break;

case 248: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
emf_u8_assign(&b, alu_setBit8(7,emf_u8_get(&b)));
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 249: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
emf_u8_assign(&c, alu_setBit8(7,emf_u8_get(&c)));
emf_u16_add(&pc, 1);
 return cycles;
} break;

}
// Even if no instruction was found (maybe, because there's holes in the instructions map), we still want to indicate that time has passed
return 1;
}
// Writing from instruction block:
// /emf/res/devices/z80/z80_ed.xml
// Emulation generateProcessMethod
uint8_t ed_ext() {
uint8_t bit;
uint8_t opcode = fetch8();
uint8_t cycles = 1;


switch (opcode) {

case 68: {
cycles += 4;
// NEG
// Reference: zaks:82 page 358
emf_u8_assign(&tmp8,  emf_u8_get(&a));
emf_u8_assign(&a, alu_sub_u8u8b(0,emf_u8_get(&a),0));
//if (tmp8.equals(0)) wasCarry=1;/*Zaks error*/;
//if (tmp8.getUnsigned() & (0x80)) wasParity=1;;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 75: {
cycles += 20;
// LD @r,(@n)
// Reference: zaks:82 page 292
emfRegisterPair_assign(&bc, memory_read16((memory_read16(emf_u16_get(&pc) + 1))));
emf_u16_add(&pc, 3);
 return cycles;
} break;


case 83: {
cycles += 15;
// LD (@n),@r
// Reference: zaks:82 page 321
memory_write16(memory_read16(emf_u16_get(&pc) + (1)),emfRegisterPair_get(&de));
emf_u16_add(&pc, 3);
 return cycles;
} break;


case 91: {
cycles += 20;
// LD @r,(@n)
// Reference: zaks:82 page 292
emfRegisterPair_assign(&de, memory_read16((memory_read16(emf_u16_get(&pc) + 1))));
emf_u16_add(&pc, 3);
 return cycles;
} break;


case 114: {
cycles += 15;
// SBC HL,@r
// Reference: zaks:82 page 422
emfRegisterPair_assign(&hl, alu_sub_u16u16b(emfRegisterPair_get(&hl),emf_u16_get(&sp),cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 120: {
cycles += 12;
// IN @r,(C)
// Reference: zaks:82 page 261
// Reference: We don't set the H flag
// emf_u8_assign(&a, in8(emfRegisterPair_get(&bc)));
emf_u8_assign(&a, in8hilo(emf_u8_get(&b), emf_u8_get(&c)));
computeFlags8(emf_u8_get(&a));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 1);
 return cycles;
} break;


case 176: {
cycles += 16;
// LDIR
// Reference: zaks:82 page 354
// Reference: The book indicates to clear the PV flag, but http://www.z80.info/z80sflag.htm suggests otherwise
memory_write8(emfRegisterPair_get(&de),  memory_read8(emfRegisterPair_get(&hl)));
emfRegisterPair_assign(&de, alu_add_u16u16(emfRegisterPair_get(&de),1));
emfRegisterPair_assign(&hl, alu_add_u16u16(emfRegisterPair_get(&hl),1));
emfRegisterPair_assign(&bc, alu_add_u16u16(emfRegisterPair_get(&bc),-1));
if (emfRegisterPair_notEquals(&bc,0)){ emf_u16_sub(&pc, 2); };
if (emfRegisterPair_equals(&bc,0)) wasOverflow=0;else wasOverflow=1;
clearFlagH();
clearFlagN();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;
case 177: {
cycles += 16;
// CPIR
// Reference: zaks:82 page 233
emf_u8_assign(&tmp8,  alu_sub_u8u8b(emf_u8_get(&a),memory_read8(emfRegisterPair_get(&hl)),0));
emfRegisterPair_assign(&hl, alu_add_u16u16(emfRegisterPair_get(&hl), 1));
emfRegisterPair_assign(&bc, alu_add_u16u16(emfRegisterPair_get(&bc),-1));
if (emfRegisterPair_notEquals(&bc,0) && emf_u8_get(&tmp8)!=0) { emf_u16_sub(&pc, 2); } ;
if (emfRegisterPair_equals(&bc,0)) wasOverflow=0;else wasOverflow=1;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
emf_u16_add(&pc, 1);
 return cycles;
} break;

}
// Even if no instruction was found (maybe, because there's holes in the instructions map), we still want to indicate that time has passed
return 1;
}
// Writing from instruction block:
// /emf/res/devices/z80/z80_fd.xml
// Emulation generateProcessMethod
uint8_t fd_ext() {
// uint8_t bit;
uint8_t opcode = fetch8();
uint8_t cycles = 1;


switch (opcode) {

case 119: {
cycles += 19;
// LD (emf_u16_get(&iy)+@n),@r
// Reference: zaks:82 page 315
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0), emf_u8_get(&a));
emf_u16_add(&pc, 2);
 return cycles;
} break;


case 203: {
cycles += 23;
// RLC (emf_u16_get(&iy)+@n)
// Reference: zaks:82 page 406
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xff) == 0x6) {
emf_u8_assign(&tmp8,  alu_rlc8(memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0))));
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0),emf_u8_get(&tmp8));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 3);
 return cycles;
}
cycles += 15;
// RRC (emf_u16_get(&iy)+@n)
// Reference: zaks:82 page 413
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xff) == 0xe) {
emf_u8_assign(&tmp8,  alu_rrc8(memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0))));
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0),emf_u8_get(&tmp8));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 3);
 return cycles;
}
cycles += 15;
// RL (emf_u16_get(&iy)+@n)
// Reference: zaks:82 page 396
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xff) == 0x16) {
emf_u8_assign(&tmp8,  alu_rl8(memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0)), cc_bit_0));
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0),emf_u8_get(&tmp8));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 3);
 return cycles;
}
cycles += 15;
// RR (emf_u16_get(&iy)+@n)
// Reference: zaks:82 page 410
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xff) == 0x1e) {
emf_u8_assign(&tmp8,  alu_rr8(memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0)), cc_bit_0));
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0),emf_u8_get(&tmp8));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 3);
 return cycles;
}
cycles += 15;
// SLA (emf_u16_get(&iy)+@n)
// Reference: zaks:82 page 428
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xff) == 0x26) {
emf_u8_assign(&tmp8,  alu_sla8(memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0))));
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0),emf_u8_get(&tmp8));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 3);
 return cycles;
}
cycles += 15;
// SRA (emf_u16_get(&iy)+@n)
// Reference: zaks:82 page 430
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xff) == 0x2e) {
emf_u8_assign(&tmp8,  alu_sra8(memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0))));
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0),emf_u8_get(&tmp8));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 3);
 return cycles;
}
cycles += 15;
// SLL (emf_u16_get(&iy)+@n)
// Reference:  page 
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xff) == 0x36) {
emf_u8_assign(&tmp8,  alu_sll8(memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0))));
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0),emf_u8_get(&tmp8));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 3);
 return cycles;
}
cycles += 23;
// SRL (emf_u16_get(&iy)+@n)
// Reference: zaks:82 page 432
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xff) == 0x3e) {
emf_u8_assign(&tmp8,  alu_srl8(memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0))));
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0),emf_u8_get(&tmp8));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
emf_u16_add(&pc, 3);
 return cycles;
}
cycles += 20;
// BIT @r,(emf_u16_get(&iy)+@n)
// Reference: zaks:82 page 213
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xc7) == 0x46) {
uint8_t bit = (memory_read8(alu_add_u16u16(emf_u16_get(&pc),2)) & 0x38)>>3;;
emf_u8_assign(&tmp8,  alu_testBit8(bit,memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0))));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
emf_u16_add(&pc, 3);
 return cycles;
}
cycles += 15;
// RES @r,(emf_u16_get(&iy)+@n)
// Reference: zaks:82 page 385
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xc7) == 0x86) {
uint8_t bit = (memory_read8(alu_add_u16u16(emf_u16_get(&pc),2)) & 0x38)>>3;;
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0), alu_clearBit8(bit,memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0))));
emf_u16_add(&pc, 3);
 return cycles;
}
cycles += 23;
// SET @r,(emf_u16_get(&iy)+@n)
// Reference: zaks:82 page 425
if ((memory_read8(emf_u16_get(&pc) + (2))& 0xc7) == 0xc6) {
uint8_t bit = (memory_read8(alu_add_u16u16(emf_u16_get(&pc),2)) & 0x38)>>3;;
memory_write8(alu_add_u16s8(emf_u16_get(&iy), memory_read8(emf_u16_get(&pc) + (1)),0), alu_setBit8(bit,memory_read8(alu_add_u16s8(emf_u16_get(&iy),memory_read8(emf_u16_get(&pc) + (1)),0))));
emf_u16_add(&pc, 3);
 return cycles;
}
} break;

}
// Even if no instruction was found (maybe, because there's holes in the instructions map), we still want to indicate that time has passed
return 1;
}
// importEmulatorALU
// POSTTEST
