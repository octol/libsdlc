//    SDLc
//    Copyright (C) 2000, 2001, 2013 Jon Häggblad
//
//    This file is part of SDLc.
//
//    SDLc is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    SDLc is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with SDLc.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include "Sprite.h"
#include "Misc.h"
#include "Screen.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------
Screen::Screen()
{
}

Screen::~Screen()
{
    close();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------
void Screen::init(int w, int h, int bpp, int type)
{
    if (sdlinited == false) {
        SDL_Init(SDL_INIT_VIDEO);
        atexit(SDL_Quit);
        sdlinited = true;
    } else {
        SDL_InitSubSystem(SDL_INIT_VIDEO);
    }

    data = SDL_SetVideoMode(w, h, bpp, type);
    if (data == NULL) {
        std::cerr << "Screen::init() " << SDL_GetError() << std::endl;
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        exit(2);
    }
    SDL_Delay(100);
}

void Screen::close()
{
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Screen::printVideoInfo()
{
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    std::cout << std::endl;

    std::cout << "hw_available: "   << info->hw_available        << std::endl;
    std::cout << "wm_available: "   << info->wm_available        << std::endl;
    std::cout << "UnusedBits1: "    << info->UnusedBits1         << std::endl;
    std::cout << "UnusedBits2: "    << info->UnusedBits2         << std::endl;
    std::cout << "blit_hw: "        << info->blit_hw             << std::endl;
    std::cout << "blit_hw_CC: "     << info->blit_hw_CC          << std::endl;
    std::cout << "blit_hw_A: "      << info->blit_hw_A           << std::endl;
    std::cout << "blit_sw: "        << info->blit_sw             << std::endl;
    std::cout << "blit_sw_CC: "     << info->blit_sw_CC          << std::endl;
    std::cout << "blit_sw_A: "      << info->blit_sw_A           << std::endl;
    std::cout << "blit_fill: "      << info->blit_fill           << std::endl;
    std::cout << "UnusedBits3: "    << info->UnusedBits3         << std::endl;
    std::cout << "video_mem: "      << info->video_mem           << std::endl;
    std::cout << "BitsPerPixel: "   << info->vfmt->BitsPerPixel  << std::endl;
    std::cout << "BytesPerPixel: "  << info->vfmt->BytesPerPixel << std::endl;

    std::cout << std::endl;
}

void Screen::updateArea(int x, int y, int w, int h)
{
    if ((x + w) > 0 && x < data->w && (y + h) > 0 && y < data->h) {
        if (x < 0) {
            w = w + x;
            x = 0;
        }
        if (y < 0) {
            h = h + y;
            y = 0;
        }

        if (w > (data->w - x))
            w = (data->w - x);
        if (h > (data->h - y))
            h = (data->h - y);

        updateR[updateI].x = x;
        updateR[updateI].y = y;
        updateR[updateI].w = w;
        updateR[updateI].h = h;

        updateI++;
    }
}

void Screen::flip()
{
    // flip screen
    int i;
    for (i = 0; i < updateI; i++) {
        SDL_UpdateRect(data, updateR[i].x, updateR[i].y, 
                       updateR[i].w, updateR[i].h);
    }
    updateI = 0;
}

void Screen::flipAll()
{
    SDL_Flip(data);
    updateI = 0;
}
