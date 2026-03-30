#include "emf/emf.h"

#include "emulator/z80/common/cpu_z80.h"

#include "platforms/zx81/common/cpp/zxio.h"
#include "platforms/zx81/common/zxmemory.h"
#include "platforms/zx81/1k/cpp/zxmemory.h"

#include "emf/framework/comparator.h"


// zx81_z80_emulator
#include "emf/emf.h"
class zx81_z80_emulator {
public:

#include "emulator/z80/cpp/zx81_z80_emulator.cpp.alu.h"
bool subtract() { return false; }
bool unused3() { return false; }


emfNumber<uint8_t> tmp8;
emfNumber<uint16_t> tmp16;

// let tmp8 = new emf.Number(8);
// let tmp16 = new emf.Number(16);

// // TODO: f = flags, and cc_ are separate but should be unified?!?!
// let f = new emf.Number(8);
emfNumber<uint8_t> f;

bool cc_bit_0 = 0;
bool cc_bit_1 = 0;
bool cc_bit_2 = 0;
bool cc_bit_3 = 0;
bool cc_bit_4 = 0;
bool cc_bit_5 = 0;
bool cc_bit_6 = 0;
bool cc_bit_7 = 0;



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


// TODO: XML to contain internal functions (e.g. memory refresh, interupt handlers)
// TODO: XML to differentiate between CPU conventions (imm0) and implementation details (inHalt)
emfRegisterPair createRegisterPair(emfNumber<uint8_t>& h, emfNumber<uint8_t>& l) {
    return emfRegisterPair(h,l);
}

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
emfNumber<uint8_t> a = 0;
emfNumber<uint8_t> gsRegisterA = 8;
emfNumber<uint8_t> b = 0;
emfNumber<uint8_t> gsRegisterB = 8;
emfNumber<uint8_t> c = 0;
emfNumber<uint8_t> gsRegisterC = 8;
emfNumber<uint8_t> d = 0;
emfNumber<uint8_t> gsRegisterD = 8;
emfNumber<uint8_t> e = 0;
emfNumber<uint8_t> gsRegisterE = 8;
emfNumber<uint8_t> h = 0;
emfNumber<uint8_t> gsRegisterH = 8;
emfNumber<uint8_t> l = 0;
emfNumber<uint8_t> gsRegisterL = 8;
emfNumber<uint16_t> pc = 16;
emfNumber<uint16_t> gsRegisterPC = 16;
emfNumber<uint16_t> sp = 16;
emfNumber<uint16_t> gsRegisterSP = 16;
emfNumber<uint16_t> ix = 16;
emfNumber<uint16_t> gsRegisterIX = 16;
emfNumber<uint16_t> iy = 16;
emfNumber<uint16_t> gsRegisterIY = 16;
emfNumber<uint8_t> prime_a = 0;
emfNumber<uint8_t> gsRegisterPRIME_A = 8;
emfNumber<uint8_t> prime_b = 0;
emfNumber<uint8_t> gsRegisterPRIME_B = 8;
emfNumber<uint8_t> prime_c = 0;
emfNumber<uint8_t> gsRegisterPRIME_C = 8;
emfNumber<uint8_t> prime_d = 0;
emfNumber<uint8_t> gsRegisterPRIME_D = 8;
emfNumber<uint8_t> prime_e = 0;
emfNumber<uint8_t> gsRegisterPRIME_E = 8;
emfNumber<uint8_t> prime_f = 0;
emfNumber<uint8_t> gsRegisterPRIME_F = 8;
emfNumber<uint8_t> prime_h = 0;
emfNumber<uint8_t> gsRegisterPRIME_H = 8;
emfNumber<uint8_t> prime_l = 0;
emfNumber<uint8_t> gsRegisterPRIME_L = 8;
emfNumber<uint8_t> intv = 8;
emfNumber<uint8_t> gsRegisterINTV = 8;
emfNumber<uint8_t> memrefresh = 8;
emfNumber<uint8_t> gsRegisterMEMREFRESH = 8;
emfNumber<uint8_t> memr7 = 8;
emfNumber<uint8_t> gsRegisterMEMR7 = 8;
emfRegisterPair af = createRegisterPair(a, f);
emfRegisterPair bc = createRegisterPair(b, c);
emfRegisterPair de = createRegisterPair(d, e);
emfRegisterPair hl = createRegisterPair(h, l);
emfRegisterPair prime_bc = createRegisterPair(prime_b, prime_c);
emfRegisterPair prime_de = createRegisterPair(prime_d, prime_e);
emfRegisterPair prime_hl = createRegisterPair(prime_h, prime_l);

/*
**
** Internal state
**
*/
bool isBigEndian = false;
bool inHalt = false;
uint8_t int_iff0 = 0;
uint8_t int_iff1 = 0;
uint8_t int_iff2 = 0;
uint8_t interruptMode = 0;
bool wasNMIGenerated = false;
bool wasIRQGenerated = false;


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
bool aluLastResult;

// TODO:
bool unused5() { return false; }
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


/*
**
** Utility methods
**
*/
void start() {
alu_main_start();
setupBusHandlersInternal();
setupBusHandlers();
return reset();
}
void reset() {
  alu_main_reset();
  a.assign(0);
  b.assign(0);
  c.assign(0);
  d.assign(0);
  e.assign(0);
  h.assign(0);
  l.assign(0);
  pc.assign(519);
  sp.assign(17404);
  ix.assign(641);
  iy.assign(16384);
  prime_a.assign(0);
  prime_b.assign(0);
  prime_c.assign(0);
  prime_d.assign(0);
  prime_e.assign(0);
  prime_f.assign(0);
  prime_h.assign(0);
  prime_l.assign(0);
  intv.assign(30);
  memrefresh.assign(202);
  memr7.assign(0);
  isBigEndian = (false);
  inHalt = (false);
  int_iff0 = (0);
  int_iff1 = (0);
  int_iff2 = (0);
  interruptMode = (0);
  wasNMIGenerated = (false);
  wasIRQGenerated = (false);
}
uint8_t getRegisterValueA() {
  return a.getUnsigned();
}
void setRegisterValueA(uint8_t v) {
   a.assign(v);
}
uint8_t getRegisterValueB() {
  return b.getUnsigned();
}
void setRegisterValueB(uint8_t v) {
   b.assign(v);
}
uint8_t getRegisterValueC() {
  return c.getUnsigned();
}
void setRegisterValueC(uint8_t v) {
   c.assign(v);
}
uint8_t getRegisterValueD() {
  return d.getUnsigned();
}
void setRegisterValueD(uint8_t v) {
   d.assign(v);
}
uint8_t getRegisterValueE() {
  return e.getUnsigned();
}
void setRegisterValueE(uint8_t v) {
   e.assign(v);
}
uint8_t getRegisterValueH() {
  return h.getUnsigned();
}
void setRegisterValueH(uint8_t v) {
   h.assign(v);
}
uint8_t getRegisterValueL() {
  return l.getUnsigned();
}
void setRegisterValueL(uint8_t v) {
   l.assign(v);
}
uint16_t getRegisterValuePC() {
  return pc.getUnsigned();
}
void setRegisterValuePC(uint16_t v) {
   pc.assign(v);
}
uint16_t getRegisterValueSP() {
  return sp.getUnsigned();
}
void setRegisterValueSP(uint16_t v) {
   sp.assign(v);
}
uint16_t getRegisterValueIX() {
  return ix.getUnsigned();
}
void setRegisterValueIX(uint16_t v) {
   ix.assign(v);
}
uint16_t getRegisterValueIY() {
  return iy.getUnsigned();
}
void setRegisterValueIY(uint16_t v) {
   iy.assign(v);
}
uint8_t getRegisterValuePRIME_A() {
  return prime_a.getUnsigned();
}
void setRegisterValuePRIME_A(uint8_t v) {
   prime_a.assign(v);
}
uint8_t getRegisterValuePRIME_B() {
  return prime_b.getUnsigned();
}
void setRegisterValuePRIME_B(uint8_t v) {
   prime_b.assign(v);
}
uint8_t getRegisterValuePRIME_C() {
  return prime_c.getUnsigned();
}
void setRegisterValuePRIME_C(uint8_t v) {
   prime_c.assign(v);
}
uint8_t getRegisterValuePRIME_D() {
  return prime_d.getUnsigned();
}
void setRegisterValuePRIME_D(uint8_t v) {
   prime_d.assign(v);
}
uint8_t getRegisterValuePRIME_E() {
  return prime_e.getUnsigned();
}
void setRegisterValuePRIME_E(uint8_t v) {
   prime_e.assign(v);
}
uint8_t getRegisterValuePRIME_F() {
  return prime_f.getUnsigned();
}
void setRegisterValuePRIME_F(uint8_t v) {
   prime_f.assign(v);
}
uint8_t getRegisterValuePRIME_H() {
  return prime_h.getUnsigned();
}
void setRegisterValuePRIME_H(uint8_t v) {
   prime_h.assign(v);
}
uint8_t getRegisterValuePRIME_L() {
  return prime_l.getUnsigned();
}
void setRegisterValuePRIME_L(uint8_t v) {
   prime_l.assign(v);
}
uint8_t getRegisterValueINTV() {
  return intv.getUnsigned();
}
void setRegisterValueINTV(uint8_t v) {
   intv.assign(v);
}
uint8_t getRegisterValueMEMREFRESH() {
  return memrefresh.getUnsigned();
}
void setRegisterValueMEMREFRESH(uint8_t v) {
   memrefresh.assign(v);
}
uint8_t getRegisterValueMEMR7() {
  return memr7.getUnsigned();
}
void setRegisterValueMEMR7(uint8_t v) {
   memr7.assign(v);
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
return  step();
}

void xferCCBitsToFlagsByte() {
  f =
    (cc_bit_0 ? 1 : 0) |
    (cc_bit_1 ? 2 : 0) |
    (cc_bit_2 ? 4 : 0) |
    (cc_bit_3 ? 8 : 0) |
    (cc_bit_4 ? 16 : 0) |
    (cc_bit_5 ? 32 : 0) |
    (cc_bit_6 ? 64 : 0) |
    (cc_bit_7 ? 128 : 0) |
    0
  ;
}
    
void xferFlagsByteToCCBits() {
  auto v = f;
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
  uint8_t v = read8(hl);
  uint8_t av = a;
  uint16_t newHL = ((av&0x0f)<<4) | (v>>4);
  uint8_t new_a = (av & 0xf0) | (v&0x0f);
  
  write8(hl, newHL);
  a=(new_a);
  
  computeFlags8(new_a);
  return new_a;
}

uint8_t alu_rld8() { // only applies to (hl)
  uint8_t v = read8(hl);
  uint8_t av = a;
  uint16_t newHL = (v<<4) | (av&0x0f);
  uint8_t new_a = (av & 0xf0) | ((v&0xf0)>>4);
  
  write8(hl, newHL);
  a=(new_a);
  
  computeFlags8(new_a);
  return new_a;
}

//
// CPU handlers
void halt() {
  inHalt = true;
  // pc.sub(1);// hold still on this instruction, until an NMI hits
  pc -= 1;
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
  memrefresh.assign((memrefresh.getUnsigned() + 1) & 0x7f);
}
/*
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
*/
uint8_t do_z80_interrupt() {
uint8_t tstates = 0;

  if( int_iff1 ) {
	    
	  if( inHalt ) { 
	    pc.add(1);
	    inHalt = false;
	  }
	    
	  int_iff1=int_iff2=0;

	  sp.sub(2);
	  write16(sp.getUnsigned(), pc.getUnsigned());

	  updateMemoryRefresh();

	  switch(interruptMode) {
	    case 0:
	    case 1:
	    	pc.assign(0x0038);
	    	tstates+=13;
	    	break;
	    case 2: 
		  {
		          // This forms the lower eight-bits of a 16-bit address.
		          uint16_t isr = 0;//TODO: auto-creation of blocks, and impl bus.readBlock('int_addr');
		          // I is the upper 8 bits
		          uint16_t ireg = intv.getUnsigned();
		          uint16_t vectorAddress = (ireg << 8) | isr;
		          uint16_t intAddress = read16(vectorAddress);
		          //console.log(`IM2 vector to ${vectorAddress}`);
		          //console.log(`IM2 jump to ${intAddress}`);
		          pc.assign(intAddress);

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
    pc.add(1);
    inHalt = false;
    tstates += 1;
  }
    
  int_iff2 = int_iff1;
  int_iff1 = 0;

  sp.sub(2);
  write16(sp.getUnsigned(), pc.getUnsigned());

  tstates += 11;
  pc.assign(0x0066);

  return tstates;
}

bool updateInterupts() {
    	
    if (!wasNMIGenerated && inHalt) {
        do_z80_interrupt();
    } else if (wasIRQGenerated) {
        do_z80_interrupt();
        wasIRQGenerated = false;
    } else if (wasNMIGenerated) {
        do_z80_nmi(); 
        return true;
    }

    return false;
  }

uint8_t step()  {
   bool interuptStates = updateInterupts();
   if (interuptStates) {
      return interuptStates;
   }

   return processOpcode();
}
// Writing from instruction block:
// /emf/res/devices/z80/z80.xml
// Emulation generateProcessMethod
uint8_t processOpcode() {
uint8_t bit;
uint8_t opcode = fetch8(pc);
uint8_t cycles = 1;


switch (opcode) {
case 0: {
cycles += 4;
// nop
// Reference: zaks:82 page 359
;
pc.add(1);
 return cycles;
} break;
case 1: {
cycles += 10;
// LD @r,@n
// Reference: zaks:82 page 293
bc.assign(read16(pc.getUnsigned() + (1)));
pc.add(3);
 return cycles;
} break;
case 2: {
cycles += 7;
// LD (BC),A
// Reference: zaks:82 page 299
write8(bc, a);
pc.add(1);
 return cycles;
} break;
case 3: {
cycles += 6;
// INC @r
// Reference: zaks:82 page 265
bc.assign(alu_add_u16u16(bc,1));
pc.add(1);
 return cycles;
} break;
case 4: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
b.assign(alu_add_u8u8c(b,1));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 5: {
cycles += 4;
// DEC @r
// Reference: zaks:82 page 238
b.assign(alu_sub_u8u8b(b,1));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 6: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
b.assign(read8(pc.getUnsigned() + (1)));
pc.add(2);
 return cycles;
} break;
case 7: {
cycles += 4;
// RLCA
// Reference: zaks:82 page 399
a.assign(alu_rlc8(a));
clearFlagH();
clearFlagN();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 8: {
cycles += 4;
// EX AF,AF’
// Reference: zaks:82 page 248
tmp8.assign(a);
a.assign(prime_a);
prime_a.assign(tmp8);
xferCCBitsToFlagsByte();
tmp8.assign(f);
f.assign(prime_f);
xferFlagsByteToCCBits();
prime_f.assign(tmp8);
pc.add(1);
 return cycles;
} break;
case 9: {
cycles += 11;
// ADD HL,@r
// Reference: zaks:82 page 203
hl.assign(alu_add_u16u16c(hl,bc));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 10: {
cycles += 7;
// LD A,(BC)
// Reference: zaks:82 page 329
a.assign(read8(bc));
pc.add(1);
 return cycles;
} break;
case 11: {
cycles += 6;
// DEC @r
// Reference: zaks:82 page 240
bc.assign(alu_sub_u16u16(bc,1));
pc.add(1);
 return cycles;
} break;
case 12: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
c.assign(alu_add_u8u8c(c,1));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 13: {
cycles += 4;
// DEC @r
// Reference: zaks:82 page 238
c.assign(alu_sub_u8u8b(c,1));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 14: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
c.assign(read8(pc.getUnsigned() + (1)));
pc.add(2);
 return cycles;
} break;
case 15: {
cycles += 4;
// RRCA
// Reference: zaks:82 page 415
a.assign(alu_rrc8(a));
clearFlagH();
clearFlagN();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 16: {
cycles += 8;
// DJNZ (PC+@n)
// Reference: zaks:82 page 245
b.assign(alu_sub_u8u8b(b,1));
if (b.notEquals(0)) {
pc.assign(alu_add_u16s8(pc,read8(pc.getUnsigned() + (1))));;
cycles += 5;
}
pc.add(2);
 return cycles;
} break;
case 17: {
cycles += 10;
// LD @r,@n
// Reference: zaks:82 page 293
de.assign(read16(pc.getUnsigned() + (1)));
pc.add(3);
 return cycles;
} break;
case 18: {
cycles += 7;
// LD (DE),A
// Reference: zaks:82 page 300
write8(de,a);
pc.add(1);
 return cycles;
} break;
case 19: {
cycles += 6;
// INC @r
// Reference: zaks:82 page 265
de.assign(alu_add_u16u16(de,1));
pc.add(1);
 return cycles;
} break;
case 20: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
d.assign(alu_add_u8u8c(d,1));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 21: {
cycles += 4;
// DEC @r
// Reference: zaks:82 page 238
d.assign(alu_sub_u8u8b(d,1));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 22: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
d.assign(read8(pc.getUnsigned() + (1)));
pc.add(2);
 return cycles;
} break;
case 23: {
cycles += 4;
// RLA
// Reference: zaks:82 page 398
a.assign(alu_rl8(a, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 24: {
cycles += 12;
// JR (PC+@n)
// Reference: zaks:82 page 290
pc.assign(alu_add_u16u16(alu_add_u16s8(pc,read8(pc.getUnsigned() + (1))), 0));
pc.add(2);
 return cycles;
} break;
case 25: {
cycles += 11;
// ADD HL,@r
// Reference: zaks:82 page 203
hl.assign(alu_add_u16u16c(hl,de));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 26: {
cycles += 7;
// LD A,(DE)
// Reference: zaks:82 page 330
a.assign(read8(de));
pc.add(1);
 return cycles;
} break;
case 27: {
cycles += 6;
// DEC @r
// Reference: zaks:82 page 240
de.assign(alu_sub_u16u16(de,1));
pc.add(1);
 return cycles;
} break;
case 28: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
e.assign(alu_add_u8u8c(e,1));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 29: {
cycles += 4;
// DEC @r
// Reference: zaks:82 page 238
e.assign(alu_sub_u8u8b(e,1));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 30: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
e.assign(read8(pc.getUnsigned() + (1)));
pc.add(2);
 return cycles;
} break;
case 31: {
cycles += 4;
// RRA
// Reference: zaks:82 page 412
a.assign(alu_rra8(a, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 32: {
cycles += 10;
// JR @r,(PC+@n)
// Reference: zaks:82 page 288
if (cc_bit_6 == 0) {
pc.assign(alu_add_u16u16(alu_add_u16s8(pc,read8(pc.getUnsigned() + (1))), 0));
cycles += 5;
} else {
pc.add(0);
}
pc.add(2);
 return cycles;
} break;
case 33: {
cycles += 10;
// LD @r,@n
// Reference: zaks:82 page 293
hl.assign(read16(pc.getUnsigned() + (1)));
pc.add(3);
 return cycles;
} break;
case 34: {
cycles += 16;
// LD (@n),HL
// Reference: zaks:82 page 323
write16(read16(pc.getUnsigned() + (1)), hl);
pc.add(3);
 return cycles;
} break;
case 35: {
cycles += 6;
// INC @r
// Reference: zaks:82 page 265
hl.assign(alu_add_u16u16(hl,1));
pc.add(1);
 return cycles;
} break;
case 36: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
h.assign(alu_add_u8u8c(h,1));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 37: {
cycles += 4;
// DEC @r
// Reference: zaks:82 page 238
h.assign(alu_sub_u8u8b(h,1));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 38: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
h.assign(read8(pc.getUnsigned() + (1)));
pc.add(2);
 return cycles;
} break;
case 39: {
cycles += 4;
// DAA
// Reference: zaks:82 page 236
a.assign(alu_daa(a, cc_bit_0, cc_bit_1));
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 40: {
cycles += 10;
// JR @r,(PC+@n)
// Reference: zaks:82 page 288
if (cc_bit_6 == 1) {
pc.assign(alu_add_u16u16(alu_add_u16s8(pc,read8(pc.getUnsigned() + (1))), 0));
cycles += 5;
}
pc.add(2);
 return cycles;
} break;
case 41: {
cycles += 11;
// ADD HL,@r
// Reference: zaks:82 page 203
hl.assign(alu_add_u16u16c(hl,hl));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 42: {
cycles += 16;
// LD HL,(@n)
// Reference: zaks:82 page 334
hl.assign(read16(read16(pc.getUnsigned() + (1))));
pc.add(3);
 return cycles;
} break;
case 43: {
cycles += 6;
// DEC @r
// Reference: zaks:82 page 240
hl.assign(alu_sub_u16u16(hl,1));
pc.add(1);
 return cycles;
} break;
case 44: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
l.assign(alu_add_u8u8c(l,1));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 45: {
cycles += 4;
// DEC @r
// Reference: zaks:82 page 238
l.assign(alu_sub_u8u8b(l,1));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 46: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
l.assign(read8(pc.getUnsigned() + (1)));
pc.add(2);
 return cycles;
} break;
case 47: {
cycles += 4;
// CPL
// Reference: zaks:82 page 235
a.assign(alu_complement8(a));
setFlagH();
setFlagN();
pc.add(1);
 return cycles;
} break;
case 48: {
cycles += 10;
// JR @r,(PC+@n)
// Reference: zaks:82 page 288
if (cc_bit_0 == 0) {
pc.assign(alu_add_u16u16(alu_add_u16s8(pc,read8(pc.getUnsigned() + (1))), 0));
cycles += 5;
}
pc.add(2);
 return cycles;
} break;
case 49: {
cycles += 10;
// LD @r,@n
// Reference: zaks:82 page 293
sp.assign(read16(pc.getUnsigned() + (1)));
pc.add(3);
 return cycles;
} break;
case 50: {
cycles += 13;
// LD (@n),A
// Reference: zaks:82 page 319
write8(read16(pc.getUnsigned() + (1)),a);
pc.add(3);
 return cycles;
} break;
case 51: {
cycles += 6;
// INC @r
// Reference: zaks:82 page 265
sp.assign(alu_add_u16u16(sp,1));
pc.add(1);
 return cycles;
} break;
case 52: {
cycles += 11;
// INC (HL)
// Reference: zaks:82 page 267
write8(hl,alu_add_u8u8c(read8(hl),1));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 53: {
cycles += 11;
// DEC (HL)
// Reference: zaks:82 page 238
write8(hl,alu_sub_u8u8b(read8(hl),1));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 54: {
cycles += 10;
// LD (HL),@n
// Reference: zaks:82 page 301
write8(hl,read8(pc.getUnsigned() + (1)));
pc.add(2);
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
pc.add(1);
 return cycles;
} break;
case 56: {
cycles += 10;
// JR @r,(PC+@n)
// Reference: zaks:82 page 288
if (cc_bit_0 == 1) {
pc.assign(alu_add_u16u16(alu_add_u16s8(pc,read8(pc.getUnsigned() + (1))), 0));
cycles += 5;
}
pc.add(2);
 return cycles;
} break;
case 57: {
cycles += 11;
// ADD HL,@r
// Reference: zaks:82 page 203
hl.assign(alu_add_u16u16c(hl,sp));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 58: {
cycles += 13;
// LD A,(@n)
// Reference: zaks:82 page 317
a.assign(read8(read16(pc.getUnsigned() + (1))));
pc.add(3);
 return cycles;
} break;
case 59: {
cycles += 6;
// DEC @r
// Reference: zaks:82 page 240
sp.assign(alu_sub_u16u16(sp,1));
pc.add(1);
 return cycles;
} break;
case 60: {
cycles += 4;
// INC @r
// Reference: zaks:82 page 264
a.assign(alu_add_u8u8c(a,1));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 61: {
cycles += 4;
// DEC @r
// Reference: zaks:82 page 238
a.assign(alu_sub_u8u8b(a,1));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 62: {
cycles += 7;
// LD @r,@n
// Reference: zaks:82 page 295
a.assign(read8(pc.getUnsigned() + (1)));
pc.add(2);
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
pc.add(1);
 return cycles;
} break;
case 64: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
b.assign(b);
pc.add(1);
 return cycles;
} break;
case 65: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
b.assign(c);
pc.add(1);
 return cycles;
} break;
case 66: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
b.assign(d);
pc.add(1);
 return cycles;
} break;
case 67: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
b.assign(e);
pc.add(1);
 return cycles;
} break;
case 68: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
b.assign(h);
pc.add(1);
 return cycles;
} break;
case 69: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
b.assign(l);
pc.add(1);
 return cycles;
} break;
case 70: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
b.assign(read8(hl));
pc.add(1);
 return cycles;
} break;
case 71: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
b.assign(a);
pc.add(1);
 return cycles;
} break;
case 72: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
c.assign(b);
pc.add(1);
 return cycles;
} break;
case 73: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
c.assign(c);
pc.add(1);
 return cycles;
} break;
case 74: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
c.assign(d);
pc.add(1);
 return cycles;
} break;
case 75: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
c.assign(e);
pc.add(1);
 return cycles;
} break;
case 76: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
c.assign(h);
pc.add(1);
 return cycles;
} break;
case 77: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
c.assign(l);
pc.add(1);
 return cycles;
} break;
case 78: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
c.assign(read8(hl));
pc.add(1);
 return cycles;
} break;
case 79: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
c.assign(a);
pc.add(1);
 return cycles;
} break;
case 80: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
d.assign(b);
pc.add(1);
 return cycles;
} break;
case 81: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
d.assign(c);
pc.add(1);
 return cycles;
} break;
case 82: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
d.assign(d);
pc.add(1);
 return cycles;
} break;
case 83: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
d.assign(e);
pc.add(1);
 return cycles;
} break;
case 84: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
d.assign(h);
pc.add(1);
 return cycles;
} break;
case 85: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
d.assign(l);
pc.add(1);
 return cycles;
} break;
case 86: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
d.assign(read8(hl));
pc.add(1);
 return cycles;
} break;
case 87: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
d.assign(a);
pc.add(1);
 return cycles;
} break;
case 88: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
e.assign(b);
pc.add(1);
 return cycles;
} break;
case 89: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
e.assign(c);
pc.add(1);
 return cycles;
} break;
case 90: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
e.assign(d);
pc.add(1);
 return cycles;
} break;
case 91: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
e.assign(e);
pc.add(1);
 return cycles;
} break;
case 92: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
e.assign(h);
pc.add(1);
 return cycles;
} break;
case 93: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
e.assign(l);
pc.add(1);
 return cycles;
} break;
case 94: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
e.assign(read8(hl));
pc.add(1);
 return cycles;
} break;
case 95: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
e.assign(a);
pc.add(1);
 return cycles;
} break;
case 96: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
h.assign(b);
pc.add(1);
 return cycles;
} break;
case 97: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
h.assign(c);
pc.add(1);
 return cycles;
} break;
case 98: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
h.assign(d);
pc.add(1);
 return cycles;
} break;
case 99: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
h.assign(e);
pc.add(1);
 return cycles;
} break;
case 100: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
h.assign(h);
pc.add(1);
 return cycles;
} break;
case 101: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
h.assign(l);
pc.add(1);
 return cycles;
} break;
case 102: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
h.assign(read8(hl));
pc.add(1);
 return cycles;
} break;
case 103: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
h.assign(a);
pc.add(1);
 return cycles;
} break;
case 104: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
l.assign(b);
pc.add(1);
 return cycles;
} break;
case 105: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
l.assign(c);
pc.add(1);
 return cycles;
} break;
case 106: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
l.assign(d);
pc.add(1);
 return cycles;
} break;
case 107: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
l.assign(e);
pc.add(1);
 return cycles;
} break;
case 108: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
l.assign(h);
pc.add(1);
 return cycles;
} break;
case 109: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
l.assign(l);
pc.add(1);
 return cycles;
} break;
case 110: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
l.assign(read8(hl));
pc.add(1);
 return cycles;
} break;
case 111: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
l.assign(a);
pc.add(1);
 return cycles;
} break;
case 112: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
write8(hl,b);
pc.add(1);
 return cycles;
} break;
case 113: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
write8(hl,c);
pc.add(1);
 return cycles;
} break;
case 114: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
write8(hl,d);
pc.add(1);
 return cycles;
} break;
case 115: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
write8(hl,e);
pc.add(1);
 return cycles;
} break;
case 116: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
write8(hl,h);
pc.add(1);
 return cycles;
} break;
case 117: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
write8(hl,l);
pc.add(1);
 return cycles;
} break;
case 118: {
cycles += 7;
// HALT
// Reference: zaks:82 page 257
halt();
pc.add(1);
 return cycles;
} break;
case 119: {
cycles += 7;
// LD (HL),@r
// Reference: zaks:82 page 303
write8(hl,a);
pc.add(1);
 return cycles;
} break;
case 120: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
a.assign(b);
pc.add(1);
 return cycles;
} break;
case 121: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
a.assign(c);
pc.add(1);
 return cycles;
} break;
case 122: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
a.assign(d);
pc.add(1);
 return cycles;
} break;
case 123: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
a.assign(e);
pc.add(1);
 return cycles;
} break;
case 124: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
a.assign(h);
pc.add(1);
 return cycles;
} break;
case 125: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
a.assign(l);
pc.add(1);
 return cycles;
} break;
case 126: {
cycles += 7;
// LD @r,(HL)
// Reference: zaks:82 page 356
a.assign(read8(hl));
pc.add(1);
 return cycles;
} break;
case 127: {
cycles += 4;
// LD @r,@s
// Reference: zaks:82 page 297
a.assign(a);
pc.add(1);
 return cycles;
} break;
case 128: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
a.assign(alu_add_u8u8c(a,b));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 129: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
a.assign(alu_add_u8u8c(a,c));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 130: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
a.assign(alu_add_u8u8c(a,d));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 131: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
a.assign(alu_add_u8u8c(a,e));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 132: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
a.assign(alu_add_u8u8c(a,h));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 133: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
a.assign(alu_add_u8u8c(a,l));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 134: {
cycles += 7;
// ADD A,(HL)
// Reference: zaks:82 page 194
a.assign(alu_add_u8u8c(a,read8(hl)));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 135: {
cycles += 4;
// ADD A,@r
// Reference: zaks:82 page 201
a.assign(alu_add_u8u8c(a,a));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 136: {
cycles += 4;
// ADC A,@r
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,b,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 137: {
cycles += 4;
// ADC A,@r
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,c,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 138: {
cycles += 4;
// ADC A,@r
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,d,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 139: {
cycles += 4;
// ADC A,@r
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,e,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 140: {
cycles += 4;
// ADC A,@r
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,h,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 141: {
cycles += 4;
// ADC A,@r
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,l,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 142: {
cycles += 7;
// ADC A,(HL)
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,read8(hl),cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 143: {
cycles += 4;
// ADC A,@r
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,a,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 144: {
cycles += 4;
// SUB A,@r
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,b));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 145: {
cycles += 4;
// SUB A,@r
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,c));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 146: {
cycles += 4;
// SUB A,@r
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,d));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 147: {
cycles += 4;
// SUB A,@r
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,e));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 148: {
cycles += 4;
// SUB A,@r
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,h));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 149: {
cycles += 4;
// SUB A,@r
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,l));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 150: {
cycles += 7;
// SUB A,(HL)
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,read8(hl)));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 151: {
cycles += 4;
// SUB A,@r
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,a));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 152: {
cycles += 4;
// SBC A,@r
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,b,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 153: {
cycles += 4;
// SBC A,@r
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,c,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 154: {
cycles += 4;
// SBC A,@r
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,d,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 155: {
cycles += 4;
// SBC A,@r
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,e,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 156: {
cycles += 4;
// SBC A,@r
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,h,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 157: {
cycles += 4;
// SBC A,@r
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,l,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 158: {
cycles += 7;
// SBC A,(HL)
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,read8(hl),cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 159: {
cycles += 4;
// SBC A,@r
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,a,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 160: {
cycles += 4;
// AND A,@r
// Reference: zaks:82 page 209
a.assign(alu_and8(a,b));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 161: {
cycles += 4;
// AND A,@r
// Reference: zaks:82 page 209
a.assign(alu_and8(a,c));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 162: {
cycles += 4;
// AND A,@r
// Reference: zaks:82 page 209
a.assign(alu_and8(a,d));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 163: {
cycles += 4;
// AND A,@r
// Reference: zaks:82 page 209
a.assign(alu_and8(a,e));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 164: {
cycles += 4;
// AND A,@r
// Reference: zaks:82 page 209
a.assign(alu_and8(a,h));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 165: {
cycles += 4;
// AND A,@r
// Reference: zaks:82 page 209
a.assign(alu_and8(a,l));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 166: {
cycles += 7;
// AND A,(HL)
// Reference: zaks:82 page 209
a.assign(alu_and8(a,read8(hl)));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 167: {
cycles += 4;
// AND A,@r
// Reference: zaks:82 page 209
a.assign(alu_and8(a,a));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 168: {
cycles += 4;
// XOR A,@r
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,b));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 169: {
cycles += 4;
// XOR A,@r
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,c));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 170: {
cycles += 4;
// XOR A,@r
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,d));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 171: {
cycles += 4;
// XOR A,@r
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,e));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 172: {
cycles += 4;
// XOR A,@r
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,h));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 173: {
cycles += 4;
// XOR A,@r
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,l));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 174: {
cycles += 7;
// XOR A,(HL)
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,read8(hl)));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 175: {
cycles += 4;
// XOR A,@r
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,a));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 176: {
cycles += 4;
// OR A,@r
// Reference: zaks:82 page 360
a.assign(alu_or8(a,b));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 177: {
cycles += 4;
// OR A,@r
// Reference: zaks:82 page 360
a.assign(alu_or8(a,c));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 178: {
cycles += 4;
// OR A,@r
// Reference: zaks:82 page 360
a.assign(alu_or8(a,d));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 179: {
cycles += 4;
// OR A,@r
// Reference: zaks:82 page 360
a.assign(alu_or8(a,e));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 180: {
cycles += 4;
// OR A,@r
// Reference: zaks:82 page 360
a.assign(alu_or8(a,h));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 181: {
cycles += 4;
// OR A,@r
// Reference: zaks:82 page 360
a.assign(alu_or8(a,l));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 182: {
cycles += 7;
// OR A,(HL)
// Reference: zaks:82 page 360
a.assign(alu_or8(a,read8(hl)));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 183: {
cycles += 4;
// OR A,@r
// Reference: zaks:82 page 360
a.assign(alu_or8(a,a));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 184: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,b));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 185: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,c));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 186: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,d));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 187: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,e));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 188: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,h));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 189: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,l));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 190: {
cycles += 7;
// CP A,(HL)
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,read8(hl)));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 191: {
cycles += 4;
// CP A,@r
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,a));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 192: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_6 == 0) {
pc.assign(read16(sp)-1);sp.assign(alu_add_u16u16(sp,2)); ;
cycles += 6;
}
pc.add(1);
 return cycles;
} break;
case 193: {
cycles += 10;
// POP @r
// Reference: zaks:82 page 373
bc.assign(read16(sp));
sp.assign(alu_add_u16u16(sp,2));
if (opcode == 0xf1) xferFlagsByteToCCBits();
pc.add(1);
 return cycles;
} break;
case 194: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_6 == 0) {
pc.assign(alu_add_u16u16(read16(pc.getUnsigned() + (1)),-3));
}
pc.add(3);
 return cycles;
} break;
case 195: {
cycles += 10;
// JP (@n)
// Reference: zaks:82 page 284
pc.assign(alu_add_u16u16(read16(pc.getUnsigned() + (1)), -3));
pc.add(3);
 return cycles;
} break;
case 196: {
cycles += 10;
// CALL @r,@n
// Reference: zaks:82 page 219
// Reference: This edition describes the P CC as 100, instead of 110
if (cc_bit_6 == 0) {
sp.assign(alu_sub_u16u16(sp,2));write16(sp,alu_add_u16u16(pc,3));
pc.assign(alu_sub_u16u16(read16(pc.getUnsigned() + (1)), 3));;
cycles += 7;
}
pc.add(3);
 return cycles;
} break;
case 197: {
cycles += 10;
// PUSH @r
// Reference: zaks:82 page 379
if (opcode == 0xf5) xferCCBitsToFlagsByte();
sp.assign(alu_sub_u16u16(sp,2));
write16(sp,bc);
pc.add(1);
 return cycles;
} break;
case 198: {
cycles += 7;
// ADD A,@n
// Reference: zaks:82 page 200
a.assign(alu_add_u8u8c(a,read8(pc.getUnsigned() + (1))));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 199: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
sp.assign(alu_add_u16u16(sp,-2));
write16(sp,alu_add_u16u16(pc,1));
pc.assign(alu_add_u16u16(0x00, -1));
pc.add(1);
 return cycles;
} break;
case 200: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_6 == 1) {
pc.assign(read16(sp)-1);sp.assign(alu_add_u16u16(sp,2)); ;
cycles += 6;
}
pc.add(1);
 return cycles;
} break;
case 201: {
cycles += 10;
// RET
// Reference: zaks:82 page 388
pc.assign(read16(sp)-1);
sp.assign(alu_add_u16u16(sp,2));
pc.add(1);
 return cycles;
} break;
case 202: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_6 == 1) {
pc.assign(alu_add_u16u16(read16(pc.getUnsigned() + (1)),-3));
}
pc.add(3);
 return cycles;
} break;
case 203: {
cycles += 0;
// CB
// Reference:  page 
// (extended instruction: This should return cb_ext();
pc.add(1);
return cb_ext();
pc.add(1);
 return cycles;
} break;
case 204: {
cycles += 10;
// CALL @r,@n
// Reference: zaks:82 page 219
// Reference: This edition describes the P CC as 100, instead of 110
if (cc_bit_6 == 1) {
sp.assign(alu_sub_u16u16(sp,2));write16(sp,alu_add_u16u16(pc,3));pc.assign(alu_sub_u16u16(read16(pc.getUnsigned() + (1)), 3));;
cycles += 7;
}
pc.add(3);
 return cycles;
} break;
case 205: {
cycles += 17;
// CALL (@n)
// Reference: zaks:82 page 222
sp.assign(alu_sub_u16u16(sp,2));
write16(sp,alu_add_u16u16(pc,3));
pc.assign(alu_sub_u16u16(read16(pc.getUnsigned() + (1)),3));
pc.add(3);
 return cycles;
} break;
case 206: {
cycles += 7;
// ADC A,@n
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,read8(pc.getUnsigned() + (1)),cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 207: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
sp.assign(alu_add_u16u16(sp,-2));
write16(sp,alu_add_u16u16(pc,1));
pc.assign(alu_add_u16u16(0x08, -1));
pc.add(1);
 return cycles;
} break;
case 208: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_0 == 0) {
pc.assign(read16(sp)-1);sp.assign(alu_add_u16u16(sp,2)); ;
cycles += 6;
}
pc.add(1);
 return cycles;
} break;
case 209: {
cycles += 10;
// POP @r
// Reference: zaks:82 page 373
de.assign(read16(sp));
sp.assign(alu_add_u16u16(sp,2));
if (opcode == 0xf1) xferFlagsByteToCCBits();
pc.add(1);
 return cycles;
} break;
case 210: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_0 == 0) {
pc.assign(alu_add_u16u16(read16(pc.getUnsigned() + (1)),-3));
}
pc.add(3);
 return cycles;
} break;
case 211: {
cycles += 11;
// OUT (@n),A
// Reference: zaks:82 page 368
emf::out8(read8(pc.getUnsigned() + (1)),a);
pc.add(2);
 return cycles;
} break;
case 212: {
cycles += 10;
// CALL @r,@n
// Reference: zaks:82 page 219
// Reference: This edition describes the P CC as 100, instead of 110
if (cc_bit_0 == 0) {
sp.assign(alu_sub_u16u16(sp,2));write16(sp,alu_add_u16u16(pc,3));pc.assign(alu_sub_u16u16(read16(pc.getUnsigned() + (1)), 3));;
cycles += 7;
}
pc.add(3);
 return cycles;
} break;
case 213: {
cycles += 10;
// PUSH @r
// Reference: zaks:82 page 379
if (opcode == 0xf5) xferCCBitsToFlagsByte();
sp.assign(alu_sub_u16u16(sp,2));
write16(sp,de);
pc.add(1);
 return cycles;
} break;
case 214: {
cycles += 7;
// SUB @n
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,read8(pc.getUnsigned() + (1))));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 215: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
sp.assign(alu_add_u16u16(sp,-2));
write16(sp,alu_add_u16u16(pc,1));
pc.assign(alu_add_u16u16(0x10, -1));
pc.add(1);
 return cycles;
} break;
case 216: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_0 == 1) {
pc.assign(read16(sp)-1);sp.assign(alu_add_u16u16(sp,2)); ;
cycles += 6;
}
pc.add(1);
 return cycles;
} break;
case 217: {
cycles += 4;
// EXX
// Reference: zaks:82 page 256
tmp16.assign(bc);
bc.assign(prime_bc);
prime_bc.assign(tmp16);
tmp16.assign(de);
de.assign(prime_de);
prime_de.assign(tmp16);
tmp16.assign(hl);
hl.assign(prime_hl);
prime_hl.assign(tmp16);
pc.add(1);
 return cycles;
} break;
case 218: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_0 == 1) {
pc.assign(alu_add_u16u16(read16(pc.getUnsigned() + (1)),-3));
}
pc.add(3);
 return cycles;
} break;
case 219: {
cycles += 11;
// IN A,(@n)
// Reference: zaks:82 page 263
a.assign(emf::in8((a.getUnsigned()*256) | (read8(pc.getUnsigned() + (1)))));
computeFlags8(a.getUnsigned());
pc.add(2);
 return cycles;
} break;
case 220: {
cycles += 10;
// CALL @r,@n
// Reference: zaks:82 page 219
// Reference: This edition describes the P CC as 100, instead of 110
if (cc_bit_0 == 1) {
sp.assign(alu_sub_u16u16(sp,2));write16(sp,alu_add_u16u16(pc,3));pc.assign(alu_sub_u16u16(read16(pc.getUnsigned() + (1)), 3));;
cycles += 7;
}
pc.add(3);
 return cycles;
} break;
case 221: {
cycles += 0;
// DD
// Reference:  page 
// (extended instruction: This should return dd_ext();
pc.add(1);
return dd_ext();
pc.add(1);
 return cycles;
} break;
case 222: {
cycles += 11;
// SBC A,@n
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,read8(pc.getUnsigned() + (1)),cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 223: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
sp.assign(alu_add_u16u16(sp,-2));
write16(sp,alu_add_u16u16(pc,1));
pc.assign(alu_add_u16u16(0x18, -1));
pc.add(1);
 return cycles;
} break;
case 224: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_2 == 0) {
pc.assign(read16(sp)-1);sp.assign(alu_add_u16u16(sp,2)); ;
cycles += 6;
}
pc.add(1);
 return cycles;
} break;
case 225: {
cycles += 10;
// POP @r
// Reference: zaks:82 page 373
hl.assign(read16(sp));
sp.assign(alu_add_u16u16(sp,2));
if (opcode == 0xf1) xferFlagsByteToCCBits();
pc.add(1);
 return cycles;
} break;
case 226: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_2 == 0) {
pc.assign(alu_add_u16u16(read16(pc.getUnsigned() + (1)),-3));
}
pc.add(3);
 return cycles;
} break;
case 227: {
cycles += 19;
// EX (SP),HL
// Reference: zaks:82 page 250
tmp16.assign(hl);
hl.assign(read16(sp));
write16(sp,tmp16);
pc.add(1);
 return cycles;
} break;
case 228: {
cycles += 10;
// CALL @r,@n
// Reference: zaks:82 page 219
// Reference: This edition describes the P CC as 100, instead of 110
if (cc_bit_2 == 0) {
sp.assign(alu_sub_u16u16(sp,2));write16(sp,alu_add_u16u16(pc,3));pc.assign(alu_sub_u16u16(read16(pc.getUnsigned() + (1)), 3));;
cycles += 7;
}
pc.add(3);
 return cycles;
} break;
case 229: {
cycles += 10;
// PUSH @r
// Reference: zaks:82 page 379
if (opcode == 0xf5) xferCCBitsToFlagsByte();
sp.assign(alu_sub_u16u16(sp,2));
write16(sp,hl);
pc.add(1);
 return cycles;
} break;
case 230: {
cycles += 7;
// AND @n
// Reference: zaks:82 page 209
a.assign(alu_and8(a,read8(pc.getUnsigned() + (1))));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(2);
 return cycles;
} break;
case 231: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
sp.assign(alu_add_u16u16(sp,-2));
write16(sp,alu_add_u16u16(pc,1));
pc.assign(alu_add_u16u16(0x20, -1));
pc.add(1);
 return cycles;
} break;
case 232: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_2 == 1) {
pc.assign(read16(sp)-1);sp.assign(alu_add_u16u16(sp,2)); ;
cycles += 6;
}
pc.add(1);
 return cycles;
} break;
case 233: {
cycles += 4;
// JP (HL)
// Reference: zaks:82 page 285
pc.assign(alu_add_u16u16(hl,-1));
pc.add(1);
 return cycles;
} break;
case 234: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_2 == 1) {
pc.assign(alu_add_u16u16(read16(pc.getUnsigned() + (1)),-3));
}
pc.add(3);
 return cycles;
} break;
case 235: {
cycles += 4;
// EX DE,HL
// Reference: zaks:82 page 249
tmp16.assign(hl);
hl.assign(de);
de.assign(tmp16);
pc.add(1);
 return cycles;
} break;
case 236: {
cycles += 10;
// CALL @r,@n
// Reference: zaks:82 page 219
// Reference: This edition describes the P CC as 100, instead of 110
if (cc_bit_2 == 1) {
sp.assign(alu_sub_u16u16(sp,2));write16(sp,alu_add_u16u16(pc,3));pc.assign(alu_sub_u16u16(read16(pc.getUnsigned() + (1)), 3));;
cycles += 7;
}
pc.add(3);
 return cycles;
} break;
case 237: {
cycles += 0;
// ED
// Reference:  page 
// (extended instruction: This should return ed_ext();
pc.add(1);
return ed_ext();
pc.add(1);
 return cycles;
} break;
case 238: {
cycles += 7;
// XOR A,@n
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,read8(pc.getUnsigned() + (1))));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(2);
 return cycles;
} break;
case 239: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
sp.assign(alu_add_u16u16(sp,-2));
write16(sp,alu_add_u16u16(pc,1));
pc.assign(alu_add_u16u16(0x28, -1));
pc.add(1);
 return cycles;
} break;
case 240: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_7 == 0) {
pc.assign(read16(sp)-1);sp.assign(alu_add_u16u16(sp,2)); ;
cycles += 6;
}
pc.add(1);
 return cycles;
} break;
case 241: {
cycles += 10;
// POP @r
// Reference: zaks:82 page 373
af.assign(read16(sp));
sp.assign(alu_add_u16u16(sp,2));
if (opcode == 0xf1) xferFlagsByteToCCBits();
pc.add(1);
 return cycles;
} break;
case 242: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_7 == 0) {
pc.assign(alu_add_u16u16(read16(pc.getUnsigned() + (1)),-3));
}
pc.add(3);
 return cycles;
} break;
case 243: {
cycles += 4;
// DI
// Reference: zaks:82 page 244
disableInterrupt();
pc.add(1);
 return cycles;
} break;
case 244: {
cycles += 10;
// CALL @r,@n
// Reference: zaks:82 page 219
// Reference: This edition describes the P CC as 100, instead of 110
if (cc_bit_7 == 0) {
sp.assign(alu_sub_u16u16(sp,2));write16(sp,alu_add_u16u16(pc,3));pc.assign(alu_sub_u16u16(read16(pc.getUnsigned() + (1)), 3));;
cycles += 7;
}
pc.add(3);
 return cycles;
} break;
case 245: {
cycles += 10;
// PUSH @r
// Reference: zaks:82 page 379
if (opcode == 0xf5) xferCCBitsToFlagsByte();
sp.assign(alu_sub_u16u16(sp,2));
write16(sp,af);
pc.add(1);
 return cycles;
} break;
case 246: {
cycles += 7;
// OR A,@n
// Reference: zaks:82 page 360
a.assign(alu_or8(a,read8(pc.getUnsigned() + (1))));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(2);
 return cycles;
} break;
case 247: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
sp.assign(alu_add_u16u16(sp,-2));
write16(sp,alu_add_u16u16(pc,1));
pc.assign(alu_add_u16u16(0x30, -1));
pc.add(1);
 return cycles;
} break;
case 248: {
cycles += 5;
// RET @r
// Reference: zaks:82 page 390
if (cc_bit_7 == 1) {
pc.assign(read16(sp)-1);sp.assign(alu_add_u16u16(sp,2)); ;
cycles += 6;
}
pc.add(1);
 return cycles;
} break;
case 249: {
cycles += 6;
// LD SP,HL
// Reference: zaks:82 page 345
sp.assign(hl);
pc.add(1);
 return cycles;
} break;
case 250: {
cycles += 10;
// JP @r,(@n)
// Reference: zaks:82 page 282
if (cc_bit_7 == 1) {
pc.assign(alu_add_u16u16(read16(pc.getUnsigned() + (1)),-3));
}
pc.add(3);
 return cycles;
} break;
case 251: {
cycles += 4;
// EI
// Reference: zaks:82 page 247
enableInterrupt();
pc.add(1);
 return cycles;
} break;
case 252: {
cycles += 10;
// CALL @r,@n
// Reference: zaks:82 page 219
// Reference: This edition describes the P CC as 100, instead of 110
if (cc_bit_7 == 1) {
sp.assign(alu_sub_u16u16(sp,2));write16(sp,alu_add_u16u16(pc,3));pc.assign(alu_sub_u16u16(read16(pc.getUnsigned() + (1)), 3));;
cycles += 7;
}
pc.add(3);
 return cycles;
} break;
case 253: {
cycles += 0;
// FD
// Reference:  page 
// (extended instruction: This should return fd_ext();
pc.add(1);
return fd_ext();
pc.add(1);
 return cycles;
} break;
case 254: {
cycles += 7;
// CP @n
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,read8(pc.getUnsigned() + (1))));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 255: {
cycles += 11;
// RST @r
// Reference: zaks:82 page 418
sp.assign(alu_add_u16u16(sp,-2));
write16(sp,alu_add_u16u16(pc,1));
pc.assign(alu_add_u16u16(0x38, -1));
pc.add(1);
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
uint8_t bit;
uint8_t opcode = fetch8(pc);
uint8_t cycles = 1;
// INLINE EXPERIMENT - START
uint16_t ipc = pc.getUnsigned();
switch (ipc) {
}
// INLINE EXPERIMENT - END
switch (opcode) {
case 0: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 1: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 2: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 3: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 4: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 5: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 6: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 7: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 8: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 9: {
cycles += 15;
// ADD IX,@r
// Reference: zaks:82 page 205
ix.assign(alu_add_u16u16c(ix,bc));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 10: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 11: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 12: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 13: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 14: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 15: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 16: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 17: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 18: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 19: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 20: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 21: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 22: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 23: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 24: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 25: {
cycles += 15;
// ADD IX,@r
// Reference: zaks:82 page 205
ix.assign(alu_add_u16u16c(ix,de));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 26: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 27: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 28: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 29: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 30: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 31: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 32: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 33: {
cycles += 15;
// LD IX,@n
// Reference: zaks:82 page 336
ix.assign(read16(pc.getUnsigned() + (1)));
pc.add(3);
 return cycles;
} break;
case 34: {
cycles += 20;
// LD (@n),IX
// Reference: zaks:82 page 325
write16(read16(pc.getUnsigned() + (1)),ix);
pc.add(3);
 return cycles;
} break;
case 35: {
cycles += 10;
// INC IX
// Reference: zaks:82 page 272
ix.assign(alu_add_u16u16(ix,1));
pc.add(1);
 return cycles;
} break;
case 36: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 37: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 38: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 39: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 40: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 41: {
cycles += 15;
// ADD IX,@r
// Reference: zaks:82 page 205
ix.assign(alu_add_u16u16c(ix,ix));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 42: {
cycles += 20;
// LD IX,(@n)
// Reference: zaks:82 page 338
ix.assign(read16(read16(pc.getUnsigned() + (1))));
pc.add(3);
 return cycles;
} break;
case 43: {
cycles += 10;
// DEC IX
// Reference: zaks:82 page 242
ix.assign(alu_sub_u16u16(ix,1));
pc.add(1);
 return cycles;
} break;
case 44: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 45: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 46: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 47: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 48: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 49: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 50: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 51: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 52: {
cycles += 23;
// INC (IX+@n)
// Reference: zaks:82 page 268
tmp8.assign(alu_add_u8u8c(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))),1));
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(2);
 return cycles;
} break;
case 53: {
cycles += 23;
// DEC (IX+@n)
// Reference: zaks:82 page 238
tmp8.assign(alu_sub_u8u8b(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))),1));
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),tmp8);
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(2);
 return cycles;
} break;
case 54: {
cycles += 19;
// LD (IX+@d),@n
// Reference: zaks:82 page 309
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),read8(pc.getUnsigned() + (2)));
pc.add(3);
 return cycles;
} break;
case 55: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 56: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 57: {
cycles += 15;
// ADD IX,@r
// Reference: zaks:82 page 205
ix.assign(alu_add_u16u16c(ix,sp));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 58: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 59: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 60: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 61: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 62: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 63: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 64: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 65: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 66: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 67: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 68: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 69: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 70: {
cycles += 19;
// LD @r,(IX+@n)
// Reference: zaks:82 page 305
b.assign(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 71: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 72: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 73: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 74: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 75: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 76: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 77: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 78: {
cycles += 19;
// LD @r,(IX+@n)
// Reference: zaks:82 page 305
c.assign(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 79: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 80: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 81: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 82: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 83: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 84: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 85: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 86: {
cycles += 19;
// LD @r,(IX+@n)
// Reference: zaks:82 page 305
d.assign(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 87: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 88: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 89: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 90: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 91: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 92: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 93: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 94: {
cycles += 19;
// LD @r,(IX+@n)
// Reference: zaks:82 page 305
e.assign(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 95: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 96: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 97: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 98: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 99: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 100: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 101: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 102: {
cycles += 19;
// LD @r,(IX+@n)
// Reference: zaks:82 page 305
h.assign(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 103: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 104: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 105: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 106: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 107: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 108: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 109: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 110: {
cycles += 19;
// LD @r,(IX+@n)
// Reference: zaks:82 page 305
l.assign(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 111: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 112: {
cycles += 19;
// LD (IX+@n),@r
// Reference: zaks:82 page 313
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))), b);
pc.add(2);
 return cycles;
} break;
case 113: {
cycles += 19;
// LD (IX+@n),@r
// Reference: zaks:82 page 313
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))), c);
pc.add(2);
 return cycles;
} break;
case 114: {
cycles += 19;
// LD (IX+@n),@r
// Reference: zaks:82 page 313
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))), d);
pc.add(2);
 return cycles;
} break;
case 115: {
cycles += 19;
// LD (IX+@n),@r
// Reference: zaks:82 page 313
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))), e);
pc.add(2);
 return cycles;
} break;
case 116: {
cycles += 19;
// LD (IX+@n),@r
// Reference: zaks:82 page 313
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))), h);
pc.add(2);
 return cycles;
} break;
case 117: {
cycles += 19;
// LD (IX+@n),@r
// Reference: zaks:82 page 313
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))), l);
pc.add(2);
 return cycles;
} break;
case 118: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 119: {
cycles += 19;
// LD (IX+@n),@r
// Reference: zaks:82 page 313
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))), a);
pc.add(2);
 return cycles;
} break;
case 120: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 121: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 122: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 123: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 124: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 125: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 126: {
cycles += 19;
// LD @r,(IX+@n)
// Reference: zaks:82 page 305
a.assign(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 127: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 128: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 129: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 130: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 131: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 132: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 133: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 134: {
cycles += 19;
// ADD A,(IX+@n)
// Reference: zaks:82 page 196
a.assign(alu_add_u8u8c(a,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 135: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 136: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 137: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 138: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 139: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 140: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 141: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 142: {
cycles += 15;
// ADC A,(IX+@n)
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))),cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(2);
 return cycles;
} break;
case 143: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 144: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 145: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 146: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 147: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 148: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 149: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 150: {
cycles += 15;
// SUB (IX+@n)
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 151: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 152: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 153: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 154: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 155: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 156: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 157: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 158: {
cycles += 19;
// SBC A,(IX+@n)
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))),cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 159: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 160: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 161: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 162: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 163: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 164: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 165: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 166: {
cycles += 19;
// AND (IX+@n)
// Reference: zaks:82 page 209
a.assign(alu_and8(a,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(2);
 return cycles;
} break;
case 167: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 168: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 169: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 170: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 171: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 172: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 173: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 174: {
cycles += 15;
// XOR A,(IX+@n)
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(2);
 return cycles;
} break;
case 175: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 176: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 177: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 178: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 179: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 180: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 181: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 182: {
cycles += 19;
// OR (IX+@n)
// Reference: zaks:82 page 360
a.assign(alu_or8(a,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(2);
 return cycles;
} break;
case 183: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 184: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 185: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 186: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 187: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 188: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 189: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 190: {
cycles += 19;
// CP A,(IX+@n)
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 191: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 192: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 193: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 194: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 195: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 196: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 197: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 198: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 199: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 200: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 201: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 202: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 203: {
cycles += 23;
// RLC (IX+@n)
// Reference: zaks:82 page 404
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x6) {
tmp8.assign(alu_rlc8(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// RRC (IX+@n)
// Reference: zaks:82 page 413
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0xe) {
tmp8.assign(alu_rrc8(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// RL (IX+@n)
// Reference: zaks:82 page 396
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x16) {
tmp8.assign(alu_rl8(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))), cc_bit_0));
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 23;
// RR (IX+@n)
// Reference: zaks:82 page 410
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x1e) {
tmp8.assign(alu_rr8(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1)))), cc_bit_0));
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// SLA (IX+@n)
// Reference: zaks:82 page 428
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x26) {
tmp8.assign(alu_sla8(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// SRA (IX+@n)
// Reference: zaks:82 page 430
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x2e) {
tmp8.assign(alu_sra8(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// SLL (IX+@n)
// Reference:  page 
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x36) {
tmp8.assign(alu_sll8(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 23;
// SRL (IX+@n)
// Reference: zaks:82 page 432
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x3e) {
tmp8.assign(alu_srl8(read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 20;
// BIT @r,(IX+@n)
// Reference: zaks:82 page 213
if ((read8(pc.getUnsigned() + (2))& 0xc7) == 0x46) {
uint8_t bit = (read8(alu_add_u16s8(pc,2)) & 0x38)>>3;;
tmp8.assign(alu_testBit8(bit,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(3);
 return cycles;
}
cycles += 15;
// RES @r,(IX+@n)
// Reference: zaks:82 page 385
if ((read8(pc.getUnsigned() + (2))& 0xc7) == 0x86) {
uint8_t bit = (read8(alu_add_u16u16(pc,2)) & 0x38)>>3;;
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))), alu_clearBit8(bit,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
pc.add(3);
 return cycles;
}
cycles += 23;
// SET @r,(IX+@n)
// Reference: zaks:82 page 425
if ((read8(pc.getUnsigned() + (2))& 0xc7) == 0xc6) {
uint8_t bit = (read8(alu_add_u16u16(pc,2)) & 0x38)>>3;;
write8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))), alu_setBit8(bit,read8(alu_add_u16s8(ix,read8(pc.getUnsigned() + (1))))));
pc.add(3);
 return cycles;
}
} break;
case 204: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 205: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 206: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 207: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 208: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 209: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 210: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 211: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 212: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 213: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 214: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 215: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 216: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 217: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 218: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 219: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 220: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 221: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 222: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 223: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 224: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 225: {
cycles += 14;
// POP IX
// Reference: zaks:82 page 375
ix.assign(read16(sp));
sp.assign(alu_add_u16u16(sp,2));
pc.add(1);
 return cycles;
} break;
case 226: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 227: {
cycles += 23;
// EX (SP),IX
// Reference: zaks:82 page 252
tmp16.assign(ix);
ix.assign(read16(sp));
write16(sp,tmp16);
pc.add(1);
 return cycles;
} break;
case 228: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 229: {
cycles += 15;
// PUSH IX
// Reference: zaks:82 page 381
sp.assign(alu_sub_u16u16(sp,2));
write16(sp,ix);
pc.add(1);
 return cycles;
} break;
case 230: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 231: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 232: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 233: {
cycles += 8;
// JP (IX)
// Reference: zaks:82 page 286
pc.assign(alu_add_u16u16(ix, -1));
pc.add(1);
 return cycles;
} break;
case 234: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 235: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 236: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 237: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 238: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 239: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 240: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 241: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 242: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 243: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 244: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 245: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 246: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 247: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 248: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 249: {
cycles += 10;
// LD SP,IX
// Reference: zaks:82 page 346
sp.assign(ix);
pc.add(1);
 return cycles;
} break;
case 250: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 251: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 252: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 253: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 254: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 255: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
}
// Even if no instruction was found (maybe, because there's holes in the instructions map), we still want to indicate that time has passed
return 1;
}
// Writing from instruction block:
// /emf/res/devices/z80/z80_cb.xml
// Emulation generateProcessMethod
uint8_t cb_ext() {
uint8_t bit;
uint8_t opcode = fetch8(pc);
uint8_t cycles = 1;
// INLINE EXPERIMENT - START
uint16_t ipc = pc.getUnsigned();
switch (ipc) {
case 16509: {
// 0
cycles += 8;
// RLC @r
// Reference: zaks:82 page 400
b.assign(alu_rlc8(b));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
}
// INLINE EXPERIMENT - END
switch (opcode) {
case 0: {
cycles += 8;
// RLC @r
// Reference: zaks:82 page 400
b.assign(alu_rlc8(b));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 1: {
cycles += 8;
// RLC @r
// Reference: zaks:82 page 400
c.assign(alu_rlc8(c));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 2: {
cycles += 8;
// RLC @r
// Reference: zaks:82 page 400
d.assign(alu_rlc8(d));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 3: {
cycles += 8;
// RLC @r
// Reference: zaks:82 page 400
e.assign(alu_rlc8(e));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 4: {
cycles += 8;
// RLC @r
// Reference: zaks:82 page 400
h.assign(alu_rlc8(h));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 5: {
cycles += 8;
// RLC @r
// Reference: zaks:82 page 400
l.assign(alu_rlc8(l));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 6: {
cycles += 15;
// RLC (HL)
// Reference: zaks:82 page 402
tmp8.assign(alu_rlc8(read8(hl)));
write8(hl,tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 7: {
cycles += 8;
// RLC @r
// Reference: zaks:82 page 400
a.assign(alu_rlc8(a));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 8: {
cycles += 8;
// RRC @r
// Reference: zaks:82 page 413
b.assign(alu_rrc8(b));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 9: {
cycles += 8;
// RRC @r
// Reference: zaks:82 page 413
c.assign(alu_rrc8(c));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 10: {
cycles += 8;
// RRC @r
// Reference: zaks:82 page 413
d.assign(alu_rrc8(d));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 11: {
cycles += 8;
// RRC @r
// Reference: zaks:82 page 413
e.assign(alu_rrc8(e));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 12: {
cycles += 8;
// RRC @r
// Reference: zaks:82 page 413
h.assign(alu_rrc8(h));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 13: {
cycles += 8;
// RRC @r
// Reference: zaks:82 page 413
l.assign(alu_rrc8(l));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 14: {
cycles += 15;
// RRC (HL)
// Reference: zaks:82 page 413
tmp8.assign(alu_rrc8(read8(hl)));
write8(hl,tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 15: {
cycles += 8;
// RRC @r
// Reference: zaks:82 page 413
a.assign(alu_rrc8(a));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 16: {
cycles += 8;
// RL @r
// Reference: zaks:82 page 396
b.assign(alu_rl8(b, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 17: {
cycles += 8;
// RL @r
// Reference: zaks:82 page 396
c.assign(alu_rl8(c, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 18: {
cycles += 8;
// RL @r
// Reference: zaks:82 page 396
d.assign(alu_rl8(d, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 19: {
cycles += 8;
// RL @r
// Reference: zaks:82 page 396
e.assign(alu_rl8(e, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 20: {
cycles += 8;
// RL @r
// Reference: zaks:82 page 396
h.assign(alu_rl8(h, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 21: {
cycles += 8;
// RL @r
// Reference: zaks:82 page 396
l.assign(alu_rl8(l, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 22: {
cycles += 15;
// RL (HL)
// Reference: zaks:82 page 396
tmp8.assign(alu_rl8(read8(hl), cc_bit_0));
write8(hl,tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 23: {
cycles += 8;
// RL @r
// Reference: zaks:82 page 396
a.assign(alu_rl8(a, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 24: {
cycles += 8;
// RR @r
// Reference: zaks:82 page 410
b.assign(alu_rr8(b, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 25: {
cycles += 8;
// RR @r
// Reference: zaks:82 page 410
c.assign(alu_rr8(c, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 26: {
cycles += 8;
// RR @r
// Reference: zaks:82 page 410
d.assign(alu_rr8(d, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 27: {
cycles += 8;
// RR @r
// Reference: zaks:82 page 410
e.assign(alu_rr8(e, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 28: {
cycles += 8;
// RR @r
// Reference: zaks:82 page 410
h.assign(alu_rr8(h, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 29: {
cycles += 8;
// RR @r
// Reference: zaks:82 page 410
l.assign(alu_rr8(l, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 30: {
cycles += 15;
// RR (HL)
// Reference: zaks:82 page 410
tmp8.assign(alu_rr8(read8(hl), cc_bit_0));
write8(hl,tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 31: {
cycles += 8;
// RR @r
// Reference: zaks:82 page 410
a.assign(alu_rr8(a, cc_bit_0));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 32: {
cycles += 8;
// SLA @r
// Reference: zaks:82 page 428
b.assign(alu_sla8(b));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 33: {
cycles += 8;
// SLA @r
// Reference: zaks:82 page 428
c.assign(alu_sla8(c));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 34: {
cycles += 8;
// SLA @r
// Reference: zaks:82 page 428
d.assign(alu_sla8(d));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 35: {
cycles += 8;
// SLA @r
// Reference: zaks:82 page 428
e.assign(alu_sla8(e));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 36: {
cycles += 8;
// SLA @r
// Reference: zaks:82 page 428
h.assign(alu_sla8(h));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 37: {
cycles += 8;
// SLA @r
// Reference: zaks:82 page 428
l.assign(alu_sla8(l));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 38: {
cycles += 15;
// SLA (HL)
// Reference: zaks:82 page 428
tmp8.assign(alu_sla8(read8(hl)));
write8(hl,tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 39: {
cycles += 8;
// SLA @r
// Reference: zaks:82 page 428
a.assign(alu_sla8(a));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 40: {
cycles += 8;
// SRA @r
// Reference: zaks:82 page 430
b.assign(alu_sra8(b));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 41: {
cycles += 8;
// SRA @r
// Reference: zaks:82 page 430
c.assign(alu_sra8(c));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 42: {
cycles += 8;
// SRA @r
// Reference: zaks:82 page 430
d.assign(alu_sra8(d));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 43: {
cycles += 8;
// SRA @r
// Reference: zaks:82 page 430
e.assign(alu_sra8(e));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 44: {
cycles += 8;
// SRA @r
// Reference: zaks:82 page 430
h.assign(alu_sra8(h));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 45: {
cycles += 8;
// SRA @r
// Reference: zaks:82 page 430
l.assign(alu_sra8(l));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 46: {
cycles += 15;
// SRA (HL)
// Reference: zaks:82 page 430
tmp8.assign(alu_sra8(read8(hl)));
write8(hl,tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 47: {
cycles += 8;
// SRA @r
// Reference: zaks:82 page 430
a.assign(alu_sra8(a));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 48: {
cycles += 8;
// SLL @r
// Reference:  page 
b.assign(alu_sll8(b));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 49: {
cycles += 8;
// SLL @r
// Reference:  page 
c.assign(alu_sll8(c));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 50: {
cycles += 8;
// SLL @r
// Reference:  page 
d.assign(alu_sll8(d));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 51: {
cycles += 8;
// SLL @r
// Reference:  page 
e.assign(alu_sll8(e));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 52: {
cycles += 8;
// SLL @r
// Reference:  page 
h.assign(alu_sll8(h));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 53: {
cycles += 8;
// SLL @r
// Reference:  page 
l.assign(alu_sll8(l));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 54: {
cycles += 15;
// SLL (HL)
// Reference:  page 
tmp8.assign(alu_sll8(read8(hl)));
write8(hl,tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 55: {
cycles += 8;
// SLL @r
// Reference:  page 
a.assign(alu_sll8(a));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 56: {
cycles += 8;
// SRL @r
// Reference: zaks:82 page 432
b.assign(alu_srl8(b));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 57: {
cycles += 8;
// SRL @r
// Reference: zaks:82 page 432
c.assign(alu_srl8(c));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 58: {
cycles += 8;
// SRL @r
// Reference: zaks:82 page 432
d.assign(alu_srl8(d));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 59: {
cycles += 8;
// SRL @r
// Reference: zaks:82 page 432
e.assign(alu_srl8(e));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 60: {
cycles += 8;
// SRL @r
// Reference: zaks:82 page 432
h.assign(alu_srl8(h));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 61: {
cycles += 8;
// SRL @r
// Reference: zaks:82 page 432
l.assign(alu_srl8(l));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 62: {
cycles += 15;
// SRL (HL)
// Reference: zaks:82 page 432
tmp8.assign(alu_srl8(read8(hl)));
write8(hl,tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 63: {
cycles += 8;
// SRL @r
// Reference: zaks:82 page 432
a.assign(alu_srl8(a));
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 64: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(0,b));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 65: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(0,c));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 66: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(0,d));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 67: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(0,e));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 68: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(0,h));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 69: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(0,l));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 70: {
cycles += 12;
// BIT @r, (HL)
// Reference: zaks:82 page 211
tmp8.assign(alu_testBit8(0,read8(hl)));
clearFlagN();
setFlagH();
affectFlagZ();
affectFlagS();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 71: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(0,a));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 72: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(1,b));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 73: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(1,c));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 74: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(1,d));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 75: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(1,e));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 76: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(1,h));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 77: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(1,l));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 78: {
cycles += 12;
// BIT @r, (HL)
// Reference: zaks:82 page 211
tmp8.assign(alu_testBit8(1,read8(hl)));
clearFlagN();
setFlagH();
affectFlagZ();
affectFlagS();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 79: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(1,a));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 80: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(2,b));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 81: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(2,c));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 82: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(2,d));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 83: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(2,e));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 84: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(2,h));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 85: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(2,l));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 86: {
cycles += 12;
// BIT @r, (HL)
// Reference: zaks:82 page 211
tmp8.assign(alu_testBit8(2,read8(hl)));
clearFlagN();
setFlagH();
affectFlagZ();
affectFlagS();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 87: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(2,a));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 88: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(3,b));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 89: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(3,c));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 90: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(3,d));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 91: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(3,e));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 92: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(3,h));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 93: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(3,l));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 94: {
cycles += 12;
// BIT @r, (HL)
// Reference: zaks:82 page 211
tmp8.assign(alu_testBit8(3,read8(hl)));
clearFlagN();
setFlagH();
affectFlagZ();
affectFlagS();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 95: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(3,a));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 96: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(4,b));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 97: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(4,c));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 98: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(4,d));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 99: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(4,e));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 100: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(4,h));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 101: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(4,l));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 102: {
cycles += 12;
// BIT @r, (HL)
// Reference: zaks:82 page 211
tmp8.assign(alu_testBit8(4,read8(hl)));
clearFlagN();
setFlagH();
affectFlagZ();
affectFlagS();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 103: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(4,a));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 104: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(5,b));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 105: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(5,c));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 106: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(5,d));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 107: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(5,e));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 108: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(5,h));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 109: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(5,l));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 110: {
cycles += 12;
// BIT @r, (HL)
// Reference: zaks:82 page 211
tmp8.assign(alu_testBit8(5,read8(hl)));
clearFlagN();
setFlagH();
affectFlagZ();
affectFlagS();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 111: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(5,a));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 112: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(6,b));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 113: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(6,c));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 114: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(6,d));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 115: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(6,e));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 116: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(6,h));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 117: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(6,l));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 118: {
cycles += 12;
// BIT @r, (HL)
// Reference: zaks:82 page 211
tmp8.assign(alu_testBit8(6,read8(hl)));
clearFlagN();
setFlagH();
affectFlagZ();
affectFlagS();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 119: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(6,a));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 120: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(7,b));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 121: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(7,c));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 122: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(7,d));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 123: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(7,e));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 124: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(7,h));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 125: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(7,l));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 126: {
cycles += 12;
// BIT @r, (HL)
// Reference: zaks:82 page 211
tmp8.assign(alu_testBit8(7,read8(hl)));
clearFlagN();
setFlagH();
affectFlagZ();
affectFlagS();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 127: {
cycles += 8;
// BIT @r,@s
// Reference: zaks:82 page 217
tmp8.assign(alu_testBit8(7,a));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 128: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
b.assign(alu_clearBit8(0,b));
pc.add(1);
 return cycles;
} break;
case 129: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
c.assign(alu_clearBit8(0,c));
pc.add(1);
 return cycles;
} break;
case 130: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
d.assign(alu_clearBit8(0,d));
pc.add(1);
 return cycles;
} break;
case 131: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
e.assign(alu_clearBit8(0,e));
pc.add(1);
 return cycles;
} break;
case 132: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
h.assign(alu_clearBit8(0,h));
pc.add(1);
 return cycles;
} break;
case 133: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
l.assign(alu_clearBit8(0,l));
pc.add(1);
 return cycles;
} break;
case 134: {
cycles += 15;
// RES @r, (HL)
// Reference: zaks:82 page 385
write8(hl, alu_clearBit8(0,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 135: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
a.assign(alu_clearBit8(0,a));
pc.add(1);
 return cycles;
} break;
case 136: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
b.assign(alu_clearBit8(1,b));
pc.add(1);
 return cycles;
} break;
case 137: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
c.assign(alu_clearBit8(1,c));
pc.add(1);
 return cycles;
} break;
case 138: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
d.assign(alu_clearBit8(1,d));
pc.add(1);
 return cycles;
} break;
case 139: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
e.assign(alu_clearBit8(1,e));
pc.add(1);
 return cycles;
} break;
case 140: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
h.assign(alu_clearBit8(1,h));
pc.add(1);
 return cycles;
} break;
case 141: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
l.assign(alu_clearBit8(1,l));
pc.add(1);
 return cycles;
} break;
case 142: {
cycles += 15;
// RES @r, (HL)
// Reference: zaks:82 page 385
write8(hl, alu_clearBit8(1,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 143: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
a.assign(alu_clearBit8(1,a));
pc.add(1);
 return cycles;
} break;
case 144: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
b.assign(alu_clearBit8(2,b));
pc.add(1);
 return cycles;
} break;
case 145: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
c.assign(alu_clearBit8(2,c));
pc.add(1);
 return cycles;
} break;
case 146: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
d.assign(alu_clearBit8(2,d));
pc.add(1);
 return cycles;
} break;
case 147: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
e.assign(alu_clearBit8(2,e));
pc.add(1);
 return cycles;
} break;
case 148: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
h.assign(alu_clearBit8(2,h));
pc.add(1);
 return cycles;
} break;
case 149: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
l.assign(alu_clearBit8(2,l));
pc.add(1);
 return cycles;
} break;
case 150: {
cycles += 15;
// RES @r, (HL)
// Reference: zaks:82 page 385
write8(hl, alu_clearBit8(2,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 151: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
a.assign(alu_clearBit8(2,a));
pc.add(1);
 return cycles;
} break;
case 152: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
b.assign(alu_clearBit8(3,b));
pc.add(1);
 return cycles;
} break;
case 153: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
c.assign(alu_clearBit8(3,c));
pc.add(1);
 return cycles;
} break;
case 154: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
d.assign(alu_clearBit8(3,d));
pc.add(1);
 return cycles;
} break;
case 155: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
e.assign(alu_clearBit8(3,e));
pc.add(1);
 return cycles;
} break;
case 156: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
h.assign(alu_clearBit8(3,h));
pc.add(1);
 return cycles;
} break;
case 157: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
l.assign(alu_clearBit8(3,l));
pc.add(1);
 return cycles;
} break;
case 158: {
cycles += 15;
// RES @r, (HL)
// Reference: zaks:82 page 385
write8(hl, alu_clearBit8(3,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 159: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
a.assign(alu_clearBit8(3,a));
pc.add(1);
 return cycles;
} break;
case 160: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
b.assign(alu_clearBit8(4,b));
pc.add(1);
 return cycles;
} break;
case 161: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
c.assign(alu_clearBit8(4,c));
pc.add(1);
 return cycles;
} break;
case 162: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
d.assign(alu_clearBit8(4,d));
pc.add(1);
 return cycles;
} break;
case 163: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
e.assign(alu_clearBit8(4,e));
pc.add(1);
 return cycles;
} break;
case 164: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
h.assign(alu_clearBit8(4,h));
pc.add(1);
 return cycles;
} break;
case 165: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
l.assign(alu_clearBit8(4,l));
pc.add(1);
 return cycles;
} break;
case 166: {
cycles += 15;
// RES @r, (HL)
// Reference: zaks:82 page 385
write8(hl, alu_clearBit8(4,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 167: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
a.assign(alu_clearBit8(4,a));
pc.add(1);
 return cycles;
} break;
case 168: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
b.assign(alu_clearBit8(5,b));
pc.add(1);
 return cycles;
} break;
case 169: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
c.assign(alu_clearBit8(5,c));
pc.add(1);
 return cycles;
} break;
case 170: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
d.assign(alu_clearBit8(5,d));
pc.add(1);
 return cycles;
} break;
case 171: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
e.assign(alu_clearBit8(5,e));
pc.add(1);
 return cycles;
} break;
case 172: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
h.assign(alu_clearBit8(5,h));
pc.add(1);
 return cycles;
} break;
case 173: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
l.assign(alu_clearBit8(5,l));
pc.add(1);
 return cycles;
} break;
case 174: {
cycles += 15;
// RES @r, (HL)
// Reference: zaks:82 page 385
write8(hl, alu_clearBit8(5,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 175: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
a.assign(alu_clearBit8(5,a));
pc.add(1);
 return cycles;
} break;
case 176: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
b.assign(alu_clearBit8(6,b));
pc.add(1);
 return cycles;
} break;
case 177: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
c.assign(alu_clearBit8(6,c));
pc.add(1);
 return cycles;
} break;
case 178: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
d.assign(alu_clearBit8(6,d));
pc.add(1);
 return cycles;
} break;
case 179: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
e.assign(alu_clearBit8(6,e));
pc.add(1);
 return cycles;
} break;
case 180: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
h.assign(alu_clearBit8(6,h));
pc.add(1);
 return cycles;
} break;
case 181: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
l.assign(alu_clearBit8(6,l));
pc.add(1);
 return cycles;
} break;
case 182: {
cycles += 15;
// RES @r, (HL)
// Reference: zaks:82 page 385
write8(hl, alu_clearBit8(6,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 183: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
a.assign(alu_clearBit8(6,a));
pc.add(1);
 return cycles;
} break;
case 184: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
b.assign(alu_clearBit8(7,b));
pc.add(1);
 return cycles;
} break;
case 185: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
c.assign(alu_clearBit8(7,c));
pc.add(1);
 return cycles;
} break;
case 186: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
d.assign(alu_clearBit8(7,d));
pc.add(1);
 return cycles;
} break;
case 187: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
e.assign(alu_clearBit8(7,e));
pc.add(1);
 return cycles;
} break;
case 188: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
h.assign(alu_clearBit8(7,h));
pc.add(1);
 return cycles;
} break;
case 189: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
l.assign(alu_clearBit8(7,l));
pc.add(1);
 return cycles;
} break;
case 190: {
cycles += 15;
// RES @r, (HL)
// Reference: zaks:82 page 385
write8(hl, alu_clearBit8(7,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 191: {
cycles += 8;
// RES @r,@s
// Reference: zaks:82 page 385
a.assign(alu_clearBit8(7,a));
pc.add(1);
 return cycles;
} break;
case 192: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
b.assign(alu_setBit8(0,b));
pc.add(1);
 return cycles;
} break;
case 193: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
c.assign(alu_setBit8(0,c));
pc.add(1);
 return cycles;
} break;
case 194: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
d.assign(alu_setBit8(0,d));
pc.add(1);
 return cycles;
} break;
case 195: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
e.assign(alu_setBit8(0,e));
pc.add(1);
 return cycles;
} break;
case 196: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
h.assign(alu_setBit8(0,h));
pc.add(1);
 return cycles;
} break;
case 197: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
l.assign(alu_setBit8(0,l));
pc.add(1);
 return cycles;
} break;
case 198: {
cycles += 15;
// SET @r, (HL)
// Reference: zaks:82 page 425
write8(hl, alu_setBit8(0,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 199: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
a.assign(alu_setBit8(0,a));
pc.add(1);
 return cycles;
} break;
case 200: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
b.assign(alu_setBit8(1,b));
pc.add(1);
 return cycles;
} break;
case 201: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
c.assign(alu_setBit8(1,c));
pc.add(1);
 return cycles;
} break;
case 202: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
d.assign(alu_setBit8(1,d));
pc.add(1);
 return cycles;
} break;
case 203: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
e.assign(alu_setBit8(1,e));
pc.add(1);
 return cycles;
} break;
case 204: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
h.assign(alu_setBit8(1,h));
pc.add(1);
 return cycles;
} break;
case 205: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
l.assign(alu_setBit8(1,l));
pc.add(1);
 return cycles;
} break;
case 206: {
cycles += 15;
// SET @r, (HL)
// Reference: zaks:82 page 425
write8(hl, alu_setBit8(1,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 207: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
a.assign(alu_setBit8(1,a));
pc.add(1);
 return cycles;
} break;
case 208: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
b.assign(alu_setBit8(2,b));
pc.add(1);
 return cycles;
} break;
case 209: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
c.assign(alu_setBit8(2,c));
pc.add(1);
 return cycles;
} break;
case 210: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
d.assign(alu_setBit8(2,d));
pc.add(1);
 return cycles;
} break;
case 211: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
e.assign(alu_setBit8(2,e));
pc.add(1);
 return cycles;
} break;
case 212: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
h.assign(alu_setBit8(2,h));
pc.add(1);
 return cycles;
} break;
case 213: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
l.assign(alu_setBit8(2,l));
pc.add(1);
 return cycles;
} break;
case 214: {
cycles += 15;
// SET @r, (HL)
// Reference: zaks:82 page 425
write8(hl, alu_setBit8(2,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 215: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
a.assign(alu_setBit8(2,a));
pc.add(1);
 return cycles;
} break;
case 216: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
b.assign(alu_setBit8(3,b));
pc.add(1);
 return cycles;
} break;
case 217: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
c.assign(alu_setBit8(3,c));
pc.add(1);
 return cycles;
} break;
case 218: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
d.assign(alu_setBit8(3,d));
pc.add(1);
 return cycles;
} break;
case 219: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
e.assign(alu_setBit8(3,e));
pc.add(1);
 return cycles;
} break;
case 220: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
h.assign(alu_setBit8(3,h));
pc.add(1);
 return cycles;
} break;
case 221: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
l.assign(alu_setBit8(3,l));
pc.add(1);
 return cycles;
} break;
case 222: {
cycles += 15;
// SET @r, (HL)
// Reference: zaks:82 page 425
write8(hl, alu_setBit8(3,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 223: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
a.assign(alu_setBit8(3,a));
pc.add(1);
 return cycles;
} break;
case 224: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
b.assign(alu_setBit8(4,b));
pc.add(1);
 return cycles;
} break;
case 225: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
c.assign(alu_setBit8(4,c));
pc.add(1);
 return cycles;
} break;
case 226: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
d.assign(alu_setBit8(4,d));
pc.add(1);
 return cycles;
} break;
case 227: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
e.assign(alu_setBit8(4,e));
pc.add(1);
 return cycles;
} break;
case 228: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
h.assign(alu_setBit8(4,h));
pc.add(1);
 return cycles;
} break;
case 229: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
l.assign(alu_setBit8(4,l));
pc.add(1);
 return cycles;
} break;
case 230: {
cycles += 15;
// SET @r, (HL)
// Reference: zaks:82 page 425
write8(hl, alu_setBit8(4,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 231: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
a.assign(alu_setBit8(4,a));
pc.add(1);
 return cycles;
} break;
case 232: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
b.assign(alu_setBit8(5,b));
pc.add(1);
 return cycles;
} break;
case 233: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
c.assign(alu_setBit8(5,c));
pc.add(1);
 return cycles;
} break;
case 234: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
d.assign(alu_setBit8(5,d));
pc.add(1);
 return cycles;
} break;
case 235: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
e.assign(alu_setBit8(5,e));
pc.add(1);
 return cycles;
} break;
case 236: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
h.assign(alu_setBit8(5,h));
pc.add(1);
 return cycles;
} break;
case 237: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
l.assign(alu_setBit8(5,l));
pc.add(1);
 return cycles;
} break;
case 238: {
cycles += 15;
// SET @r, (HL)
// Reference: zaks:82 page 425
write8(hl, alu_setBit8(5,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 239: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
a.assign(alu_setBit8(5,a));
pc.add(1);
 return cycles;
} break;
case 240: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
b.assign(alu_setBit8(6,b));
pc.add(1);
 return cycles;
} break;
case 241: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
c.assign(alu_setBit8(6,c));
pc.add(1);
 return cycles;
} break;
case 242: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
d.assign(alu_setBit8(6,d));
pc.add(1);
 return cycles;
} break;
case 243: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
e.assign(alu_setBit8(6,e));
pc.add(1);
 return cycles;
} break;
case 244: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
h.assign(alu_setBit8(6,h));
pc.add(1);
 return cycles;
} break;
case 245: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
l.assign(alu_setBit8(6,l));
pc.add(1);
 return cycles;
} break;
case 246: {
cycles += 15;
// SET @r, (HL)
// Reference: zaks:82 page 425
write8(hl, alu_setBit8(6,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 247: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
a.assign(alu_setBit8(6,a));
pc.add(1);
 return cycles;
} break;
case 248: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
b.assign(alu_setBit8(7,b));
pc.add(1);
 return cycles;
} break;
case 249: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
c.assign(alu_setBit8(7,c));
pc.add(1);
 return cycles;
} break;
case 250: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
d.assign(alu_setBit8(7,d));
pc.add(1);
 return cycles;
} break;
case 251: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
e.assign(alu_setBit8(7,e));
pc.add(1);
 return cycles;
} break;
case 252: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
h.assign(alu_setBit8(7,h));
pc.add(1);
 return cycles;
} break;
case 253: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
l.assign(alu_setBit8(7,l));
pc.add(1);
 return cycles;
} break;
case 254: {
cycles += 15;
// SET @r, (HL)
// Reference: zaks:82 page 425
write8(hl, alu_setBit8(7,read8(hl)));
pc.add(1);
 return cycles;
} break;
case 255: {
cycles += 8;
// SET @r,@s
// Reference: zaks:82 page 425
a.assign(alu_setBit8(7,a));
pc.add(1);
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
uint8_t opcode = fetch8(pc);
uint8_t cycles = 1;
// INLINE EXPERIMENT - START
uint16_t ipc = pc.getUnsigned();
switch (ipc) {
}
// INLINE EXPERIMENT - END
switch (opcode) {
case 0: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 1: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 2: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 3: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 4: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 5: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 6: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 7: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 8: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 9: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 10: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 11: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 12: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 13: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 14: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 15: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 16: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 17: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 18: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 19: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 20: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 21: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 22: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 23: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 24: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 25: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 26: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 27: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 28: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 29: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 30: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 31: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 32: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 33: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 34: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 35: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 36: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 37: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 38: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 39: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 40: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 41: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 42: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 43: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 44: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 45: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 46: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 47: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 48: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 49: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 50: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 51: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 52: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 53: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 54: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 55: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 56: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 57: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 58: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 59: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 60: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 61: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 62: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 63: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 64: {
cycles += 12;
// IN @r,(C)
// Reference: zaks:82 page 261
// Reference: We don't set the H flag
b.assign(emf::in8(bc));
computeFlags8(b.getUnsigned());
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 65: {
cycles += 12;
// OUT (C),@r
// Reference: zaks:82 page 366
emf::out8(bc, b);
pc.add(1);
 return cycles;
} break;
case 66: {
cycles += 15;
// SBC HL,@r
// Reference: zaks:82 page 422
hl.assign(alu_sub_u16u16b(hl,bc,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 67: {
cycles += 15;
// LD (@n),@r
// Reference: zaks:82 page 321
write16(read16(pc.getUnsigned() + (1)),bc);
pc.add(3);
 return cycles;
} break;
case 68: {
cycles += 4;
// NEG
// Reference: zaks:82 page 358
tmp8.assign(a);
a.assign(alu_sub_u8u8b(0,a));
//if (tmp8.equals(0)) wasCarry=1;/*Zaks error*/;
//if (tmp8.getUnsigned() & (0x80)) wasParity=1;;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 69: {
cycles += 14;
// RETN
// Reference: zaks:82 page 394
pc.assign(read16(sp)-1);
sp.assign(alu_add_u16u16(sp,2));
int_iff1=int_iff2;
pc.add(1);
 return cycles;
} break;
case 70: {
cycles += 8;
// IM 0
// Reference: zaks:82 page 258
im0();
pc.add(1);
 return cycles;
} break;
case 71: {
cycles += 9;
// LD I,A
// Reference: zaks:82 page 332
intv.assign(a);
pc.add(1);
 return cycles;
} break;
case 72: {
cycles += 12;
// IN @r,(C)
// Reference: zaks:82 page 261
// Reference: We don't set the H flag
c.assign(emf::in8(bc));
computeFlags8(c.getUnsigned());
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 73: {
cycles += 12;
// OUT (C),@r
// Reference: zaks:82 page 366
emf::out8(bc, c);
pc.add(1);
 return cycles;
} break;
case 74: {
cycles += 15;
// ADC HL,@r
// Reference: zaks:82 page 192
hl.assign(alu_add_u16u16c(hl,bc,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 75: {
cycles += 20;
// LD @r,(@n)
// Reference: zaks:82 page 292
bc.assign(read16(read16(pc.getUnsigned() + (1))));
pc.add(3);
 return cycles;
} break;
case 76: {
cycles += 4;
// NEG
// Reference: zaks:82 page 358
tmp8.assign(a);
a.assign(alu_sub_u8u8b(0,a));
//if (tmp8.equals(0)) wasCarry=1;/*Zaks error*/;
//if (tmp8.getUnsigned() & (0x80)) wasParity=1;;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 77: {
cycles += 14;
// RETI
// Reference: zaks:82 page 392
pc.assign(read16(sp)-1);
sp.assign(alu_add_u16u16(sp,2));
pc.add(1);
 return cycles;
} break;
case 78: {
cycles += 8;
// IM 1
im1();
pc.add(1);
 return cycles;
} break;
case 79: {
cycles += 9;
// LD R,A
// Reference: zaks:82 page 344
memrefresh.assign(a);
memr7.assign(a);
pc.add(1);
 return cycles;
} break;
case 80: {
cycles += 12;
// IN @r,(C)
// Reference: zaks:82 page 261
// Reference: We don't set the H flag
d.assign(emf::in8(bc));
computeFlags8(d.getUnsigned());
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 81: {
cycles += 12;
// OUT (C),@r
// Reference: zaks:82 page 366
emf::out8(bc, d);
pc.add(1);
 return cycles;
} break;
case 82: {
cycles += 15;
// SBC HL,@r
// Reference: zaks:82 page 422
hl.assign(alu_sub_u16u16b(hl,de,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 83: {
cycles += 15;
// LD (@n),@r
// Reference: zaks:82 page 321
write16(read16(pc.getUnsigned() + (1)),de);
pc.add(3);
 return cycles;
} break;
case 84: {
cycles += 4;
// NEG
// Reference: zaks:82 page 358
tmp8.assign(a);
a.assign(alu_sub_u8u8b(0,a));
//if (tmp8.equals(0)) wasCarry=1;/*Zaks error*/;
//if (tmp8.getUnsigned() & (0x80)) wasParity=1;;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 85: {
cycles += 14;
// RETN
pc.assign(read16(sp)-1);
sp.assign(alu_add_u16u16(sp,2));
int_iff1=int_iff2;
pc.add(1);
 return cycles;
} break;
case 86: {
cycles += 8;
// IM 1
// Reference: zaks:82 page 259
im1();
pc.add(1);
 return cycles;
} break;
case 87: {
cycles += 9;
// LD A,I
// Reference: zaks:82 page 331
a.assign(intv);
wasParity = int_iff2 ? 1 : 0;
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 88: {
cycles += 12;
// IN @r,(C)
// Reference: zaks:82 page 261
// Reference: We don't set the H flag
e.assign(emf::in8(bc));
computeFlags8(e.getUnsigned());
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 89: {
cycles += 12;
// OUT (C),@r
// Reference: zaks:82 page 366
emf::out8(bc, e);
pc.add(1);
 return cycles;
} break;
case 90: {
cycles += 15;
// ADC HL,@r
// Reference: zaks:82 page 192
hl.assign(alu_add_u16u16c(hl,de,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 91: {
cycles += 20;
// LD @r,(@n)
// Reference: zaks:82 page 292
de.assign(read16(read16(pc.getUnsigned() + (1))));
pc.add(3);
 return cycles;
} break;
case 92: {
cycles += 4;
// NEG
// Reference: zaks:82 page 358
tmp8.assign(a);
a.assign(alu_sub_u8u8b(0,a));
//if (tmp8.equals(0)) wasCarry=1;/*Zaks error*/;
//if (tmp8.getUnsigned() & (0x80)) wasParity=1;;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 93: {
cycles += 14;
// RETN
pc.assign(read16(sp)-1);
sp.assign(alu_add_u16u16(sp,2));
int_iff1=int_iff2;
pc.add(1);
 return cycles;
} break;
case 94: {
cycles += 8;
// IM 2
// Reference: zaks:82 page 260
im2();
pc.add(1);
 return cycles;
} break;
case 95: {
cycles += 9;
// LD A,R
// Reference: zaks:82 page 333
a.assign(memrefresh.getUnsigned() | (memr7.getUnsigned() & 0x80));
wasParity = int_iff2 ? 1 : 0;
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 96: {
cycles += 12;
// IN @r,(C)
// Reference: zaks:82 page 261
// Reference: We don't set the H flag
h.assign(emf::in8(bc));
computeFlags8(h.getUnsigned());
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 97: {
cycles += 12;
// OUT (C),@r
// Reference: zaks:82 page 366
emf::out8(bc, h);
pc.add(1);
 return cycles;
} break;
case 98: {
cycles += 15;
// SBC HL,@r
// Reference: zaks:82 page 422
hl.assign(alu_sub_u16u16b(hl,hl,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 99: {
cycles += 15;
// LD (@n),@r
// Reference: zaks:82 page 321
write16(read16(pc.getUnsigned() + (1)),hl);
pc.add(3);
 return cycles;
} break;
case 100: {
cycles += 4;
// NEG
// Reference: zaks:82 page 358
tmp8.assign(a);
a.assign(alu_sub_u8u8b(0,a));
//if (tmp8.equals(0)) wasCarry=1;/*Zaks error*/;
//if (tmp8.getUnsigned() & (0x80)) wasParity=1;;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 101: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 102: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 103: {
cycles += 18;
// RRD
// Reference: zaks:82 page 416
tmp8.assign(alu_rrd8());
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 104: {
cycles += 12;
// IN @r,(C)
// Reference: zaks:82 page 261
// Reference: We don't set the H flag
l.assign(emf::in8(bc));
computeFlags8(l.getUnsigned());
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 105: {
cycles += 12;
// OUT (C),@r
// Reference: zaks:82 page 366
emf::out8(bc, l);
pc.add(1);
 return cycles;
} break;
case 106: {
cycles += 15;
// ADC HL,@r
// Reference: zaks:82 page 192
hl.assign(alu_add_u16u16c(hl,hl,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 107: {
cycles += 20;
// LD @r,(@n)
// Reference: zaks:82 page 292
hl.assign(read16(read16(pc.getUnsigned() + (1))));
pc.add(3);
 return cycles;
} break;
case 108: {
cycles += 4;
// NEG
// Reference: zaks:82 page 358
tmp8.assign(a);
a.assign(alu_sub_u8u8b(0,a));
//if (tmp8.equals(0)) wasCarry=1;/*Zaks error*/;
//if (tmp8.getUnsigned() & (0x80)) wasParity=1;;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 109: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 110: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 111: {
cycles += 18;
// RLD
// Reference: zaks:82 page 408
tmp8.assign(alu_rld8());
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 112: {
cycles += 12;
// IN (C)
tmp8.assign(emf::in8(bc));
computeFlags8(tmp8.getUnsigned());
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 113: {
cycles += 12;
// OUT (C),0
emf::out8(bc, 0);
pc.add(1);
 return cycles;
} break;
case 114: {
cycles += 15;
// SBC HL,@r
// Reference: zaks:82 page 422
hl.assign(alu_sub_u16u16b(hl,sp,cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 115: {
cycles += 15;
// LD (@n),@r
// Reference: zaks:82 page 321
write16(read16(pc.getUnsigned() + (1)),sp);
pc.add(3);
 return cycles;
} break;
case 116: {
cycles += 4;
// NEG
// Reference: zaks:82 page 358
tmp8.assign(a);
a.assign(alu_sub_u8u8b(0,a));
//if (tmp8.equals(0)) wasCarry=1;/*Zaks error*/;
//if (tmp8.getUnsigned() & (0x80)) wasParity=1;;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 117: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 118: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 119: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 120: {
cycles += 12;
// IN @r,(C)
// Reference: zaks:82 page 261
// Reference: We don't set the H flag
a.assign(emf::in8(bc));
computeFlags8(a.getUnsigned());
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 121: {
cycles += 12;
// OUT (C),@r
// Reference: zaks:82 page 366
emf::out8(bc, a);
pc.add(1);
 return cycles;
} break;
case 122: {
cycles += 15;
// ADC HL,@r
// Reference: zaks:82 page 192
hl.assign(alu_add_u16u16c(hl,sp,cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 123: {
cycles += 20;
// LD @r,(@n)
// Reference: zaks:82 page 292
sp.assign(read16(read16(pc.getUnsigned() + (1))));
pc.add(3);
 return cycles;
} break;
case 124: {
cycles += 4;
// NEG
// Reference: zaks:82 page 358
tmp8.assign(a);
a.assign(alu_sub_u8u8b(0,a));
//if (tmp8.equals(0)) wasCarry=1;/*Zaks error*/;
//if (tmp8.getUnsigned() & (0x80)) wasParity=1;;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 125: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 126: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 127: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 128: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 129: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 130: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 131: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 132: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 133: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 134: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 135: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 136: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 137: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 138: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 139: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 140: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 141: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 142: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 143: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 144: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 145: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 146: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 147: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 148: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 149: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 150: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 151: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 152: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 153: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 154: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 155: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 156: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 157: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 158: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 159: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 160: {
cycles += 16;
// LDI
// Reference: zaks:82 page 352
write8(de, read8(hl));
hl.assign(alu_add_u16u16(hl,1));
de.assign(alu_add_u16u16(de,1));
bc.assign(alu_add_u16u16(bc,-1));
if(bc.equals(0))wasParity=0;else wasParity=1;
clearFlagH();
clearFlagN();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 161: {
cycles += 16;
// CPI
// Reference: zaks:82 page 231
tmp8.assign(alu_sub_u8u8b(a, read8(hl)));
hl.assign(alu_add_u16u16(hl,1));
bc.assign(alu_add_u16u16(bc,-1));
if(bc.equals(0)) wasParity=0;else wasParity=1;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 162: {
cycles += 16;
// INI
// Reference: zaks:82 page 278
tmp8.assign(emf::in8(bc));
write8(hl,tmp8);
hl.assign(alu_add_u16u16(hl,1));
b.assign(alu_add_u8u8(b,-1));
if(bc.equals(0))wasZero=1;else wasZero=0;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 163: {
cycles += 16;
// OUTI
// Reference: zaks:82 page 371
// Reference: confusion whether should N be set or not?
emf::out8(bc, read8(hl));
hl.assign(alu_add_u16u16(hl,1));
b.assign(alu_add_u8u8(b,-1));
if(bc.equals(0))wasZero=1;else wasZero=0;
setFlagN();
affectFlagZ();
pc.add(1);
 return cycles;
} break;
case 164: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 165: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 166: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 167: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 168: {
cycles += 16;
// LDD
// Reference: zaks:82 page 348
write8(de, read8(hl));
de.assign(alu_add_u16u16(de,-1));
hl.assign(alu_add_u16u16(hl,-1));
bc.assign(alu_add_u16u16(bc,-1));
if(bc.equals(0)) wasParity=0;else wasParity=1;
clearFlagH();
clearFlagN();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 169: {
cycles += 16;
// CPD
// Reference: zaks:82 page 227
tmp8.assign(alu_sub_u8u8b(a,read8(hl)));
hl.assign(alu_add_u16u16(hl,-1));
bc.assign(alu_add_u16u16(bc,-1));
if(bc.equals(0))wasParity=0;else wasParity=1;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 170: {
cycles += 16;
// IND
// Reference: zaks:82 page 274
tmp8.assign(emf::in8(bc));
write8(hl,tmp8);
b.assign(alu_add_u8u8(b,-1));
hl.assign(alu_add_u16u16(hl,-1));
if(b.equals(0)) wasZero=1; else wasZero=0;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 171: {
cycles += 16;
// OUTD
// Reference: zaks:82 page 369
emf::out8(bc, read8(hl));
hl.assign(alu_add_u16u16(hl,-1));
b.assign(alu_add_u8u8(b,-1));
if(bc.equals(0))wasZero=1;else wasZero=0;
setFlagN();
affectFlagZ();
pc.add(1);
 return cycles;
} break;
case 172: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 173: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 174: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 175: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 176: {
cycles += 16;
// LDIR
// Reference: zaks:82 page 354
// Reference: The book indicates to clear the PV flag, but http://www.z80.info/z80sflag.htm suggests otherwise
write8(de, read8(hl));
de.assign(alu_add_u16u16(de,1));
hl.assign(alu_add_u16u16(hl,1));
bc.assign(alu_add_u16u16(bc,-1));
if (bc.notEquals(0)){ pc.sub(2); };
if (bc.equals(0)) wasOverflow=0;else wasOverflow=1;
clearFlagH();
clearFlagN();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 177: {
cycles += 16;
// CPIR
// Reference: zaks:82 page 233
tmp8.assign(alu_sub_u8u8b(a,read8(hl)));
hl.assign(alu_add_u16u16(hl, 1));
bc.assign(alu_add_u16u16(bc,-1));
if (bc.notEquals(0) && tmp8.notEquals(0)) { pc.sub(2); } ;
if (bc.equals(0)) wasOverflow=0;else wasOverflow=1;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(1);
 return cycles;
} break;
case 178: {
cycles += 16;
// INIR
// Reference: zaks:82 page 280
tmp8.assign(emf::in8(bc));
write8(hl,tmp8);
hl.assign(alu_add_u16u16(hl,1));
b.assign(alu_add_u8u8(b,-1));
if(b.notEquals(0)){ pc.sub(2); };
setFlagN();
setFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 179: {
cycles += 16;
// OTIR
// Reference: zaks:82 page 364
emf::out8(bc, read8(hl));
hl.assign(alu_add_u16u16(hl,1));
b.assign(alu_add_u8u8(b,-1));
if(b.notEquals(0)){ pc.sub(2); };
setFlagN();
setFlagZ();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 180: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 181: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 182: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 183: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 184: {
cycles += 16;
// LDDR
// Reference: zaks:82 page 350
write8(de, read8(hl));
de.assign(alu_add_u16u16(de,-1));
hl.assign(alu_add_u16u16(hl,-1));
bc.assign(alu_add_u16u16(bc,-1));
if (bc.equals(0)) wasParity=0; else wasParity=1;
if (bc.notEquals(0)) {pc.sub(2);} ;
clearFlagH();
clearFlagN();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 185: {
cycles += 16;
// CPDR
// Reference: zaks:82 page 229
tmp8.assign(alu_sub_u8u8b(a,read8(hl)));
hl.assign(alu_add_u16u16(hl,-1));
bc.assign(alu_add_u16u16(bc,-1));
if (bc.equals(0)) wasParity=0; else wasParity=1;
if (bc.notEquals(0) && tmp8.notEquals(0)) {pc.sub(2);} ;
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 186: {
cycles += 16;
// INDR
// Reference: zaks:82 page 276
tmp8.assign(emf::in8(bc));
write8(hl,tmp8);
hl.assign(alu_add_u16u16(hl,-1));
b.assign(alu_add_u8u8(b,-1));
if (b.notEquals(0)) { pc.sub(2); };
setFlagN();
affectFlagP();
pc.add(1);
 return cycles;
} break;
case 187: {
cycles += 16;
// OTDR
// Reference: zaks:82 page 362
emf::out8(bc, read8(hl));
hl.assign(alu_add_u16u16(hl,-1));
b.assign(alu_add_u8u8(b,-1));
if (b.notEquals(0)){ pc.sub(2); };
setFlagZ();
setFlagN();
pc.add(1);
 return cycles;
} break;
case 188: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 189: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 190: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 191: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 192: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 193: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 194: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 195: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 196: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 197: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 198: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 199: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 200: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 201: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 202: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 203: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 204: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 205: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 206: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 207: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 208: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 209: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 210: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 211: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 212: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 213: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 214: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 215: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 216: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 217: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 218: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 219: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 220: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 221: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 222: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 223: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 224: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 225: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 226: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 227: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 228: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 229: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 230: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 231: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 232: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 233: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 234: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 235: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 236: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 237: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 238: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 239: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 240: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 241: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 242: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 243: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 244: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 245: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 246: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 247: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 248: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 249: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 250: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 251: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 252: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 253: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 254: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 255: {
// Unknown instruction
pc.add(1);
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
uint8_t bit;
uint8_t opcode = fetch8(pc);
uint8_t cycles = 1;
// INLINE EXPERIMENT - START
uint16_t ipc = pc.getUnsigned();
switch (ipc) {
}
// INLINE EXPERIMENT - END
switch (opcode) {
case 0: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 1: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 2: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 3: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 4: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 5: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 6: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 7: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 8: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 9: {
cycles += 15;
// ADD IY,@r
// Reference: zaks:82 page 207
iy.assign(alu_add_u16u16c(iy,bc));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 10: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 11: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 12: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 13: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 14: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 15: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 16: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 17: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 18: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 19: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 20: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 21: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 22: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 23: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 24: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 25: {
cycles += 15;
// ADD IY,@r
// Reference: zaks:82 page 207
iy.assign(alu_add_u16u16c(iy,de));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 26: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 27: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 28: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 29: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 30: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 31: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 32: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 33: {
cycles += 15;
// LD IY,@n
// Reference: zaks:82 page 340
iy.assign(read16(pc.getUnsigned() + (1)));
pc.add(3);
 return cycles;
} break;
case 34: {
cycles += 20;
// LD (@n),IY
// Reference: zaks:82 page 327
write16(read16(pc.getUnsigned() + (1)),iy);
pc.add(3);
 return cycles;
} break;
case 35: {
cycles += 10;
// INC IY
// Reference: zaks:82 page 273
iy.assign(alu_add_u16u16c(iy,1));
pc.add(1);
 return cycles;
} break;
case 36: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 37: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 38: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 39: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 40: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 41: {
cycles += 15;
// ADD IY,@r
// Reference: zaks:82 page 207
iy.assign(alu_add_u16u16c(iy,iy));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 42: {
cycles += 20;
// LD IY,(@n)
// Reference: zaks:82 page 342
iy.assign(read16(read16(pc.getUnsigned() + (1))));
pc.add(3);
 return cycles;
} break;
case 43: {
cycles += 10;
// DEC IY
// Reference: zaks:82 page 243
iy.assign(alu_sub_u16u16(iy,1));
pc.add(1);
 return cycles;
} break;
case 44: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 45: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 46: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 47: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 48: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 49: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 50: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 51: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 52: {
cycles += 23;
// INC (IY+@n)
// Reference: zaks:82 page 270
tmp8.assign(alu_add_u8u8c(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))),1));
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(2);
 return cycles;
} break;
case 53: {
cycles += 15;
// DEC (IY+@n)
// Reference: zaks:82 page 238
tmp8.assign(alu_sub_u8u8b(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))),1));
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),tmp8);
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(2);
 return cycles;
} break;
case 54: {
cycles += 19;
// LD (IY+@d),@n
// Reference: zaks:82 page 311
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),read8(pc.getUnsigned() + (2)));
pc.add(3);
 return cycles;
} break;
case 55: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 56: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 57: {
cycles += 15;
// ADD IY,@r
// Reference: zaks:82 page 207
iy.assign(alu_add_u16u16c(iy,sp));
clearFlagN();
affectFlagH();
affectFlagC();
pc.add(1);
 return cycles;
} break;
case 58: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 59: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 60: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 61: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 62: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 63: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 64: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 65: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 66: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 67: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 68: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 69: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 70: {
cycles += 19;
// LD @r,(IY+@n)
// Reference: zaks:82 page 307
b.assign(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 71: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 72: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 73: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 74: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 75: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 76: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 77: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 78: {
cycles += 19;
// LD @r,(IY+@n)
// Reference: zaks:82 page 307
c.assign(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 79: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 80: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 81: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 82: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 83: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 84: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 85: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 86: {
cycles += 19;
// LD @r,(IY+@n)
// Reference: zaks:82 page 307
d.assign(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 87: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 88: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 89: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 90: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 91: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 92: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 93: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 94: {
cycles += 19;
// LD @r,(IY+@n)
// Reference: zaks:82 page 307
e.assign(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 95: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 96: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 97: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 98: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 99: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 100: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 101: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 102: {
cycles += 19;
// LD @r,(IY+@n)
// Reference: zaks:82 page 307
h.assign(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 103: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 104: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 105: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 106: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 107: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 108: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 109: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 110: {
cycles += 19;
// LD @r,(IY+@n)
// Reference: zaks:82 page 307
l.assign(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 111: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 112: {
cycles += 19;
// LD (IY+@n),@r
// Reference: zaks:82 page 315
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))), b);
pc.add(2);
 return cycles;
} break;
case 113: {
cycles += 19;
// LD (IY+@n),@r
// Reference: zaks:82 page 315
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))), c);
pc.add(2);
 return cycles;
} break;
case 114: {
cycles += 19;
// LD (IY+@n),@r
// Reference: zaks:82 page 315
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))), d);
pc.add(2);
 return cycles;
} break;
case 115: {
cycles += 19;
// LD (IY+@n),@r
// Reference: zaks:82 page 315
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))), e);
pc.add(2);
 return cycles;
} break;
case 116: {
cycles += 19;
// LD (IY+@n),@r
// Reference: zaks:82 page 315
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))), h);
pc.add(2);
 return cycles;
} break;
case 117: {
cycles += 19;
// LD (IY+@n),@r
// Reference: zaks:82 page 315
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))), l);
pc.add(2);
 return cycles;
} break;
case 118: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 119: {
cycles += 19;
// LD (IY+@n),@r
// Reference: zaks:82 page 315
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))), a);
pc.add(2);
 return cycles;
} break;
case 120: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 121: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 122: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 123: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 124: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 125: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 126: {
cycles += 19;
// LD @r,(IY+@n)
// Reference: zaks:82 page 307
a.assign(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))));
pc.add(2);
 return cycles;
} break;
case 127: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 128: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 129: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 130: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 131: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 132: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 133: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 134: {
cycles += 19;
// ADD A,(IY+@n)
// Reference: zaks:82 page 198
a.assign(alu_add_u8u8c(a,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 135: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 136: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 137: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 138: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 139: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 140: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 141: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 142: {
cycles += 15;
// ADC A,(IY+@n)
// Reference: zaks:82 page 190
a.assign(alu_add_u8u8c(a,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))),cc_bit_0));
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
pc.add(2);
 return cycles;
} break;
case 143: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 144: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 145: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 146: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 147: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 148: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 149: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 150: {
cycles += 15;
// SUB (IY+@n)
// Reference: zaks:82 page 434
a.assign(alu_sub_u8u8b(a,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 151: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 152: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 153: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 154: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 155: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 156: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 157: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 158: {
cycles += 19;
// SBC A,(IY+@n)
// Reference: zaks:82 page 420
a.assign(alu_sub_u8u8b(a,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))),cc_bit_0));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 159: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 160: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 161: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 162: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 163: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 164: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 165: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 166: {
cycles += 19;
// AND (IY+@n)
// Reference: zaks:82 page 209
a.assign(alu_and8(a,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
clearFlagN();
clearFlagC();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(2);
 return cycles;
} break;
case 167: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 168: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 169: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 170: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 171: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 172: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 173: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 174: {
cycles += 15;
// XOR A,(IY+@n)
// Reference: zaks:82 page 436
a.assign(alu_xor8(a,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
clearFlagH();
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(2);
 return cycles;
} break;
case 175: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 176: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 177: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 178: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 179: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 180: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 181: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 182: {
cycles += 19;
// OR (IY+@n)
// Reference: zaks:82 page 360
a.assign(alu_or8(a,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
clearFlagN();
clearFlagC();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(2);
 return cycles;
} break;
case 183: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 184: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 185: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 186: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 187: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 188: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 189: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 190: {
cycles += 19;
// CP A,(IY+@n)
// Reference: zaks:82 page 225
tmp8.assign(alu_sub_u8u8b(a,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
setFlagN();
affectFlagS();
affectFlagZ();
affectFlagH();
affectFlagV();
affectFlagC();
pc.add(2);
 return cycles;
} break;
case 191: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 192: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 193: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 194: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 195: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 196: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 197: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 198: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 199: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 200: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 201: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 202: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 203: {
cycles += 23;
// RLC (IY+@n)
// Reference: zaks:82 page 406
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x6) {
tmp8.assign(alu_rlc8(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// RRC (IY+@n)
// Reference: zaks:82 page 413
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0xe) {
tmp8.assign(alu_rrc8(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// RL (IY+@n)
// Reference: zaks:82 page 396
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x16) {
tmp8.assign(alu_rl8(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))), cc_bit_0));
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// RR (IY+@n)
// Reference: zaks:82 page 410
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x1e) {
tmp8.assign(alu_rr8(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1)))), cc_bit_0));
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// SLA (IY+@n)
// Reference: zaks:82 page 428
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x26) {
tmp8.assign(alu_sla8(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// SRA (IY+@n)
// Reference: zaks:82 page 430
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x2e) {
tmp8.assign(alu_sra8(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 15;
// SLL (IY+@n)
// Reference:  page 
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x36) {
tmp8.assign(alu_sll8(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 23;
// SRL (IY+@n)
// Reference: zaks:82 page 432
if ((read8(pc.getUnsigned() + (2))& 0xff) == 0x3e) {
tmp8.assign(alu_srl8(read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))),tmp8);
clearFlagH();
clearFlagN();
affectFlagS();
affectFlagZ();
affectFlagP();
affectFlagC();
pc.add(3);
 return cycles;
}
cycles += 20;
// BIT @r,(IY+@n)
// Reference: zaks:82 page 213
if ((read8(pc.getUnsigned() + (2))& 0xc7) == 0x46) {
uint8_t bit = (read8(alu_add_u16u16(pc,2)) & 0x38)>>3;;
tmp8.assign(alu_testBit8(bit,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
clearFlagN();
setFlagH();
affectFlagS();
affectFlagZ();
affectFlagP();
pc.add(3);
 return cycles;
}
cycles += 15;
// RES @r,(IY+@n)
// Reference: zaks:82 page 385
if ((read8(pc.getUnsigned() + (2))& 0xc7) == 0x86) {
uint8_t bit = (read8(alu_add_u16u16(pc,2)) & 0x38)>>3;;
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))), alu_clearBit8(bit,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
pc.add(3);
 return cycles;
}
cycles += 23;
// SET @r,(IY+@n)
// Reference: zaks:82 page 425
if ((read8(pc.getUnsigned() + (2))& 0xc7) == 0xc6) {
uint8_t bit = (read8(alu_add_u16u16(pc,2)) & 0x38)>>3;;
write8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))), alu_setBit8(bit,read8(alu_add_u16s8(iy,read8(pc.getUnsigned() + (1))))));
pc.add(3);
 return cycles;
}
} break;
case 204: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 205: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 206: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 207: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 208: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 209: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 210: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 211: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 212: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 213: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 214: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 215: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 216: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 217: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 218: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 219: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 220: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 221: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 222: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 223: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 224: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 225: {
cycles += 14;
// POP IY
// Reference: zaks:82 page 377
iy.assign(read16(sp));
sp.assign(alu_add_u16u16(sp,2));
pc.add(1);
 return cycles;
} break;
case 226: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 227: {
cycles += 23;
// EX (SP),IY
// Reference: zaks:82 page 254
tmp16.assign(iy);
iy.assign(read16(sp));
write16(sp,tmp16);
pc.add(1);
 return cycles;
} break;
case 228: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 229: {
cycles += 15;
// PUSH IY
// Reference: zaks:82 page 383
sp.assign(alu_sub_u16u16(sp,2));
write16(sp,iy);
pc.add(1);
 return cycles;
} break;
case 230: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 231: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 232: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 233: {
cycles += 8;
// JP (IY)
// Reference: zaks:82 page 286
pc.assign(alu_add_u16u16(iy, -1));
pc.add(1);
 return cycles;
} break;
case 234: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 235: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 236: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 237: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 238: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 239: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 240: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 241: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 242: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 243: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 244: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 245: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 246: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 247: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 248: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 249: {
cycles += 10;
// LD SP,IY
// Reference: zaks:82 page 347
sp.assign(iy);
pc.add(1);
 return cycles;
} break;
case 250: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 251: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 252: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 253: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 254: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
case 255: {
// Unknown instruction
pc.add(1);
 return cycles;
} break;
}
// Even if no instruction was found (maybe, because there's holes in the instructions map), we still want to indicate that time has passed
return 1;
}
// importEmulatorALU
// POSTTEST


void getState(struct Z80State *pState) {
    pState->pc = pc.getUnsigned();
    pState->sp = sp.getUnsigned();
    pState->ix = ix.getUnsigned();
    pState->iy = iy.getUnsigned();
    pState->a = a.getUnsigned();
    pState->b = b.getUnsigned();
    pState->c = c.getUnsigned();
    pState->d = d.getUnsigned();
    pState->e = e.getUnsigned();
    pState->h = h.getUnsigned();
    pState->l = l.getUnsigned();
    pState->prime_b = prime_b.getUnsigned();
    pState->prime_c = prime_c.getUnsigned();
    pState->prime_h = prime_h.getUnsigned();
    pState->prime_l = prime_l.getUnsigned();
}

};

