#ifndef __JUNZHE_STACK_UTILITIES_H
#define __JUNZHE_STACK_UTILITIES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

struct STACK {
    int topIndex;
    int size;
    int data[];
};


struct STACK * stack_init(int size);
SDL_bool stack_push(struct STACK * stack, int data);
SDL_bool stack_pop(struct STACK * stack, int * data);
SDL_bool stack_peek(struct STACK * stack, int * data);
SDL_bool stack_empty(struct STACK * stack);
SDL_bool stack_full(struct STACK * stack);
void stack_destroy(struct STACK * stack);


#endif // __JUNZHE_STACK_UTILITIES_H
