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

Surface::Surface() : ref_count_(new int(0))
{
#ifdef DEBUG_LOG
    std::cerr << "init (" << this << ")";
    std::cerr << ", ref: " << *ref_count_ << " (" << ref_count_ << ")";
    std::cerr << ", data: " << data << std::endl;
#endif
}

Surface::Surface(std::string path) : Surface()
{
#ifdef DEBUG_LOG
    std::cerr << "loading " << path << " (" << this << ")" << std::endl;
#endif
    load(path);
#ifdef DEBUG_LOG
    std::cerr << "  data is now: " << data << std::endl;
#endif
}

Surface::Surface(int w, int h, int bpp, int type) : Surface()
{
    alloc(w, h, bpp, type);
}

Surface::Surface(int w, int h, int bpp) : Surface()
{
    alloc(w, h, bpp);
}

Surface::Surface(int w, int h) : Surface()
{
    alloc(w, h);
}

// Copy
Surface::Surface(const Surface& surface) : Surface()
{
#ifdef DEBUG_LOG
    std::cerr << "cc (" << this << ")";
#endif

    if (*surface.ref_count_ > 0) {
#ifdef DEBUG_LOG
        std::cerr << " delete: " << ref_count_;
#endif
        data = surface.data;
        delete ref_count_;
        ref_count_ = surface.ref_count_;
        width_ = surface.width_;
        height_ = surface.height_;

        ++(*ref_count_); 
    }

#ifdef DEBUG_LOG
    std::cerr << ", ref: " << *ref_count_ << " (" << ref_count_ << ")";
    std::cerr << ", data: " << data << std::endl;
#endif
}

// Move
Surface::Surface(Surface&& surface)
    : ref_count_(surface.ref_count_),
      width_(surface.width_),
      height_(surface.height_)
{
#ifdef DEBUG_LOG
    std::cerr << "move: (" << this << ")" << std::endl;
#endif
    data = surface.data;

    surface.data = nullptr;
    surface.ref_count_ = new int(0);
    surface.width_ = 0;
    surface.height_ = 0;
    assert(!(*ref_count_ <= 0 && data != nullptr));
}

// Assignment
Surface& Surface::operator=(const Surface& rhs)
{
#ifdef DEBUG_LOG
    std::cerr << "as: (" << this << ")" << std::endl;
#endif
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
#ifdef DEBUG_LOG
    std::cerr << "as move: (" << this << ")" << std::endl;
    std::cerr << std::endl;
#endif
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
        assert(!(*rhs.ref_count_ <= 0 && rhs.data != nullptr));
    }
    return *this;
}

Surface::~Surface()
{
#ifdef DEBUG_LOG
    std::cerr << "Destroying surface (" << this << ")";
    std::cerr << ", ref: " << *ref_count_ << " (" << ref_count_ << ")";
    std::cerr << ", data: " << data;
#endif

    // Note that *ref_count_ can be < 0 if we initialise without Surface data
    // and then call reset.
    if (--(*ref_count_) <= 0) {
#ifdef DEBUG_LOG
        std::cerr << " (deleting)";
#endif
        // Last reference
        SDL_FreeSurface(data);
        delete ref_count_;
        data = nullptr;
        ref_count_ = nullptr;
    }
#ifdef DEBUG_LOG
    std::cerr << std::endl;
#endif

    data = nullptr;
    ref_count_ = nullptr;
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

int Surface::make_independent_copy()
{
    SDL_Surface* new_data = SDL_DisplayFormat(data);
    if (new_data) {
        reset();                // Remove our current refence.
        data = new_data;        // Replace with new data
        *ref_count_ = 1;        // Set reference counting
        set_width(data->w);
        set_height(data->h);
        return 0;
    } else {
        return -1;
    }
}

void Surface::alloc(int w, int h, int bpp, int type)
{
#ifdef DEBUG_LOG
    std::cerr << "alloc surface (" << this << ")";
    std::cerr << ", ref: " << *ref_count_ << " (" << ref_count_ << ")";
    std::cerr << ", data: " << data;
    std::cerr << std::endl;
#endif

    // Free any previous data
    reset();

    SDL_Surface* screen = Screen::video_surface();
    SDL_Surface* surface = SDL_CreateRGBSurface(type, w, h, bpp, 
                                                screen->format->Rmask, 
                                                screen->format->Gmask,
                                                screen->format->Bmask,
                                                screen->format->Amask);

    data = SDL_DisplayFormat(surface);
    if (data == NULL) 
        throw std::runtime_error("SDL_DisplayFormat() failed");
    *ref_count_ = 1;
    SDL_FreeSurface(surface);

    set_width(data->w);
    set_height(data->h);
    assert(!(*ref_count_ <= 0 && data != nullptr));

#ifdef DEBUG_LOG
    std::cerr << "  done alloc surface (" << this << ")";
    std::cerr << ", ref: " << *ref_count_ << " (" << ref_count_ << ")";
    std::cerr << ", data: " << data;
    std::cerr << std::endl;
#endif
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
        throw std::invalid_argument("Surface::alloc() undefined video mode");
    }
}

