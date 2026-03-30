#include "emf/emf.h"
#include "screen.h"
#include "click_interface.h"

// #include "platforms/zx81/common/cpp/zxkeyboard.h"

// #include <Arduino.h> 

// Timer to handle fake keypresses, from mouse
// uint8_t* m_pkeyBuffer = nullptr;
// uint8_t keyBuffer[5]={'-','-','-','-',0};



bool 
ClickInterface::clickAt(CommonScreen& zxscreen, int x, int y) {
    // If it's already in process - ignore
    if (m_pkeyBuffer) {
        return false;
    }
    // Else determine the new board square
    ChessboardPoint pt;

    bool onBoard = zxscreen.getSquareAt(&pt, x, y);

    if (onBoard) {
        keyBuffer[0] = pt.y + '1';
        keyBuffer[2] = pt.x + 'A';

        m_pkeyBuffer = keyBuffer;
    }

    return onBoard;
}

// Return true if there's more presses required
bool
ClickInterface::pumpKeypresses() {
    // Not sending keys ATM...
    if (!m_pkeyBuffer) {
        return false;
    }
    //
    int idx = m_pkeyBuffer - keyBuffer;
    //
    switch(idx) {
        case 0:
        onKeyDown(keyBuffer[0]);
        break;

        case 1:
        onKeyUp(keyBuffer[0]);
        break;

        case 2:
        onKeyDown(keyBuffer[2]);
        break;

        case 3:
        onKeyUp(keyBuffer[2]);
        break;

        case 4:
        m_pkeyBuffer = nullptr;
        return false;
    }

    ++m_pkeyBuffer;

    return true;
}
