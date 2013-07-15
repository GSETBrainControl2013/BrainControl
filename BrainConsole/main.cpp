#include "MorseTranslator.hpp"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
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

struct ttfQuitter {
    ~ttfQuitter() {
        TTF_Quit();
    };
    void foo() {}
};

struct surfaceHandler {
    SDL_Surface* surface;
    surfaceHandler(): surface(NULL) {}
    ~surfaceHandler() {
        if(surface) {
            SDL_FreeSurface(surface);
        }
    }
};

struct fontHandler {
    TTF_Font* font;
    fontHandler() : font(NULL) {}
    ~fontHandler() {
        TTF_CloseFont(font);
    }
};

void drawText(SDL_Surface* surf,TTF_Font* font,int x,int y,std::string s,SDL_Color txtColor) {
    SDL_Surface* txt = TTF_RenderText_Blended(font,s.c_str(),txtColor);
    SDL_Rect loc = { x,y,0,0 };
    SDL_BlitSurface(txt,NULL,surf,&loc);
    SDL_FreeSurface(txt);
}

void drawTextLines(SDL_Surface* surf,TTF_Font* font,int x,int y,std::string s,SDL_Color txtColor,int* xOut = NULL,int* yOut = NULL) {
    std::string line;
    for(size_t i=0;i<s.length();++i) {
        line += s[i];
        int textWidth,textHeight;
        TTF_SizeText(font,line.c_str(),&textWidth,&textHeight);
        if(textWidth > surf->w-x) {
            drawText(surf,font,x,y,line.substr(0,line.length()-1),txtColor);
            y += textHeight;
            line.erase(0,line.length()-1);
            x = 0;
        }
    }
    drawText(surf,font,x,y,line,txtColor);
    TTF_SizeText(font,line.c_str(),xOut,NULL);
    if(xOut) {
        *xOut += x;
    }
    if(yOut) {
        *yOut = y;
    }
}

int main(int argc,char* argv[]) {
    MorseTranslator morse;
    SDL_Color txtColor   = { 0,  0,  0 },
              morseColor = { 255,0,  0 };
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw std::runtime_error("SDL init failed");
    }
    screenHandler screen;
    if(TTF_Init() < 0) {
        throw std::runtime_error("TTF init failed");
    }
    ttfQuitter _quitTTF;
    _quitTTF.foo();
    if(!(screen.screen = SDL_SetVideoMode(400,300,32,SDL_SWSURFACE|SDL_RESIZABLE))) {
        throw std::runtime_error("SDL surface init failed");
    }
    SDL_EnableUNICODE(1);

    fontHandler font;
    font.font = TTF_OpenFont("brain.ttf",20);
    SDL_WM_SetCaption("SDL Test",NULL);
    std::string txt = "";
    int txtTick = 0;

    bool running = true;
    while(running) {
        Uint32 frameBegin = SDL_GetTicks();
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_VIDEORESIZE:
                screen.screen = SDL_SetVideoMode(e.resize.w,e.resize.h, 32, SDL_SWSURFACE | SDL_RESIZABLE );
                break;
            case SDL_KEYDOWN: {
                    std::string s;
                    std::cout << s << std::endl;
                    std::cout.flush();
                    s += (char)e.key.keysym.unicode;
                    morse.add(s);
                }
                break;
            default:
                break;
            }
        }

        SDL_FillRect(screen.screen,NULL,SDL_MapRGB(screen.screen->format,255,255,255));

        std::cout << morse.text() << "|" << morse.morseLetter() << std::endl;

        int x,y;
        drawTextLines(screen.screen,font.font,0,0,morse.text(),txtColor,&x,&y);
        std::string morseTxt = morse.morseLetter();
        if(morseTxt.empty()) {
            morseTxt = "_";
        }
        drawTextLines(screen.screen,font.font,x,y,morseTxt,morseColor);

        SDL_Flip(screen.screen);
        Uint32 frameEnd = SDL_GetTicks();
        Uint32 frameTime = frameEnd-frameBegin;
        //std::cout << frameTime << "ms/frame" << std::endl;
        //std::cout.flush();
        if(frameEnd-frameBegin < MS_PER_FRAME) {
            SDL_Delay(MS_PER_FRAME-(SDL_GetTicks()-frameBegin));
        }
    }
    return 0;
}