void Surface::alloc(int w, int h)
{
    SDL_Surface* screen = Screen::video_surface();
    alloc(w, h, screen->format->BitsPerPixel);
}

void Surface::load(const std::string path)
{
#ifdef DEBUG_LOG
    std::cerr << "load (" << this << ")";
    std::cerr << ", ref: " << *ref_count_ << " (" << ref_count_ << ")";
    std::cerr << ", data: " << data;
    std::cerr << std::endl;
#endif


#ifdef DEBUG_LOG
    std::cerr << "  temp: "; 
#endif
    Surface surface;
#ifdef DEBUG_LOG
    std::cerr << "  (&surface)" << std::endl;
#endif
    surface.load_alpha(path);
    bool pinkfound = false, alphafound = false;
    check_for_transparency(surface, pinkfound, alphafound);

    if (pinkfound)
        load_color_key(path);
    else if (alphafound)
        load_alpha(path);
    else load_raw(path);

    assert(!(*ref_count_ <= 0 && data != nullptr));
#ifdef DEBUG_LOG
    std::cerr << "  done load (" << this << ")";
    std::cerr << ", ref: " << *ref_count_ << " (" << ref_count_ << ")";
    std::cerr << ", data: " << data;
    std::cerr << std::endl;
#endif
}

void Surface::load_raw(const std::string path)
{
    reset();
    SDL_Surface* surface = sdl_load(path);

    data = SDL_DisplayFormat(surface);
    if (data == NULL) 
        throw std::runtime_error("SDL_DisplayFormat() failed");

    *ref_count_ = 1;
    SDL_FreeSurface(surface);
}

void Surface::load_alpha(const std::string path)
{
    reset();
    SDL_Surface* surface = sdl_load(path);
    
    data = SDL_DisplayFormatAlpha(surface);
    if (data == NULL) 
        throw std::runtime_error("SDL_DisplayFormatAlpha() failed");

    *ref_count_ = 1;
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

    if (SDL_SetColorKey(data, SDL_SRCCOLORKEY | SDL_RLEACCEL, 
                        SDL_MapRGB(screen->format, 255, 0, 255)) == -1) {
        throw std::runtime_error("SDL_SetColorKey() failed");
    }
}

void Surface::reset()
{
#ifdef DEBUG_LOG
    std::cerr << "Reset surface (" << this << ")";
    std::cerr << ", ref: " << *ref_count_ << " (" << ref_count_ << ")";
    std::cerr << ", data: " << data;
#endif
 
    // Note that *ref_count_ can be < 0 after decrementing if we initialise
    // without Surface data and then call reset.
    if (--(*ref_count_) <= 0) {
#ifdef DEBUG_LOG
        std::cerr << " (deleting)";
#endif
        SDL_FreeSurface(data);
        delete ref_count_;
    }
#ifdef DEBUG_LOG
    std::cerr << std::endl;
#endif

    // Restore plain constructor state.
    data = nullptr;
    ref_count_ = new int(0);
    width_ = 0;
    height_ = 0;
#ifdef DEBUG_LOG
    assert(!(*ref_count_ <= 0 && data != nullptr));
    std::cerr << "  done reset surface (" << this << ")";
    std::cerr << ", ref: " << *ref_count_ << " (" << ref_count_ << ")";
    std::cerr << ", data: " << data;
    std::cerr << std::endl;
#endif
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
// Static functions
// -----------------------------------------------------------------------------

void Surface::check_for_transparency(Surface& s, bool& pink, bool& alpha)
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
    SDL_Surface* surface;

#ifndef USE_SDL_IMAGE
    surface = SDL_LoadBMP(path.c_str());
#endif
#ifdef USE_SDL_IMAGE
    surface = IMG_Load(path.c_str());
#endif

    if (surface == NULL) {
        throw std::invalid_argument("Surface::sdl_load(): " 
                                + std::string(SDL_GetError()));
    } else {
        width_ = surface->w;
        height_ = surface->h;
    }

    return surface;
}
} // namespace sdlc
