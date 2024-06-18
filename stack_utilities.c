#include "stack_utilities.h"
#include <stdlib.h>

struct STACK * stack_init(int size) {
    if (size <= 0) {
        return NULL;
    }

    struct STACK * stack = 
            (struct STACK *) malloc(sizeof(struct STACK) + sizeof(int) * size);
    if ( !stack ) {
        SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to allocate memory for new stack.\n");
        return NULL;
    }

    stack->topIndex = 0;
    stack->size = size;

    return stack;
}

SDL_bool stack_push(struct STACK * stack, int data) {
    if ( !stack ) {
        return SDL_FALSE;
    }

    // Stack Full.
    if ( stack->topIndex >= stack->size ) {
        return SDL_FALSE;
    }
    
    // Stack Not Full.
    int top = stack->topIndex;
    stack->data[top] = data;
    stack->topIndex = top + 1;
    
    return SDL_TRUE;
}

SDL_bool stack_pop(struct STACK * stack, int * data) {
    if ( !stack ) {
        return SDL_FALSE;
    }

    // Stack Empty.
    if (stack->topIndex <= 0) {
        return SDL_FALSE;
    }

    // Stack Not Empty.
    int top = stack->topIndex;
    if ( data != NULL ) {
        *data = stack->data[top - 1];
    }
    stack->topIndex = top - 1;
    return SDL_TRUE;
}

SDL_bool stack_peek(struct STACK * stack, int * data) {
    if ( !stack ) {
        return SDL_FALSE;
    }

    if (stack->topIndex <= 0) {
        return SDL_FALSE;
    }

    int top = stack->topIndex;
    if ( !data ) {
        return SDL_FALSE;
    }
    *data = stack->data[top - 1];
    return SDL_TRUE;
}

// NULL-safe function.
void stack_destroy(struct STACK * stack) {
    if ( stack ) {
        free( stack );
        stack = NULL;
    }
}

SDL_bool stack_empty(struct STACK * stack) {
    if ( !stack ) {
        return SDL_FALSE;
    }

    if ( stack->topIndex != 0 ) {
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

SDL_bool stack_full(struct STACK * stack) {
    if ( !stack ) {
        return SDL_FALSE;
    }

    if ( stack->topIndex != stack->size ) {
        return SDL_FALSE;
    }

    return SDL_TRUE;
}
