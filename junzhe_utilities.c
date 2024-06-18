#include "junzhe_utilities.h"
#include <math.h>

const int SCREEN_WIDTH  = 720;
const int SCREEN_HEIGHT = 720;

const int STACK_WIDTH  = 100;
const int STACK_HEIGHT = 50;

const int FONTSIZE = 20;

const int BUTTON_WIDTH = 50;
const int BUTTON_HEIGHT = 25;

const int PUSH_BUTTON_X = 40;
const int PUSH_BUTTON_Y = SCREEN_HEIGHT - 50;
const int PEEK_BUTTON_X = 115;
const int PEEK_BUTTON_Y = SCREEN_HEIGHT - 50;
const int POP_BUTTON_X = 190;
const int POP_BUTTON_Y = SCREEN_HEIGHT - 50;
const int REFRESH_BUTTON_X = 265;
const int REFRESH_BUTTON_Y = SCREEN_HEIGHT - 50;
const int QUIT_BUTTON_X = 370;
const int QUIT_BUTTON_Y = SCREEN_HEIGHT - 50;

const SDL_Color JUNZHE_black = { 0, 0, 0, 0xff };
const SDL_Color JUNZHE_white = { 0xff, 0xff, 0xff, 0xff };
const SDL_Color JUNZHE_lightgrey = { 200, 200, 200, 0xff };
const SDL_Color JUNZHE_darkgrey  = { 50, 50, 50, 0xff };

SDL_Window   * gWindow   = NULL;
SDL_Renderer * gRenderer = NULL;
TTF_Font     * gFont     = NULL;

SDL_bool JUNZHE_init() {
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR,
            "Failed at SDL_Init(): %s\n", SDL_GetError()
        );
        return SDL_FALSE;
    }
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "VIDEO subsystem initialized.\n"
    );
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "AUDIO subsystem initialized.\n"
    );
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "TIMER subsystem initialized.\n"
    );

    if ( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") ) {
        SDL_LogWarn(
            SDL_LOG_CATEGORY_APPLICATION,
            "Failed at SDL_SetHint(): %s\n", SDL_GetError()
        );
    }

    gWindow = SDL_CreateWindow(
        "Junzhe STACK",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if ( !gWindow ) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR,
            "Failed at SDL_CreateWindow(): %s\n", SDL_GetError()
        );
        SDL_Quit();
        return SDL_FALSE;
    }
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "WINDOW initialized.\n"
    );

    gRenderer = SDL_CreateRenderer(
        gWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if ( !gRenderer ) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR,
            "Failed at SDL_CreateRenderer(): %s\n", SDL_GetError()
        );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        SDL_Quit();
        return SDL_FALSE;
    }
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "RENDER initialized.\n"
    );

    SDL_SetRenderDrawColor(
        gRenderer,
        0xff, 0xff, 0xff, 0xff
    );
    SDL_RenderClear( gRenderer );

    int imgFlags = IMG_INIT_PNG;
    int initFlags = IMG_Init( imgFlags );
    if ( imgFlags != (initFlags & imgFlags) ) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR,
            "Failed at IMG_Init(): %s\n", IMG_GetError()
        );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        SDL_DestroyRenderer( gRenderer );
        gRenderer = NULL;
        SDL_Quit();
        return SDL_FALSE;
    }
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "IMG subsytem initialized.\n"
    );

    if ( TTF_Init() == -1 ) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR,
            "Failed at TTF_Init(): %s\n", TTF_GetError()
        );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        SDL_DestroyRenderer( gRenderer );
        gRenderer = NULL;
        IMG_Quit();
        SDL_Quit();
        return SDL_FALSE;
    }
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "TTF subsytem initialized.\n"
    );

    if ( !JUNZHE_loadFont( "UbuntuMono-Regular.ttf" ) ) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR,
            "Failed at load_font()\n"
        );
        SDL_DestroyWindow( gWindow );
        gWindow = NULL;
        SDL_DestroyRenderer( gRenderer );
        gRenderer = NULL;
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return SDL_FALSE;
    }
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "OpenSans-Regular.ttf font loaded.\n"
    );
    

    SDL_StartTextInput();
    return SDL_TRUE;
}

