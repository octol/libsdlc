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
#include <stdexcept>
#include "Screen.h"
#include "Surface.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Surface::Surface() : ref_count_(new std::size_t(1))
{
}

Surface::Surface(std::string path, LoadType load_type) : Surface()
{
    unchecked_load(path, load_type);
}

Surface::Surface(int w, int h, int bpp, int type) : Surface()
{
    unchecked_alloc(w, h, bpp, type);
}

Surface::Surface(int w, int h, int bpp) : Surface()
{
    unchecked_alloc(w, h, bpp);
}

Surface::Surface(int w, int h) : Surface()
{
    unchecked_alloc(w, h);
}

// Copy
Surface::Surface(const Surface& surface)
    : BaseSurface(surface.data),
      ref_count_(surface.ref_count_),
      width_(surface.width_),
      height_(surface.height_)
{
    ++(*ref_count_);
}

// Move
Surface::Surface(Surface&& surface)
    : BaseSurface(surface.data),
      ref_count_(surface.ref_count_),
      width_(surface.width_),
      height_(surface.height_)
{
    surface.data = nullptr;
    surface.ref_count_ = new std::size_t(1);
    surface.width_ = 0;
    surface.height_ = 0;
}

// Assignment
Surface& Surface::operator=(const Surface& rhs)
{
    if (this != &rhs) {
        if (--(*ref_count_) == 0) {
            SDL_FreeSurface(data);
            delete ref_count_;
        }

        data = rhs.data;
        ref_count_ = rhs.ref_count_;
        width_ = rhs.width_;
        height_ = rhs.height_;

        ++(*rhs.ref_count_);
    }
    return *this;
}

// Move assignment
Surface& Surface::operator=(Surface&& rhs)
{
    if (this != &rhs) {
        if (--(*ref_count_) == 0) {
            SDL_FreeSurface(data);
            delete ref_count_;
        }

        data = rhs.data;
        ref_count_ = rhs.ref_count_;
        width_ = rhs.width_;
        height_ = rhs.height_;

        rhs.data = nullptr;
        rhs.ref_count_ = new std::size_t(1);
        rhs.width_ = 0;
        rhs.height_ = 0;
    }
    return *this;
}

