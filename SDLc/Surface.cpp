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

#include <cassert>
#include <iostream>
#include "Screen.h"
#include "Surface.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Surface::Surface() : ref_count_(new int(0))
{
}

Surface::Surface(std::string path) : Surface()
{
    load(path);
}

// Copy
Surface::Surface(const Surface& surface) : Surface()
{
    if (*surface.ref_count_ > 0) {
        data = surface.data;
        delete ref_count_;
        ref_count_ = surface.ref_count_;
        width_ = surface.width_;
        height_ = surface.height_;

        ++(*ref_count_); 
    }
}

// Move
Surface::Surface(Surface&& surface)
    : ref_count_(surface.ref_count_),
      width_(surface.width_),
      height_(surface.height_)
{
    data = surface.data;

    surface.data = nullptr;
    surface.ref_count_ = new int(0);
    surface.width_ = 0;
    surface.height_ = 0;
}

// Assignment
Surface& Surface::operator=(const Surface& rhs)
{
    if (this != &rhs && *rhs.ref_count_ > 0) {
        if (--(*ref_count_) <= 0) {
            SDL_FreeSurface(data);
            delete ref_count_;
        }

        data = rhs.data;
        ref_count_ = rhs.ref_count_;
        width_ = rhs.width_;
        height_ = rhs.height_;

        ++(*ref_count_); 
    }
    return *this;
}

// Move assignment
Surface& Surface::operator=(Surface&& rhs)
{
    if (this != &rhs) {
        if (--(*ref_count_) <= 0) {
            SDL_FreeSurface(data);
            delete ref_count_;
        }

        data = rhs.data;
        ref_count_ = rhs.ref_count_;
        width_ = rhs.width_;
        height_ = rhs.height_;

        rhs.data = nullptr;
        rhs.ref_count_ = new int(0);
        rhs.width_ = 0;
        rhs.height_ = 0;
    }
    return *this;
}

Surface::~Surface()
{
    // Note that *ref_count_ can be < 0 if we initialise without Surface data
    // and then call reset.
    if (--(*ref_count_) <= 0) {
        // Last reference
        SDL_FreeSurface(data);
        delete ref_count_;
        data = nullptr;
        ref_count_ = nullptr;
    }

    data = nullptr;
    ref_count_ = nullptr;
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Surface::alloc(int w, int h, int bpp, int type)
{
    // Free any previous data
    reset();

    SDL_Surface* screen = Screen::video_surface();
    SDL_Surface* surface = SDL_CreateRGBSurface(type, w, h, bpp, 
                                                screen->format->Rmask, 
                                                screen->format->Gmask,
                                                screen->format->Bmask,
                                                screen->format->Amask);

    data = SDL_DisplayFormat(surface);
    if (data) 
        *ref_count_ = 1;

    SDL_FreeSurface(surface);

    set_width(data->w);
    set_height(data->h);
}

void Surface::alloc(int w, int h, int bpp)
{
    SDL_Surface* screen = Screen::video_surface();

    if (screen->flags == SDL_SWSURFACE || 
        screen->flags == (SDL_SWSURFACE | SDL_FULLSCREEN)) {
        alloc(w, h, bpp, SDL_SWSURFACE);
    } else if (screen->flags == SDL_HWSURFACE || 
               screen->flags == (SDL_HWSURFACE | SDL_FULLSCREEN) || 
               screen->flags == (SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN)) {
        alloc(w, h, bpp, SDL_HWSURFACE);
    } else {
        std::cout << "Surface::alloc() Undefined videomode" << std::endl;
    }
}

void Surface::alloc(int w, int h)
{
    SDL_Surface* screen = Screen::video_surface();
    alloc(w, h, screen->format->BitsPerPixel);
}

void Surface::load(const std::string path)
{
    uint8_t r[4], g[4], b[4], a[4];

    Surface surface;
    surface.load_alpha(path);
    int w = surface.width();
    int h = surface.height();
    surface.lock();
    surface.get_pix(0, 0, &r[0], &g[0], &b[0], &a[0]);
    surface.get_pix(w - 1, 0, &r[1], &g[1], &b[1], &a[1]);
    surface.get_pix(0, h - 1, &r[2], &g[2], &b[2], &a[2]);
    surface.get_pix(w - 1, h - 1, &r[3], &g[3], &b[3], &a[3]);
    surface.unlock();

    bool pinkfound = false;
    bool alphafound = false;
    for (int i = 0; i < 4; i++) {
        if (r[i] >= 248 && g[i] == 0 && b[i] >= 248)
            pinkfound = true;
        if (a[i] != 255)
            alphafound = true;
    }

    if (pinkfound)
        load_color_key(path);
    else if (alphafound)
        load_alpha(path);
    else load_raw(path);
}

void Surface::load_raw(const std::string path)
{
    reset();
    SDL_Surface* surface = internal_load(path);
    data = SDL_DisplayFormat(surface);
    if (data) {
        assert(ref_count_);
        *ref_count_ = 1;
    }
    SDL_FreeSurface(surface);
}

void Surface::load_alpha(const std::string path)
{
    reset();
    SDL_Surface* surface = internal_load(path);
    data = SDL_DisplayFormatAlpha(surface);
    if (data) {
        assert(ref_count_);
        *ref_count_ = 1;
    }
    SDL_FreeSurface(surface);
}

void Surface::load_color_key(const std::string path)
{
    load_raw(path);
    set_color_key();
}

void Surface::set_color_key()
{
    SDL_Surface* screen = Screen::video_surface();
    SDL_SetColorKey(data, SDL_SRCCOLORKEY | SDL_RLEACCEL, 
                    SDL_MapRGB(screen->format, 255, 0, 255));
}

void Surface::reset()
{
    // Note that *ref_count_ can be < 0 after decrementing if we initialise
    // without Surface data and then call reset.
    if (--(*ref_count_) <= 0) {
        SDL_FreeSurface(data);
        delete ref_count_;
    }

    // Restore plain constructor state.
    data = nullptr;
    ref_count_ = new int(0);
    width_ = 0;
    height_ = 0;
}

Surface* Surface::enable_per_pixel_alpha() const
{
    SDL_Surface* data_alpha = SDL_DisplayFormatAlpha(data);
    
    Surface* surface_alpha = new Surface;
    surface_alpha->data = data_alpha;
    *(surface_alpha->ref_count_) = 1;

    surface_alpha->width_ = surface_alpha->data->w;
    surface_alpha->height_ = surface_alpha->data->h;
    return surface_alpha;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

SDL_Surface* Surface::internal_load(std::string path)
{
    SDL_Surface* surface;

#ifndef USE_SDL_IMAGE
    surface = SDL_LoadBMP(path.c_str());
#endif
#ifdef USE_SDL_IMAGE
    surface = IMG_Load(path.c_str());
#endif

    if (surface == NULL)
        std::cerr << "Surface::internal_load() " << SDL_GetError() << std::endl;

    width_ = surface->w;
    height_ = surface->h;

    return surface;
}
} // namespace sdlc
