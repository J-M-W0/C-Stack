#ifndef __JUNZHE_UTILITIES_H
#define __JUNZHE_UTILITIES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "stack_utilities.h"

enum STACK_ENUM {
    STACK_ENUM_INIT, // 0
    STACK_ENUM_PROC, // 1
    STACK_ENUM_PUSH, // 2 
    STACK_ENUM_PEEK, // 3
    STACK_ENUM_POP,  // 4
    STACK_ENUM_COUNT // 5
};

enum BUTTON_ENUM {
    BUTTON_ENUM_OUTSIDE,
    BUTTON_ENUM_INSIDE_PUSH,
    BUTTON_ENUM_INSIDE_PEEK,
    BUTTON_ENUM_INSIDE_POP,
    BUTTON_ENUM_INSIDE_REFRESH,
    BUTTON_ENUM_INSIDE_QUIT,
    BUTTON_ENUM_COUNT,
};

SDL_bool JUNZHE_init();
SDL_bool JUNZHE_loadFont( const char * fontname );
void JUNZHE_main();
void JUNZHE_close();

void JUNZHE_clearScreen();
void JUNZHE_writeAt(const char * text, int x, int y, SDL_Color color);
void JUNZHE_drawStackAt(struct STACK * stack, int x, int y);
void JUNZHE_drawButtonAt(const char * text, int x, int y, SDL_Color color);
void JUNZHE_drawButtonAt2(const char * text, int x, int y, int w, int h, SDL_Color color);

void JUNZHE_drawCircle(int centerX, int centerY, double radius, SDL_Color color);

enum BUTTON_ENUM JUNZHE_retrieveButtonPosition(int mouseX, int mouseY);

#endif // __JUNZHE_UTILITIES_H