Surface::~Surface()
{
    if (--(*ref_count_) == 0) {
        SDL_FreeSurface(data);
        delete ref_count_;
    }
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Surface::make_unique()
{
    if (*ref_count_ != 1) {
        SDL_Surface* new_data = nullptr;
        if (data != nullptr) {
            new_data = SDL_DisplayFormat(data);
            if (!new_data)
                throw std::runtime_error("SDL_DisplayFormat failed");
        }

        --(*ref_count_);
        ref_count_ = new std::size_t(1);
        data = new_data;
        width_ = data->w;
        height_ = data->h;
    }
}

void Surface::reset()
{
    if (--(*ref_count_) == 0) {
        SDL_FreeSurface(data);
        delete ref_count_;
    }

    // Restore plain constructor state.
    data = nullptr;
    ref_count_ = nullptr;
    width_ = 0;
    height_ = 0;

    ref_count_ = new std::size_t(1);
}

void Surface::alloc(int w, int h, int bpp, int type)
{
    // Free any previous data
    reset();
    unchecked_alloc(w, h, bpp, type);
}

void Surface::alloc(int w, int h, int bpp)
{
    SDL_Surface* screen = Screen::video_surface();
    alloc(w, h, bpp, screen->flags & SDL_HWSURFACE);
}

void Surface::alloc(int w, int h)
{
    SDL_Surface* screen = Screen::video_surface();
    alloc(w, h, screen->format->BitsPerPixel);
}

void Surface::load(const std::string path, enum LoadType load_type)
{
    reset();
    unchecked_load(path, load_type);
}

void Surface::set_color_key()
{
    SDL_Surface* screen = Screen::video_surface();
    uint32_t c = SDL_MapRGB(screen->format, 255, 0, 255);

    if (SDL_SetColorKey(data, SDL_SRCCOLORKEY | SDL_RLEACCEL, c) == -1) 
        throw std::runtime_error("SDL_SetColorKey() failed");
}

Surface* Surface::enable_per_pixel_alpha() const
{
    SDL_Surface* data_alpha = SDL_DisplayFormatAlpha(data);
    if (data_alpha == NULL)
        throw std::runtime_error("SDL_DisplayFormatAlpha failed");
    
    Surface* surface_alpha = new Surface;
    surface_alpha->data = data_alpha;
    surface_alpha->width_ = surface_alpha->data->w;
    surface_alpha->height_ = surface_alpha->data->h;
    return surface_alpha;
}

// -----------------------------------------------------------------------------
// Static functions
// -----------------------------------------------------------------------------

void Surface::check_for_transparency(Surface& s, bool& pink, bool& alpha)
{
    check_for_transparency(std::move(s), pink, alpha);
}

void Surface::check_for_transparency(Surface&& s, bool& pink, bool& alpha)
{
    uint8_t r[4], g[4], b[4], a[4];
    int w = s.width();
    int h = s.height();
    s.lock();
    s.get_pix(0, 0, &r[0], &g[0], &b[0], &a[0]);
    s.get_pix(w - 1, 0, &r[1], &g[1], &b[1], &a[1]);
    s.get_pix(0, h - 1, &r[2], &g[2], &b[2], &a[2]);
    s.get_pix(w - 1, h - 1, &r[3], &g[3], &b[3], &a[3]);
    s.unlock();

    for (int i = 0; i < 4; i++) {
        if (r[i] >= 248 && g[i] == 0 && b[i] >= 248)
            pink = true;
        if (a[i] != 255)
            alpha = true;
    }
}


// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

SDL_Surface* Surface::sdl_load(std::string path)
{
#ifndef USE_SDL_IMAGE
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
#endif
#ifdef USE_SDL_IMAGE
    SDL_Surface* surface = IMG_Load(path.c_str());
#endif

    if (surface == NULL) 
        throw std::runtime_error("Surface::sdl_load(): " 
                                + std::string(SDL_GetError()));

    width_ = surface->w;
    height_ = surface->h;

    return surface;
}

void Surface::unchecked_alloc(int w, int h, int bpp, int type)
{
    SDL_Surface* screen = Screen::video_surface();
    SDL_Surface* surface = SDL_CreateRGBSurface(type, w, h, bpp, 
                                                screen->format->Rmask, 
                                                screen->format->Gmask,
                                                screen->format->Bmask,
                                                screen->format->Amask);
    if (surface == NULL)
        throw std::runtime_error("SDL_CreateRGBSurface() failed");

    data = SDL_DisplayFormat(surface);
    SDL_FreeSurface(surface);
    if (data == NULL) 
        throw std::runtime_error("SDL_DisplayFormat() failed");

    width_ = data->w;
    height_ = data->h;
}

void Surface::unchecked_alloc(int w, int h, int bpp)
{
    SDL_Surface* screen = Screen::video_surface();
    unchecked_alloc(w, h, bpp, screen->flags & SDL_HWSURFACE);
}

void Surface::unchecked_alloc(int w, int h)
{
    SDL_Surface* screen = Screen::video_surface();
    unchecked_alloc(w, h, screen->format->BitsPerPixel);
}

void Surface::unchecked_load(std::string path, enum LoadType load_type)
{
    if (load_type == LoadType::Auto) {
        bool pinkfound = false, alphafound = false;
        check_for_transparency(Surface(path, LoadType::Alpha), pinkfound, alphafound);

        if (pinkfound) {
            unchecked_load(path, LoadType::Colorkey);
        } else if (alphafound) {
            unchecked_load(path, LoadType::Alpha);
        } else {
            unchecked_load(path, LoadType::Raw);
        }
    } else {
        SDL_Surface* surface = sdl_load(path);
        if (load_type == LoadType::Alpha) {
            data = SDL_DisplayFormatAlpha(surface);
        } else {
            data = SDL_DisplayFormat(surface);
        }
        SDL_FreeSurface(surface);
        if (data == NULL) {
            data = nullptr;
            throw std::runtime_error("Setting display format failed");
        }
        if (load_type == LoadType::Colorkey) {
            set_color_key();
        }
    }
    width_ = data->w;
    height_ = data->h;
}

} // namespace sdlc
