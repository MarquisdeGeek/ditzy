#include "sdl_screen.h"


// 1. Renderer class wraps SDL
SDL_Render::SDL_Render(SDL_Surface* pSurface) : m_pSurface(pSurface) {
    // nop
}


void
SDL_Render::fillRect(int x, int y, int w, int h, uint32_t col) {
    SDL_Rect rc = { x, y, w, h };
    SDL_FillRect(m_pSurface, &rc, col);
}


void
SDL_Render::clearScreen(uint32_t col) {
    SDL_FillRect(m_pSurface, nullptr, col);
}



// 2. Our chess- and SDL-specific
SDL_ZXScreen::SDL_ZXScreen(int scale, bool showComparison) : CommonScreen(scale, showComparison) {

#if CHESS_ONLY_OPTIMISATIONS
    m_ifaceWidth = m_Scale * m_fontWidth * 10; // 10=8 board + 2 edge
    m_ifaceHeight = m_Scale * m_fontHeight * 11; // 11=8 board + 1 ABC notation + 2 edge
#endif
}


void
SDL_ZXScreen::getOffset(ScreenPoint* pOffset, int position) const {

    CommonScreen::getOffset(pOffset, position);

#if CHESS_ONLY_OPTIMISATIONS
    // 4=fudge factor for the blank lines in the DFILE
    pOffset->y -= 4 * m_gapY;
#endif
}


void
SDL_ZXScreen::clearBackground(ScreenPoint* pOffset) {
    const uint32_t col = 0xddaa7700;

#if CHESS_ONLY_OPTIMISATIONS
    // REM: offset is the top of the board, but we need some tweaks
    // because of the extra 118s, and border
    int x = pOffset->x - m_gapX;
    int y = pOffset->y + (3 * m_gapY);
    int w = m_ifaceWidth + m_gapX * 2;
    int h = m_ifaceHeight + m_gapY * 2;

    m_pRenderer->fillRect(x, y, w, h, col);
#else
    m_pRenderer->clearScreen(col / 4);
#endif
}


bool
SDL_ZXScreen::getSquareAt(ChessboardPoint* pChessboard, int x, int y) const {
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
