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

// -----------------------------------------------------------------------------
// Reference counted Surface. Adds memory management on top of BaseSurface.
// -----------------------------------------------------------------------------

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
    Surface(int w, int h, int bpp, int type);
    Surface(int w, int h, int bpp);
    Surface(int w, int h);

    // Copy/move the surface using the same underlying data
    Surface(const Surface& surface);
    Surface(Surface&& surface);
    Surface& operator=(const Surface& rhs);
    Surface& operator=(Surface&& rhs);
    virtual ~Surface();

    // Make the current object independent from other Surfaces by
    // duplicating the underlying data.
    void make_unique();

    // Reset the object to an initial state equivalent to running the
    // default constructor. Deletes the underlying data if this is the last
    // reference.
    void reset();

    // Allocate an empty surface ready from drawing on/blitting to.
    void alloc(int w, int h, int bpp, int type);
    void alloc(int w, int h, int bpp);
    void alloc(int w, int h);

    // Allocate and load the surface from a bitmap.
    void load(std::string path);
    void load_raw(std::string path);
    void load_alpha(std::string path);
    void load_color_key(std::string path);

    // Sets the color key used for transparency.
    void set_color_key();

    // Returns a new surface where per-pixel alpha blending is supported.
    Surface* enable_per_pixel_alpha() const;

    int width() const;
    int height() const;

    static void check_for_transparency(Surface& s, bool& pink, bool& alpha);

protected:
    int set_width(int w);
    int set_height(int h);

private:
    SDL_Surface* sdl_load(std::string path);

    void unchecked_alloc(int w, int h, int bpp, int type);
    void unchecked_alloc(int w, int h, int bpp);
    void unchecked_alloc(int w, int h);
    void unchecked_load(std::string path);
    void unchecked_load_raw(std::string path);
    void unchecked_load_alpha(std::string path);
    void unchecked_load_color_key(std::string path);

    // Used for reference counting SDL_Surface* data.
    std::size_t* ref_count_ = nullptr;

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
