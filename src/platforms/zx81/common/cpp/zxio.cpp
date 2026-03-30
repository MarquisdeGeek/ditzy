#include "emf/emf.h"
#include "zxio.h"
#include "platforms/zx81/common/cpp/zxkeyboard.h"


extern ZXKeyboard zx81_keyboard;
extern int ticks;

namespace emf {

uint8_t in8(uint16_t addr) {
    uint8_t a = addr & 0xff;
    uint8_t h = addr >> 8;

    return in8hilo(h, a);
}


uint8_t in8hilo(uint8_t h, uint8_t a) { //hi,lo

    uint8_t retval = 0xff;

    switch (a) {
      case 0xFE:
        // bus.ula.setHSYNCGenerator(false);

        //printf("ticks= %d (*h=%d) at pc=%d op=%d\n", ticks, h, cstate.pc, memory_read8(cstate.pc));
        switch (h) {
          case 0xfe:
            retval = zx81_keyboard.getRow(0);
            break;
          case 0xfd:
            retval = zx81_keyboard.getRow(1);
            break;
          case 0xfb:
            retval = zx81_keyboard.getRow(2);
            break;
          case 0xf7:
            retval = zx81_keyboard.getRow(3);
            break;
          case 0xef:
            retval = zx81_keyboard.getRow(4);
            break;
          case 0xdf:
            retval = zx81_keyboard.getRow(5);
            break;
          case 0xbf:
            retval = zx81_keyboard.getRow(6);
            break;
          case 0x7f:
            retval = zx81_keyboard.getRow(7);
            break;

          default:
            for (int i = 0, mask = 1; i < 8; i++, mask <<= 1) {
              if (!(h & mask)) {
                retval &= zx81_keyboard.getRow(i);
              }
            }
        }
    }

    return retval;
}


void out8(uint16_t a, uint8_t b) {

    switch (a) {
      case 0xFD:
        // bus.ula.setNMIGenerator(false);
        // bus.ula.setHSYNCGenerator(true);
        break;

      case 0xFE:
        // bus.ula.setNMIGenerator(true);
        break;

      case 0x07:
      case 0xFF:
        // bus.ula.setHSYNCGenerator(true);
        break;
    }

    return ;

}

}
