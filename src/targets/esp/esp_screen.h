#ifndef ESP_ZXSCREEN_H
#define ESP_ZXSCREEN_H 1


#include "emf/emf.h"
#include "targets/common/cpp/screen.h"


class ESP_Render : public BaseRender {
public:
    ESP_Render();

    virtual void fillRect(int x, int y, int w, int h, uint32_t col);
    virtual void clearScreen(uint32_t col);
};


// This class is only focused for playing chess, not running the whole BASIC interpreter
class ESP_ZXScreen : public CommonScreen {
public:
    ESP_ZXScreen(int scale, bool showComparison);

    void getOffset(ScreenPoint* pOffset, int position) const;
    bool getSquareAt(ChessboardPoint* pChessboard, int x, int y) const;

    void clearBackground();

protected:
    void  drawCharacterRects(ScreenPoint* pOffset, int screenX, int screenY, int ch) override;
    void  clearRestOfLine(ScreenPoint* pOffset, int screenX, int screenY) override;

};

#endif // ESP_ZXSCREEN_H
