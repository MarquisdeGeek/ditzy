#include "emf/emf.h"

#include "platforms/zx81/common/zxmemory.h"

#include "esp_screen.h"

#include <TFT_eSPI.h>

extern TFT_eSPI tft;


// 1. Renderer class wraps ESP
ESP_Render::ESP_Render() {
    // nop
}

// TODO: Find out why I get "Guru Meditation Error" when using this abstraction
void
ESP_Render::fillRect(int x, int y, int w, int h, uint32_t col) {
    tft.fillRect(x, y, w, h, col);
}


void
ESP_Render::clearScreen(uint32_t col) {
    tft.fillRect(
        0,0,
        240,320,
        col
    );
}



// 2. Our chess- and ESP-specific
ESP_ZXScreen::ESP_ZXScreen(int scale, bool showComparison) : CommonScreen(scale, showComparison) {
    m_ifaceWidth = m_Scale * m_fontWidth * 10; // 10=8 board + 2 edge
    m_ifaceHeight = m_Scale * m_fontHeight * 11; // 11=8 board + 1 ABC notation + 2 edge

    m_gapX = m_gapY = 0;
}


void
ESP_ZXScreen::getOffset(ScreenPoint* pOffset, int _) const {
    pOffset->x = 0;
    pOffset->y = -3 * (m_Scale * m_fontWidth);
}


void
ESP_ZXScreen::clearRestOfLine(ScreenPoint* pOffset, int screenX, int screenY) {
    const uint32_t col = TFT_OLIVE;
    tft.fillRect(
        screenX * m_fontWidth * m_Scale, screenY * m_fontHeight * m_Scale,
        (32 - screenX) * m_fontWidth * m_Scale, m_fontHeight * m_Scale,
        col
    );
}


void
ESP_ZXScreen::drawCharacterRects(ScreenPoint* pOffset,  int screenX, int screenY, int ch) {
    int rcX = screenX * m_fontWidth * m_Scale;
    int rcY = screenY * m_fontHeight * m_Scale;

    rcX += pOffset->x;
    rcY += pOffset->y;
    
    const uint8_t *pFont = rom;

    pFont += 0x1d00;
    pFont += 8*32;
    pFont += (ch & 0x7f) * 8;

    for(int j=0;j<m_fontHeight;++j) {
        uint8_t b = *pFont++;
        if (ch>127) {
            b = ~b;
        }

        for(signed int i=m_fontWidth-1;i>=0;--i) {
            // RRGGBBAA
            uint32_t col = ((1<<i) & b) ? TFT_GOLD : TFT_BLACK;

            tft.fillRect(
                rcX, rcY,
                m_Scale, m_Scale,
                col
            );

            rcX += m_Scale;
        }
        rcX -= m_fontWidth * m_Scale;
        rcY += m_Scale;
    }
}


bool
ESP_ZXScreen::getSquareAt(ChessboardPoint* pChessboard, int x, int y) const {
    int squareX = x / 32; // 320 (width) / 10 (squares) = 32 px/sq;
    int squareY = y / 18; // 240 (width) / 13 (squares) = 18 px/sq;

    squareX -= 2;
    squareY -= 2;

    if (squareX < 0 || squareY < 0) {
        return false;
    }
    if (squareX >= 8 || squareY >= 8) {
        return false;
    }

    pChessboard->x = 7 - (squareX); // 0-7
    pChessboard->y = squareY; // 0-7

    return true;
}
