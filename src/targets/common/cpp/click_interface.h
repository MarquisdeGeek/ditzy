#ifndef TARGETS_COMMON_CLICK_INTERFACE_H
#define TARGETS_COMMON_CLICK_INTERFACE_H 1


#include "emf/emf.h"
#include "screen.h"

// #include "platforms/zx81/common/cpp/zxkeyboard.h"


// The platform should implement these methods:
void onKeyDown(int keycode);
void onKeyUp(int keycode);


// And use this
class ClickInterface {
public:
    bool clickAt(CommonScreen& zxscreen, int x, int y);
    bool pumpKeypresses();

protected:
    uint8_t* m_pkeyBuffer = nullptr;
    uint8_t  keyBuffer[5]={'-','-','-','-',0};

};

#endif // TARGETS_COMMON_CLICK_INTERFACE_H
