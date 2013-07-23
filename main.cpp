#include "EmoStateDLL.h"
#include "edk.h"
#include <iostream>
#include "EmoEngine.hpp"
#include "Expression.hpp"
#include "ExpressionProcessor.hpp"
#include "NodDetector.hpp"
#include "Exp2Morse.hpp"
#include "MorseTranslator.hpp"
#include "MorseTranslator.hpp"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <stdexcept>

#define DEBUG

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

SDL_Surface* buildWindow(TTF_Font* font,const std::vector<std::string>& lines,
                         int selectedLine,SDL_Color fg,SDL_Color bg,SDL_Color highlight) {
    if(lines.empty()) {
        return NULL;
    }
    int maxW = 0,totalH = 0;
    std::vector<int> yPos,height;
    int w,h;
    for(size_t i=0;i<lines.size();++i) {
        TTF_SizeText(font,lines[i].c_str(),&w,&h);
        if(w > maxW) {
            maxW = w;
        }
        yPos.push_back(totalH);
        height.push_back(h);
        totalH += h;
    }
    SDL_Surface* firstLine = TTF_RenderText_Blended(font,lines[0].c_str(),fg);
    if(!firstLine) {
        return NULL;
    }
    SDL_Surface* ret = SDL_CreateRGBSurface(SDL_SWSURFACE, maxW, totalH, firstLine->format->BitsPerPixel,
                                   firstLine->format->Rmask,
                                   firstLine->format->Gmask,
                                   firstLine->format->Bmask,
                                   firstLine->format->Amask);
    SDL_FillRect(ret,NULL,SDL_MapRGB(ret->format,bg.r,bg.g,bg.b));
    SDL_Surface* line = firstLine;
    for(size_t i=0;i<lines.size();++i) {
        SDL_Rect loc = { 0,yPos[i],ret->w,height[i] };
        if(i != 0) {
            SDL_FreeSurface(line);
            line = TTF_RenderText_Blended(font,lines[i].c_str(),fg);
        }
        if(i == selectedLine) {
            SDL_FillRect(ret,&loc,SDL_MapRGB(ret->format,highlight.r,highlight.g,highlight.b));
        }
        SDL_BlitSurface(line,NULL,ret,&loc);
    }
    SDL_FreeSurface(line);
    return ret;
}

void wordSuggestions(std::vector<std::string>& suggs) {
    suggs.clear();
}

