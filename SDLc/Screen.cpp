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

#include <ostream>
#include <iostream>
#include "Sprite.h"
#include "Misc.h"
#include "Screen.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Free functions
// -----------------------------------------------------------------------------

int init()
{
    return SDL_Init(0);
}

int init(uint32_t flags)
{
    return SDL_Init(flags);
}

void quit()
{
    SDL_Quit();
}

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Screen::Screen()
{
}

Screen::Screen(int w, int h, int bpp, int type)
{
    open(w, h, bpp, type);
}

Screen::~Screen()
{
    close();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Screen::open(int w, int h, int bpp, int type)
{
    SDL_InitSubSystem(SDL_INIT_VIDEO);

    data = SDL_SetVideoMode(w, h, bpp, type);
    if (data == NULL) {
        std::cerr << "Screen::init() " << SDL_GetError() << std::endl;
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        exit(2);
    }
}

void Screen::close()
{
    if (SDL_WasInit(SDL_INIT_VIDEO) == SDL_INIT_VIDEO) 
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

int Screen::show_cursor(bool toggle)
{
    return SDL_ShowCursor(toggle);
}

void Screen::set_caption(std::string title)
{
    SDL_WM_SetCaption(title.c_str(), NULL);
}

void Screen::update_area(int x, int y, int w, int h)
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

        update_r_[update_i_].x = x;
        update_r_[update_i_].y = y;
        update_r_[update_i_].w = w;
        update_r_[update_i_].h = h;

        assert(update_i_ < 255);
        update_i_++;
    }
}

void Screen::flip()
{
    // flip screen
    for (int i = 0; i < update_i_; i++) {
        SDL_UpdateRect(data, update_r_[i].x, update_r_[i].y, 
                       update_r_[i].w, update_r_[i].h);
    }
    update_i_ = 0;
}

void Screen::flip_all()
{
    SDL_Flip(data);
    update_i_ = 0;
}

// -----------------------------------------------------------------------------
// Static functions
// -----------------------------------------------------------------------------

SDL_Surface* Screen::video_surface()
{
    return SDL_GetVideoSurface();
}

// -----------------------------------------------------------------------------
// Free functions
// -----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Screen& screen)
{
    (void)(screen);
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    
    os << "hw_available: "   << info->hw_available        << std::endl;
    os << "wm_available: "   << info->wm_available        << std::endl;
    os << "UnusedBits1: "    << info->UnusedBits1         << std::endl;
    os << "UnusedBits2: "    << info->UnusedBits2         << std::endl;
    os << "blit_hw: "        << info->blit_hw             << std::endl;
    os << "blit_hw_CC: "     << info->blit_hw_CC          << std::endl;
    os << "blit_hw_A: "      << info->blit_hw_A           << std::endl;
    os << "blit_sw: "        << info->blit_sw             << std::endl;
    os << "blit_sw_CC: "     << info->blit_sw_CC          << std::endl;
    os << "blit_sw_A: "      << info->blit_sw_A           << std::endl;
    os << "blit_fill: "      << info->blit_fill           << std::endl;
    os << "UnusedBits3: "    << info->UnusedBits3         << std::endl;
    os << "video_mem: "      << info->video_mem           << std::endl;
    os << "BitsPerPixel: "   << info->vfmt->BitsPerPixel  << std::endl;
    os << "BytesPerPixel: "  << info->vfmt->BytesPerPixel;

    return os;
}

} // namespace sdlc
