#include "junzhe_utilities.h"
#include "stack_utilities.h"

int main(int argc, char * argv[]) {
    if ( !JUNZHE_init() ) {
        SDL_LogError(
            SDL_LOG_CATEGORY_ERROR,
            "Failed at init()\n"
        );
        return -1;
    }
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "Initialization finished!!\n"
    );

    JUNZHE_main();

    JUNZHE_close();
    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "Program finished!!\n"
    );
    return 0;
}
