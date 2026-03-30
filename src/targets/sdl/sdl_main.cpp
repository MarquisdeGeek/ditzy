#include <SDL.h>
#include "emf/emf.h"

// common
#include "sdl_screen.h"
#include "sdl_update.h"

#include "targets/common/cpp/click_interface.h"

//cpp
#include "platforms/zx81/common/cpp/zxkeyboard.h"

// c
#include "platforms/zx81/common/c/zxckeyboard.h"


bool gUpdateScreen = true;
ClickInterface clicker;


// On timer
Uint32 update(Uint32 interval, void *param) {
    zxupdate();
    return interval;
}

// Keyboard mapping
int sdlKeyRemap(int keycode) {
    return keycode > 90 ? keycode - 32 : keycode;
}


// Timer to handle fake keypresses, from mouse
Uint32 fakeKeys(Uint32 interval, void *param) {
    if (clicker.pumpKeypresses()) {
        return interval;
    }
    return 0;
}


int main(int argc, char* argv[])
{
#if EMF_DUAL_EMULATION
    int scale = 4;
    bool comparisonVersion = true;
#else
    int scale = 4;
    bool comparisonVersion = false;
#endif

    SDL_ZXScreen zxscreen(scale, comparisonVersion);

    int width = zxscreen.getWidth();
    int height = zxscreen.getHeight();

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ditzy - Dragon 1K ZX Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_Surface* pixels = SDL_CreateRGBSurfaceWithFormat(0, width, height,32, SDL_PIXELFORMAT_RGBX8888);

    SDL_Render renderer(pixels);
#if CHESS_ONLY_OPTIMISATIONS
    renderer.clearScreen(zxscreen.colLight);
#else
    renderer.clearScreen(zxscreen.colLight / 4);
#endif
    zxscreen.setRenderer(&renderer);
    zxstart();


    int fps = 50;
    SDL_AddTimer(1000/fps, update, pixels);

    bool running = true;
    
    while(running)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
             switch (ev.type) {

                case SDL_QUIT:
                    running = false;
                    zxend();
                    return 0;

                case SDL_KEYDOWN:
                    if (!ev.key.repeat) {
                        onKeyDown(ev.key.keysym.sym);
                    }
                    break;

                case SDL_KEYUP:
                    onKeyUp(ev.key.keysym.sym);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if(ev.button.button == SDL_BUTTON_LEFT){
                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);

                        if (clicker.clickAt(zxscreen, mouseX, mouseY)) {
                            SDL_AddTimer(20, fakeKeys, nullptr);
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        // Switch between C and C++ versions...
        if (gUpdateScreen) {
            ScreenPoint pt;
            zxscreen.getOffset(&pt, 0);
#if EMF_SINGLE_EMULATOR_CPP==1
            zxscreen.drawCPP(&pt);
#else
            zxscreen.drawC(&pt);
#endif

#if EMF_DUAL_EMULATION
            zxscreen.getOffset(&pt, 1);
#if EMF_SINGLE_EMULATOR_CPP==1
            zxscreen.drawC(&pt);
#else
            zxscreen.drawCPP(&pt);
#endif
#endif
            //
            gUpdateScreen = false;
        }

        // copy to window
        SDL_BlitSurface(pixels, NULL, screen, NULL);
        SDL_UpdateWindowSurface(window);
    }

}


// Essentially global callbacks...
extern "C" {
void memory_trap8(uint16_t addr, uint8_t data) {
    gUpdateScreen = true;
}
}

void memory_trap8cpp(uint16_t addr, uint8_t data) {
    gUpdateScreen = true;
}


void onKeyDown(int keycode) {
    // cpp
    zxkeyboard_onDown(keycode);
    // c
    zxckeyboard_onDown(sdlKeyRemap(keycode));
}


void onKeyUp(int keycode) {
    // cpp
    zxkeyboard_onUp(keycode);
    // c
    zxckeyboard_onUp(sdlKeyRemap(keycode));
}
