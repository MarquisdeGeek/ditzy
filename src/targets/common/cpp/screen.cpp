#include "emf/emf.h"

#include "platforms/zx81/common/zxmemory.h"
#include "platforms/zx81/1k/cpp/zxmemory.h"
#include "platforms/zx81/1k/c/mem1k.h"

#include "screen.h"


CommonScreen::CommonScreen(int scale, bool showComparison) : m_Scale(scale), m_bShowComparison(showComparison) {

    m_ifaceWidth = m_Scale * m_fontWidth * 32;
    m_ifaceHeight = m_Scale * m_fontHeight * 24;

    m_gapX = m_Scale * m_fontWidth;
    m_gapY = m_Scale * m_fontHeight;
}


void
CommonScreen::setRenderer(BaseRender* pRenderer) {
    m_pRenderer = pRenderer;
}


int
CommonScreen::getWidth() const {

    if (m_bShowComparison) {
        return m_gapX + m_ifaceWidth + m_gapX*2 + m_ifaceWidth + m_gapX;
    }

    return m_gapX + m_ifaceWidth + m_gapX;
}


int
CommonScreen::getHeight() const {

    return m_gapY + m_ifaceHeight + m_gapY;
}


void
CommonScreen::getOffset(ScreenPoint* pOffset, int position) const {
    pOffset->x = m_gapX;
    pOffset->y = m_gapY;

    if (position) {
        pOffset->x += m_ifaceWidth + m_gapX*2;
    }
}


void
CommonScreen::drawScreen(ScreenPoint* pOffset, cbRead8 r8, cbRead16 r16) {
    uint16_t dfile = r16(16396);
    uint16_t ptr = dfile + 1; // ignore first 118
    int x = 0;
    for(int row=0;row<24;) {
        uint8_t byte = r8(ptr);
        // the x>31 check stops spurious crap appearing in the border
        // if the DFILE is malformed. (As usually happens on reboot, when
        // the DFILE has not been initialised)
        if (byte == 118 || x > 31) {
            clearRestOfLine(pOffset, x, row);
            row++;
            x = 0;
        } else {
            drawCharacterRects(pOffset, x, row, byte);
            ++x;
        }
        //
        ++ptr;
    }  
}


void
CommonScreen::drawC(ScreenPoint* pOffset) {
    drawScreen(pOffset, memory_read8, memory_read16);
}


void
CommonScreen::drawCPP(ScreenPoint* pOffset) {
    drawScreen(pOffset, read8, read16);
}


void
CommonScreen::clearBackground(ScreenPoint* _) {
    const uint32_t col = colLight;
    m_pRenderer->clearScreen(col);
}


void
CommonScreen::clearRestOfLine(ScreenPoint* pOffset, int screenX, int screenY) {

    int x = screenX * m_fontWidth * m_Scale;
    int y = screenY * m_fontHeight * m_Scale;
    int w = (32 - screenX) * m_fontWidth * m_Scale;
    int h = m_fontHeight * m_Scale;

    x += pOffset->x;
    y += pOffset->y;

    const uint32_t col = colLight;
    m_pRenderer->fillRect(x, y, w, h, col);
}


void
CommonScreen::drawCharacterRects(ScreenPoint* pOffset, int screenX, int screenY, int ch) {

    int x = screenX * m_fontWidth * m_Scale;
    int y = screenY * m_fontHeight * m_Scale;
    int w = m_Scale;
    int h = m_Scale;

    x += pOffset->x;
    y += pOffset->y;

    const uint8_t *pFont = rom;

    pFont += 0x1d00;
    pFont += 8*32;
    pFont += (ch & 0x7f) * 8;

    for(int j=0;j<m_fontHeight;++j) {
        uint8_t b = *pFont++;
        if (ch>127) {
            b = ~b;
        }

        for(int i=m_fontWidth-1;i>=0;--i) {
            // RRGGBBAA
            uint32_t col = ((1<<i) & b) ? colDark : colLight;
            m_pRenderer->fillRect(x, y, w, h, col);
            x += m_Scale;
        }
        x -= m_fontWidth * m_Scale;
        y += m_Scale;
    }
}


bool
CommonScreen::getSquareAt(ChessboardPoint* pChessboard, int x, int y) const {
    // Only worry about the LHS board
    int xoff = x - m_gapX*3; // 1 gap + 2 iface chars
    int yoff = y - m_gapY*2; // 1 gap + 1 iface row (DRH)

    if (xoff < 0 || yoff < 0) {
        return false;
    }
    if (xoff >= m_ifaceWidth || yoff >= m_ifaceHeight) {
        return false;
    }

    // REM: The gap is the same width as a character block
    pChessboard->x = 7 - (xoff / m_gapX); // 0-7
    pChessboard->y = yoff / m_gapY; // 0-7

    return true;
}
