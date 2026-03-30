#include "emf/types.h"

#include "platforms/zx81/common/c/zxcio.h"
#include "platforms/zx81/common/c/zxckeyboard.h"


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

        switch (h) {
          case 0xfe:
            retval = zxckeyboard_getRow(0);
            break;
          case 0xfd:
            retval = zxckeyboard_getRow(1);
            break;
          case 0xfb:
            retval = zxckeyboard_getRow(2);
            break;
          case 0xf7:
            retval = zxckeyboard_getRow(3);
            break;
          case 0xef:
            retval = zxckeyboard_getRow(4);
            break;
          case 0xdf:
            retval = zxckeyboard_getRow(5);
            break;
          case 0xbf:
            retval = zxckeyboard_getRow(6);
            break;
          case 0x7f:
            retval = zxckeyboard_getRow(7);
            break;

          default:
            for (uint8_t i = 0, mask = 1; i < 8U; i++, mask <<= 1) {
              if (!(h & mask)) {
                retval &= zxckeyboard_getRow(i);
              }
            }
        }//switch(h)
        //}//FE
    }//switch(a)

    return retval;
}

void out8(uint16_t addr, uint8_t d) {

}

