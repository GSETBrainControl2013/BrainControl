#include "MorseTranslator.hpp"
#include "SDL/SDL.h"
#include <iostream>
#include <stdexcept>

const float FPS = 30;
const Uint32 MS_PER_FRAME = 1000/FPS;

struct screenHandler {
    SDL_Surface* screen;
    screenHandler() : screen(NULL) {}
    ~screenHandler() {
        SDL_Quit();
    }
};

struct surfaceHandler {
    SDL_Surface* surface;
    surfaceHandler(): surface(NULL) {}
    ~surfaceHandler() {
        SDL_FreeSurface(surface);
    }
};

int main(int argc,char* argv[]) {
    screenHandler screen;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw std::runtime_error("SDL init failed");
    }
    if(!(screen.screen = SDL_SetVideoMode(400,300,32,SDL_SWSURFACE|SDL_RESIZABLE))) {
        throw std::runtime_error("SDL surface init failed");
    }
    SDL_WM_SetCaption("SDL Test",NULL);

    bool running = true;
    while(running) {
        Uint32 frameBegin = SDL_GetTicks();
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_FillRect(screen.screen,NULL,SDL_MapRGB(screen.screen->format,255,255,255));
        SDL_Flip(screen.screen);
        Uint32 frameEnd = SDL_GetTicks();
        if(frameEnd-frameBegin < MS_PER_FRAME) {
            SDL_Delay(MS_PER_FRAME-(SDL_GetTicks()-frameBegin));
        }
    }

#if 0
    MorseTranslator translator;
    std::string str = ".. / -.-. .- -. .----. - / .-.. . - / -.-- --- ..- / -.. --- / - .... .- - --..-- / .--- .. -- .-.-.- ";
    for(size_t i=0;i<str.length();++i) {
        std::string morse = str.substr(i,1);
        translator.add(morse);
        std::cout << translator.text() << "|" << translator.morseLetter() << std::endl;
    }
    std::cin.get();
#endif
    return 0;
}
