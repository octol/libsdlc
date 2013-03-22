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

// TODO: move to composition.
class Surface : public BaseSurface {
public:
    Surface() {};
    virtual ~Surface();

    void alloc(int w, int h, int bpp, int type);
    void alloc(int w, int h, int bpp);
    void alloc(int w, int h);
    void load(const std::string path);
    void load_raw(const std::string path);
    void load_alpha(const std::string path);
    void load_color_key(const std::string path);
    void link(SDL_Surface* src);  // remove this ?
    void link(Surface* src);
    void unload();

    void enable_per_pixel_alpha();
    void set_color_key();

    int width() const;
    int height() const;

    // DEPRECATED
    void loadRaw(const std::string path);
    void loadAlpha(const std::string path);
    void loadColorkey(const std::string path);

    void enablePerPixelAlpha();
    void setColorKey();

    int getWidth() const;
    int getHeight() const;

protected:
    int set_width(int w);
    int set_height(int h);

    // DEPRECATED
    int setWidth(int w);
    int setHeight(int h);

private:
    SDL_Surface* internal_load(std::string path);
    bool loaded_ = false;
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

inline
int Surface::getWidth() const
{
    return width_;
}

inline
int Surface::getHeight() const
{
    return height_;
}

inline
int Surface::setWidth(int w) 
{
    return (width_ = w);
}

inline
int Surface::setHeight(int h) 
{
    return (height_ = h);
}
} // namespace sdlc
#endif // SDLC_SURFACE_H
