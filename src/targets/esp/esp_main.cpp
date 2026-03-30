/*  Rui Santos & Sara Santos - Random Nerd Tutorials
    THIS EXAMPLE WAS TESTED WITH THE FOLLOWING HARDWARE:
    1) ESP32-2432S028R 2.8 inch 240×320 also known as the Cheap Yellow Display (CYD): https://makeradvisor.com/tools/cyd-cheap-yellow-display-esp32-2432s028r/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/cyd/
    2) REGULAR ESP32 Dev Board + 2.8 inch 240x320 TFT Display: https://makeradvisor.com/tools/2-8-inch-ili9341-tft-240x320/ and https://makeradvisor.com/tools/esp32-dev-board-wi-fi-bluetooth/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/esp32-tft/
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <SPI.h>

/*  Install the "TFT_eSPI" library by Bodmer to interface with the TFT Display - https://github.com/Bodmer/TFT_eSPI
    *** IMPORTANT: User_Setup.h available on the internet will probably NOT work with the examples available at Random Nerd Tutorials ***
    *** YOU MUST USE THE User_Setup.h FILE PROVIDED IN THE LINK BELOW IN ORDER TO USE THE EXAMPLES FROM RANDOM NERD TUTORIALS ***
    FULL INSTRUCTIONS AVAILABLE ON HOW CONFIGURE THE LIBRARY: https://RandomNerdTutorials.com/cyd/ or https://RandomNerdTutorials.com/esp32-tft/   */
#include <TFT_eSPI.h>

// Install the "XPT2046_Touchscreen" library by Paul Stoffregen to use the Touchscreen - https://github.com/PaulStoffregen/XPT2046_Touchscreen
// Note: this library doesn't require further configuration
#include <XPT2046_Touchscreen.h>

#include "targets/esp/esp_screen.h"
#include "targets/esp/esp_update.h"
#include "targets/common/cpp/click_interface.h"
#include "platforms/zx81/common/cpp/zxkeyboard.h"


TFT_eSPI tft = TFT_eSPI();

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// ZX 1K Chess
const int scale = 3;
const int speedMultiplier = 10;
ESP_ZXScreen zxscreen(scale, false);
ClickInterface clicker;
ScreenPoint ptOffset;
//
int counter = 0;
bool gUpdateScreen = true;
//
const int fakeKeyCountdownStart = 20;
int fakeKeyCountdown = 0;
//

void setup() {
  Serial.begin(115200);

  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 3: touchscreen.setRotation(3);
  touchscreen.setRotation(0);

  tft.init();
  tft.setRotation(0);

  // ZX starts
  ESP_Render renderer;
  renderer.clearScreen(zxscreen.colLight / 4);
  zxscreen.setRenderer(&renderer);
  zxscreen.getOffset(&ptOffset, 0);
  //
  zxstart();
}


void loop() {

  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    // Get Touchscreen points
    TS_Point p = touchscreen.getPoint();

    // Calibrate Touchscreen points with map function to the correct width and height
    int x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    int y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);

    if (clicker.clickAt(zxscreen, x, y)) {
      // Trick the fake key routine to run immediately
      // (without duplicating the fakeKeysPump(); and =0 code)
      fakeKeyCountdown = fakeKeyCountdownStart;
    }
  }

  // We run the update loop more often that the renderer
  int speedCount = speedMultiplier;
  while(speedCount--) {
    zxupdate();

    if (++fakeKeyCountdown > fakeKeyCountdownStart) {
      clicker.pumpKeypresses();
      fakeKeyCountdown = 0;
    }
  }

  // Draw, if any changes to DFILE memory
  if (gUpdateScreen) {
    zxscreen.drawCPP(&ptOffset);
    gUpdateScreen = false;
  }

}


// Essentially global callbacks...
void memory_trap8cpp(uint16_t addr, uint8_t data) {
    gUpdateScreen = true;
}


void onKeyDown(int keycode) {
    zxkeyboard_onDown(keycode);
}

void onKeyUp(int keycode) {
    zxkeyboard_onUp(keycode);
}

