# Ditzy Chess
A program to emulate a ZX81 on a Dragon 64!

It handles just enough to run a game of 1K Chess, by David Horne. (Details on https://en.wikipedia.org/wiki/1K_ZX_Chess)


# Building it

```
cd src
cmake .
cmake --build .
```


# Running it

* Load xroar
* Switch into Dragon 64 mode (Menu > Hardware > Machine > Dragon 64)
* Load and run the zx1kmain.bin (Menu > File > Run)
* Switch in fast mode, for a more pleasant experience (Shift+F12)
* Enter the movements in reverse order, so E7-E5 should be typed as '7', 'E', '5', and 'E'. 
* Note: If a key doesn't register, re-press and release the previously key _slowly_
* Press the space bar to toggle between text and graphic modes


# Known issues

* Mate doesn't get reported (need to check with original code)


# Structure

* targets : platforms for which we can build the premixed emulator - d32 or SDL
* emf : EMF support files and code
* emulator : emulation engines for individual components, like Z80
* platforms : the machine-specific code for the platform to be emulated, e.g. zx81_1k


# Development

There is also an SDL version to compare the original C++ version with the minimized Dragon 64 CMOC-compiled version. Use this to find bugs, and expand on the original. (But note the CMOC-specific
issues http://gvlsywt.cluster051.hosting.ovh.net/dev/cmoc-manual.html#t8)


# Behind the scenes

* The Dragon text screen is 16 characters high, the ZX81 has 22+2
* (At least the Dragon can display a full 32x24 in PMODE 4 with 256x192 pixels)
* ZX81 uses a compressed display
* There seem to be more 118's at the start of the display, than when viewed in other emulators
* MUL168 from CMOC's stdlib gets used occassionally, but it would be nice to avoid it since it bulks the binary
* EMF_GRAPHICS_WITH_DIRTY_GRID doesn't work, yet
* Crashes when either side takes a piece - unless case "128:ADD A,B" is included. That's it - just one instruction needed for handling piece taking, over what was already there


