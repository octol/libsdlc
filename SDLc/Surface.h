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

#ifndef SDLC_SURFACE_H
#define SDLC_SURFACE_H

#define USE_SDL_IMAGE

#include "BaseSurface.h"

#ifdef USE_SDL_IMAGE
#include "SDL_image.h"
#endif

namespace sdlc {

class Surface : public BaseSurface {
public:
    Surface();
    explicit Surface(std::string path);
    Surface(const Surface& surface);
    Surface(Surface&& surface);
    Surface& operator=(const Surface& rhs);
    Surface& operator=(Surface&& rhs);
    virtual ~Surface();

    void alloc(int w, int h, int bpp, int type);
    void alloc(int w, int h, int bpp);
    void alloc(int w, int h);
    int load(const std::string path);
    int load_raw(const std::string path);
    int load_alpha(const std::string path);
    int load_color_key(const std::string path);
    int set_color_key();
    void reset();

    Surface* enable_per_pixel_alpha() const;

    int width() const;
    int height() const;

protected:
    int set_width(int w);
    int set_height(int h);

private:
    SDL_Surface* internal_load(std::string path);

    // Used for reference counting SDL_Surface* data.
    int *ref_count_ = nullptr;

    int width_ = 0;
    int height_ = 0;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
int Surface::width() const
{
    return width_;
}

inline
int Surface::height() const
{
    return height_;
}

inline
int Surface::set_width(int w) 
{
    return width_ = w;
}

inline
int Surface::set_height(int h) 
{
    return height_ = h;
}
} // namespace sdlc
#endif // SDLC_SURFACE_H