SDL_bool JUNZHE_loadFont( const char * fontname ) {
    gFont = TTF_OpenFont( fontname, FONTSIZE );
    if ( !gFont ) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR,
            "Failed at TTF_OpenFont() :%s\n", TTF_GetError()
        );
        return SDL_FALSE;
    }
    return SDL_TRUE;
}

void JUNZHE_main() {
    enum STACK_ENUM status = STACK_ENUM_INIT;
    struct STACK * stack = NULL;
    int topData;
    SDL_bool success;
    
    SDL_Color pushButtonColor = JUNZHE_lightgrey;
    SDL_Color peekButtonColor = JUNZHE_lightgrey;
    SDL_Color popButtonColor  = JUNZHE_lightgrey;
    SDL_Color refreshButtonColor = JUNZHE_lightgrey;
    SDL_Color quitButtonColor = JUNZHE_lightgrey;
    
    char str[255];
    bzero(str, sizeof(str));

    SDL_bool done = SDL_FALSE;
    SDL_Event e;
    do {
        SDL_PumpEvents();
        while ( SDL_PollEvent(&e) ) 
        {
            if (e.type == SDL_QUIT) {
                done = SDL_TRUE;
                break;
            }
            else if (e.type == SDL_TEXTINPUT) {
                // only 'e.text.text[0]' valid.
                if (str[253] == '\0') {
                    strncat(str, e.text.text, 1);
                }
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN) {
                    switch (status) 
                    {
                        case STACK_ENUM_INIT:
                        {
                            char * endptr;
                            int size = (int) strtol(str, &endptr, 10);
                            if (endptr == str || *endptr != '\0' || size <= 0) {
                            } else {
                                stack = stack_init(size);
                                status = STACK_ENUM_PROC;
                            }
                        } break;

                        case STACK_ENUM_PUSH:
                        {
                            char * endptr;
                            int data = (int) strtol(str, &endptr, 10);
                            if (endptr == str || *endptr != '\0') {
                            } else {
                                stack_push(stack, data);
                                status = STACK_ENUM_PROC;
                            }
                        } break;

                        case STACK_ENUM_PEEK: 
                        case STACK_ENUM_POP:
                        {
                            status = STACK_ENUM_PROC;
                        } break;

                        default:
                        {} break;
                    }
                    bzero(str, sizeof(str));
                } else if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    int len = strlen(str);
                    if (len > 0) {
                        str[len - 1] = '\0';
                    }
                }
            }
            else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
                int mouseX;
                int mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                enum BUTTON_ENUM bottomStatus = JUNZHE_retrieveButtonPosition(mouseX, mouseY);
                switch (bottomStatus) {
                    case BUTTON_ENUM_INSIDE_PUSH:
                    {
                        pushButtonColor = JUNZHE_darkgrey;
                        peekButtonColor = JUNZHE_lightgrey;
                        popButtonColor  = JUNZHE_lightgrey;
                        refreshButtonColor = JUNZHE_lightgrey;
                        quitButtonColor = JUNZHE_lightgrey;
                        if (e.type == SDL_MOUSEBUTTONUP && status != STACK_ENUM_INIT) {
                            status = STACK_ENUM_PUSH;
                        }
                    } break;

                    case BUTTON_ENUM_INSIDE_PEEK:
                    {
                        pushButtonColor = JUNZHE_lightgrey;
                        peekButtonColor = JUNZHE_darkgrey;
                        popButtonColor  = JUNZHE_lightgrey;
                        refreshButtonColor = JUNZHE_lightgrey;
                        quitButtonColor = JUNZHE_lightgrey;
                        if (e.type == SDL_MOUSEBUTTONUP && status != STACK_ENUM_INIT) {
                            success = stack_peek(stack, &topData);
                            status = STACK_ENUM_PEEK;
                        }
                    } break;
                    
                    case BUTTON_ENUM_INSIDE_POP:
                    {
                        pushButtonColor = JUNZHE_lightgrey;
                        peekButtonColor = JUNZHE_lightgrey;
                        popButtonColor  = JUNZHE_darkgrey;
                        refreshButtonColor = JUNZHE_lightgrey;
                        quitButtonColor = JUNZHE_lightgrey;
                        if (e.type == SDL_MOUSEBUTTONUP && status != STACK_ENUM_INIT) {
                            success = stack_pop(stack, &topData);
                            status = STACK_ENUM_POP;
                        }
                    } break;

                    case BUTTON_ENUM_INSIDE_REFRESH:
                    {
                        pushButtonColor = JUNZHE_lightgrey;
                        peekButtonColor = JUNZHE_lightgrey;
                        popButtonColor  = JUNZHE_lightgrey;
                        refreshButtonColor = JUNZHE_darkgrey;
                        quitButtonColor = JUNZHE_lightgrey;
                        if (e.type == SDL_MOUSEBUTTONUP && status != STACK_ENUM_INIT) {
                            bzero(str, sizeof(str));
                            status = STACK_ENUM_PROC;
                        }
                    } break;

                    case BUTTON_ENUM_INSIDE_QUIT:
                    {
                        pushButtonColor = JUNZHE_lightgrey;
                        peekButtonColor = JUNZHE_lightgrey;
                        popButtonColor  = JUNZHE_lightgrey;
                        refreshButtonColor = JUNZHE_lightgrey;
                        quitButtonColor = JUNZHE_darkgrey;
                        if (e.type == SDL_MOUSEBUTTONUP) {
                            done = SDL_TRUE;
                        }
                    } break;

                    default: 
                    {
                        pushButtonColor = JUNZHE_lightgrey;
                        peekButtonColor = JUNZHE_lightgrey;
                        popButtonColor  = JUNZHE_lightgrey;
                        refreshButtonColor = JUNZHE_lightgrey;
                        quitButtonColor = JUNZHE_lightgrey;
                    } break;
                }
            }
        }
        
        // Clear the screen.
        JUNZHE_clearScreen();

        switch (status) 
        {
            case STACK_ENUM_INIT:
            {
                char init[] = "Please Enter the Size of the Stack (a postive number):";
                JUNZHE_writeAt(init, 20, 20, JUNZHE_black);
            } break;

            case STACK_ENUM_PROC:
            {
                char proc[] = "Please Click on One of the Three Buttons Blow.";
                JUNZHE_writeAt(proc, 20, 20, JUNZHE_black);
            } break;

            case STACK_ENUM_PUSH:
            {
                char push[] = "Please Enter the Number You Wish to Push onto the Stack:";
                JUNZHE_writeAt(push, 20, 20, JUNZHE_black);
            } break;

            case STACK_ENUM_PEEK:
            {
                char peek[] = "The Number on the Top of the Stack is:";
                JUNZHE_writeAt(peek, 20, 20, JUNZHE_black);
                if (success) {
                    char text[20];
                    snprintf(text, sizeof(text), "%d", topData);
                    JUNZHE_writeAt(text, 20, 50, JUNZHE_black);
                } else {
                    char errmsg[] = "Failed to Peek!";
                    JUNZHE_writeAt(errmsg, 20, 50, JUNZHE_black);
                }
            } break;

            case STACK_ENUM_POP:
            {
                char pop[] = "The Number Popped out of the Stack is:";
                JUNZHE_writeAt(pop, 20, 20, JUNZHE_black);
                if (success) {
                    char text[20];
                    snprintf(text, sizeof(text), "%d", topData);
                    JUNZHE_writeAt(text, 20, 50, JUNZHE_black);
                } else {
                    char errmsg[] = "Failed to Pop!";
                    JUNZHE_writeAt(errmsg, 20, 50, JUNZHE_black);
                }
            } break;

            default:
            {} break;
        }
       
        JUNZHE_writeAt(str, 50, 50, JUNZHE_black);

        JUNZHE_drawButtonAt("push", PUSH_BUTTON_X, PUSH_BUTTON_Y, pushButtonColor);
        JUNZHE_drawButtonAt("peek", PEEK_BUTTON_X, PEEK_BUTTON_Y, peekButtonColor);
        JUNZHE_drawButtonAt("pop", POP_BUTTON_X, POP_BUTTON_Y, popButtonColor);
        JUNZHE_drawButtonAt2("refresh", REFRESH_BUTTON_X, REFRESH_BUTTON_Y, BUTTON_WIDTH + 30, BUTTON_HEIGHT, refreshButtonColor);
        JUNZHE_drawButtonAt("quit", QUIT_BUTTON_X, QUIT_BUTTON_Y, quitButtonColor);

        JUNZHE_drawStackAt(
            stack, 
            SCREEN_WIDTH - STACK_WIDTH - 100, SCREEN_HEIGHT - STACK_HEIGHT - 50
        );

        SDL_RenderPresent( gRenderer );
    } while ( !done );

    stack_destroy(stack);
    stack = NULL;
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "STACK destroyed!!\n"
    );
}

