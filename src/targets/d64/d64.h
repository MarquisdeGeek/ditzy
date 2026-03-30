#ifndef D64_H
#define D64_H 1


#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 192


typedef short tScreenWidth;
typedef short tScreenHeight;


#ifdef __cplusplus
extern "C" {
#endif


unsigned char waitForKey64();
unsigned char getKey64();

void poke(unsigned short addr, unsigned char data);
void poke16(unsigned short addr, unsigned short data);
unsigned char peek(short addr);

void setTextMode(/*TODO, mode select*/);
void setPMode(/*TODO, mode select*/);
void clearScreenGraphics();
void clearScreenText();
void blit(const unsigned char* pScreen);
void initGraphicsMode(void);
void plot(tScreenWidth x, tScreenHeight y);
void unplot(tScreenWidth x, tScreenHeight y);

// THese are expected to be in multiples of 8, making the screen 0-31, 0-23
void xorBlock(tScreenWidth x8, tScreenHeight y8, int size);

// w===8
void xorUDG(tScreenWidth x8, tScreenHeight y8, int w, int h, unsigned char*pData);

#endif // D64_H
