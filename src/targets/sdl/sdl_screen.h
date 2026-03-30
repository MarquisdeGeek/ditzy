#ifndef SDL_ZXSCREEN_H
#define SDL_ZXSCREEN_H 1

#include <SDL.h>

#include "emf/emf.h"
#include "targets/common/cpp/screen.h"


class SDL_Render : public BaseRender {
public:
    SDL_Render(SDL_Surface* pSurface);

    virtual void fillRect(int x, int y, int w, int h, uint32_t col);
    virtual void clearScreen(uint32_t col);

protected:
    SDL_Surface* m_pSurface;
};



class SDL_ZXScreen : public CommonScreen {
public:
    SDL_ZXScreen(int scale, bool showComparison);

    void getOffset(ScreenPoint* pOffset, int position) const override;
    bool getSquareAt(ChessboardPoint* pChessboard, int x, int y) const override;

    void clearBackground(ScreenPoint* pOffset) override;
};


#endif // SDL_ZXSCREEN_H