void JUNZHE_close() {

    SDL_StopTextInput();
    
    TTF_CloseFont( gFont );
    gFont = NULL;
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "OpenSans-Regular.ttf font closed.\n"
    );
    
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "RENDER destroyed.\n"
    );
    
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "WINDOW destroyed.\n"
    );
    
    TTF_Quit();
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "TTF subsytem quited.\n"
    );
    
    IMG_Quit();
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "IMG subsytem quited.\n"
    );
    
    SDL_Quit();
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "TIMER subsytem quitted.\n"
    );
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "AUDIO subsytem quitted.\n"
    );
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "VIDEO subsytem quitted.\n"
    );
}

void JUNZHE_drawCircle(int centerX, int centerY, double radius, SDL_Color color) {
    SDL_SetRenderDrawColor(
        gRenderer, 
        color.r, color.g, color.b, color.a 
    );
    
    int x;
    int y;
    for (int i = 0; i < 360; i++) {
        x = centerX + radius * cos(i * M_PI / 180);
        y = centerY + radius * sin(i * M_PI / 180);
        SDL_RenderDrawPoint(gRenderer, x, y);
    }
}

void JUNZHE_writeAt(const char * text, int x, int y, SDL_Color color) {
    if ( text[0] == '\0' || text == NULL ) {
        return;
    }
    SDL_Surface * textSurface = TTF_RenderText_Solid(
        gFont,
        text,
        color
    );
    if ( !textSurface ) {
        fprintf( stderr, "Error: Unable to write [%s]\n", text );
        return;
    }

    SDL_Texture * textTexture = SDL_CreateTextureFromSurface(
        gRenderer,
        textSurface
    );
    if ( !textTexture ) {
        SDL_FreeSurface( textSurface );
        fprintf( stderr, "Error: Unable to write [%s]\n", text );
        return;
    }

    SDL_FreeSurface( textSurface );
    textSurface = NULL;

    SDL_Rect textRect = {
        x, y,
        0, 0
    };
    SDL_QueryTexture( 
        textTexture, 
        NULL, 
        NULL, 
        &textRect.w,
        &textRect.h
    );

    SDL_RenderCopy(
        gRenderer,
        textTexture,
        NULL,
        &textRect
    );

    SDL_DestroyTexture( textTexture );
    textTexture = NULL;
}