int main(int argc,char* argv[]) {
	EmoEngine engine;
	ExpressionProcessor processor;
    MorseTranslator morse;

    SDL_Color txtColor   = {   0,  0,  0 },
              morseColor = { 255,  0,  0 },
              bgColor    = {   0,255,  0 },
              suggColor  = {   0,  0,255 };
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
    SDL_WM_SetCaption("T3: Thought To Text",NULL);
    std::string txt = "";

    std::ofstream eventLog("logs/events.csv",std::ios_base::out|std::ios_base::trunc);

    bool locked = false;

    //morse.add(".--- --- . ");

    std::string morseToAdd = "";

    Uint32 prevFrameBegin = 0;
    bool running = true;
    while(running) {
        processor.processEvents(engine);

        Expression e;
        while(processor.getExpression(e)) {
            eventLog << e.time << "," << e.event << "," << e.power << ","
                     << !!(e.eyeState & Expression::BLINK) << ","
                     << !!(e.eyeState & Expression::LWINK) << ","
                     << !!(e.eyeState & Expression::RWINK) << ","
                     << !!(e.eyeState & Expression::RLOOK) << ","
                     << !!(e.eyeState & Expression::LLOOK) << std::endl;
            morseToAdd += exp2Morse(e);
        }

        Uint32 frameBegin = SDL_GetTicks();
        if(frameBegin-prevFrameBegin >= MS_PER_FRAME) {
            prevFrameBegin = frameBegin;
            SDL_Event e;
            while(SDL_PollEvent(&e)) {
                switch(e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_VIDEORESIZE:
                    screen.screen = SDL_SetVideoMode(e.resize.w,e.resize.h, 32, SDL_SWSURFACE | SDL_RESIZABLE );
                    break;
                //case SDL_KEYDOWN: {
                //        std::string s;
                //        std::cout << s << std::endl;
                //        std::cout.flush();
                //        s += (char)e.key.keysym.unicode;
                //        morse.add(s);
                //    }
                //    break;
#ifdef DEBUG
                case SDL_KEYDOWN: {
                        std::string s = "";
                        s += (char)e.key.keysym.unicode;
                        morseToAdd += s;
                    }
                    break;
#endif
                default:
                    break;
                }
            }

            for(size_t i=0;i<morseToAdd.length();++i) {
                std::string morseText = morseToAdd.substr(i,1);
                if(morseText == "@") {
                    locked = true;
                } else if(locked && morseText == "^") {
                    locked = false;
                } else if(!locked && !morseText.empty()) {
                    morse.add(morseText);
                }
            }
            morseToAdd.clear();

            SDL_FillRect(screen.screen,NULL,SDL_MapRGB(screen.screen->format,255,255,255));

            std::cout << (morse.text()+morse.currWord()) << "|" << morse.morseLetter() << std::endl;

            int x,y;
            drawTextLines(screen.screen,font.font,0,0,(morse.text()+morse.currWord()),txtColor,&x,&y);
            std::string morseTxt = morse.morseLetter();
            if(morseTxt.empty()) {
                morseTxt = "_";
            }
            drawTextLines(screen.screen,font.font,x,y,morseTxt,morseColor,&x,&y);

            std::vector<std::pair<std::string,std::string> > suggestions = morse.morseSuggestions();
            std::vector<std::string> suggestionLines;
            size_t lineLength = 0;
            for(size_t i=0;i<suggestions.size() && i<10;++i) {
                size_t length = suggestions[i].first.length() + suggestions[i].second.length()+1;
                if(length > lineLength) {
                    lineLength = length;
                }
            }
            for(size_t i=0;i<suggestions.size() && i<10;++i) {
                size_t length = suggestions[i].first.length() + suggestions[i].second.length()+1;
                std::string pad = "";
                for(int i=length;i<lineLength;++i) {
                    pad += " ";
                }
                suggestionLines.push_back(suggestions[i].first+ " " + pad + suggestions[i].second);
            }
            SDL_Surface* morsePrompt = buildWindow(font.font,suggestionLines,0,txtColor,bgColor,bgColor);
            int morsePromptH = 0;
            if(morsePrompt) {
                if(x+morsePrompt->w >= screen.screen->w) {
                    int height;
                    TTF_SizeText(font.font,morseTxt.c_str(),NULL,&height);
                    y += height;
                    x = 0;
                }
                SDL_Rect windowLoc = { x,y,0,0 };
                x += morsePrompt->w;
                SDL_BlitSurface(morsePrompt,NULL,screen.screen,&windowLoc);
                SDL_FreeSurface(morsePrompt);
            }
            const std::vector<std::string>& wordSuggs = morse.wordSuggestions();
            SDL_Surface* suggPrompt = buildWindow(font.font,wordSuggs,morse.selectionIndex(),txtColor,suggColor,morseColor);
            if(suggPrompt) {
                if(x+suggPrompt->w >= screen.screen->w) {
                    int height;
                    TTF_SizeText(font.font,morseTxt.c_str(),NULL,&height);
                    y += height;
                    x = 0;
                }
                SDL_Rect windowLoc = { x,y,0,0 };
                x += suggPrompt->w;
                SDL_BlitSurface(suggPrompt,NULL,screen.screen,&windowLoc);
                SDL_FreeSurface(suggPrompt);
            }

            if(locked) {
                std::vector<std::string> lines;
                lines.push_back("Locked. Nod up to unlock");
                SDL_Surface* lockDialog = buildWindow(font.font,lines,0,morseColor,suggColor,suggColor);
                SDL_Rect loc = { (screen.screen->w - lockDialog->w)/2,
                                 (screen.screen->h - lockDialog->h)/2,0,0 };
                SDL_BlitSurface(lockDialog,NULL,screen.screen,&loc);
                SDL_FreeSurface(lockDialog);
            }

            SDL_Flip(screen.screen);
            //Uint32 frameEnd = SDL_GetTicks();
            //Uint32 frameTime = frameEnd-frameBegin;
            //std::cout << frameTime << "ms/frame" << std::endl;
            //std::cout.flush();
            //if(frameTime < MS_PER_FRAME) {
            //    SDL_Delay(MS_PER_FRAME-(SDL_GetTicks()-frameBegin));
            //}
        }
        SDL_Delay(1);
    }
    return 0;





}
