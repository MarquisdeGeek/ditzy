#ifndef TARGETS_COMMON_SCREEN_H
#define TARGETS_COMMON_SCREEN_H 1

#include "emf/emf.h"


class BaseRender {
public:
    virtual void fillRect(int x, int y, int w, int h, uint32_t col) = 0;
    virtual void clearScreen(uint32_t col) = 0;
};


class ScreenPoint {
public:
    int x, y;
};


class ChessboardPoint {
public:
    int x, y;
};


class CommonScreen {
public:
    const uint32_t colLight = 0xddaa7700;
    const uint32_t colDark  = 0xaa2200ff;


    CommonScreen(int scale, bool showComparison);

    virtual void setRenderer(BaseRender* pRenderer);

    virtual int getWidth() const;
    virtual int getHeight() const;

    virtual void getOffset(ScreenPoint* pOffset, int position) const;
    virtual bool getSquareAt(ChessboardPoint* pChessboard, int x, int y) const;

    virtual void clearBackground(ScreenPoint* pOffset);
    virtual void drawC(ScreenPoint* pOffset);
    virtual void drawCPP(ScreenPoint* pOffset);

protected:
    BaseRender* m_pRenderer;

    int  m_Scale;
    bool m_bShowComparison;
    int  m_ifaceWidth; // size of total chess board and collateral
    int  m_ifaceHeight; // size of total chess board and collateral
    int  m_gapX; // space around each board
    int  m_gapY; // space around each board

    const int m_fontWidth = 8;
    const int m_fontHeight = 8;

    typedef uint8_t (*cbRead8)(uint16_t a);
    typedef uint16_t (*cbRead16)(uint16_t a);

    virtual void  drawScreen(ScreenPoint* pOffset, cbRead8 r8, cbRead16 r16);
    virtual void  drawCharacterRects(ScreenPoint* pOffset, int screenX, int screenY, int ch);
    virtual void  clearRestOfLine(ScreenPoint* pOffset, int screenX, int screenY);
};


#endif // TARGETS_COMMON_SCREEN_H