static void JUNZHE_drawRectAt(int x, int y, int w, int h) {
    SDL_Rect rect = {
        x, y,
        w, h
    };
    
    SDL_SetRenderDrawColor(
        gRenderer,
        0, 0, 0, 0xff
    );

    SDL_RenderDrawRect(
        gRenderer,
        &rect
    );
}

void JUNZHE_drawStackAt(struct STACK * stack, int x, int y) {
    if ( !stack ) {
        return;
    }

    for (int i = 0; i < stack->size; i++) {
        int stackX = x;
        int stackY = y - i * 50;
        JUNZHE_drawRectAt(stackX, stackY, STACK_WIDTH, STACK_HEIGHT);
        
        if (i >= stack->topIndex) {
            continue;
        }
        
        char text[20];
        int data = stack->data[i];
        snprintf(text, sizeof(text), "%d", data);

        SDL_Color color = {
            0, 0, 0, 0xff
        };
        JUNZHE_writeAt(text, stackX + 20, stackY + 15, color);
    }
}

void JUNZHE_drawButtonAt(const char * text, int x, int y, SDL_Color color) {
    SDL_Rect rect = {
        x, y,
        BUTTON_WIDTH, BUTTON_HEIGHT
    };
    SDL_SetRenderDrawColor(
        gRenderer,
        color.r, 
        color.g, 
        color.b, 
        color.a
    );
    SDL_RenderFillRect(
        gRenderer,
        &rect
    );

    SDL_Color black = {
        0, 0, 0, 0xff
    };
    JUNZHE_writeAt(text, x + 2, y, black);
}

void JUNZHE_drawButtonAt2(const char * text, int x, int y, int w, int h, SDL_Color color) {
    SDL_Rect rect = {
        x, y,
        w, h
    };
    SDL_SetRenderDrawColor(
        gRenderer,
        color.r, 
        color.g, 
        color.b, 
        color.a
    );
    SDL_RenderFillRect(
        gRenderer,
        &rect
    );

    SDL_Color black = {
        0, 0, 0, 0xff
    };
    JUNZHE_writeAt(text, x + 2, y, black);
}

void JUNZHE_clearScreen() {
    SDL_SetRenderDrawColor(
        gRenderer,
        0xff, 0xff, 0xff, 0xff
    );
    SDL_RenderClear( gRenderer );
}

enum BUTTON_ENUM JUNZHE_retrieveButtonPosition(int mouseX, int mouseY) {
    enum BUTTON_ENUM status = BUTTON_ENUM_OUTSIDE;

    if (mouseX > PUSH_BUTTON_X && mouseX < PUSH_BUTTON_X + BUTTON_WIDTH) {
        if (mouseY > PUSH_BUTTON_Y && mouseY < PUSH_BUTTON_Y + BUTTON_HEIGHT) {
            status = BUTTON_ENUM_INSIDE_PUSH;
            goto end_;
        }
    }
    if (mouseX > PEEK_BUTTON_X && mouseX < PEEK_BUTTON_X + BUTTON_WIDTH) {
        if (mouseY > PEEK_BUTTON_Y && mouseY < PEEK_BUTTON_Y + BUTTON_HEIGHT) {
            status = BUTTON_ENUM_INSIDE_PEEK;
            goto end_;
        }
    }
    if (mouseX > POP_BUTTON_X && mouseX < POP_BUTTON_X + BUTTON_WIDTH) {
        if (mouseY > POP_BUTTON_Y && mouseY < POP_BUTTON_Y + BUTTON_HEIGHT) {
            status = BUTTON_ENUM_INSIDE_POP;
            goto end_;
        }
    }
    if (mouseX > REFRESH_BUTTON_X && mouseX < REFRESH_BUTTON_X + BUTTON_WIDTH) {
        if (mouseY > REFRESH_BUTTON_Y && mouseY < REFRESH_BUTTON_Y + BUTTON_HEIGHT) {
            status = BUTTON_ENUM_INSIDE_REFRESH;
            goto end_;
        }
    }
    if (mouseX > QUIT_BUTTON_X && mouseX < QUIT_BUTTON_X + BUTTON_WIDTH) {
        if (mouseY > QUIT_BUTTON_Y && mouseY < QUIT_BUTTON_Y + BUTTON_HEIGHT) {
            status = BUTTON_ENUM_INSIDE_QUIT;
            goto end_;
        }
    }

end_:
    return status;
}
