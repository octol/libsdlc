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
    Surface();
    virtual ~Surface();

    void alloc(int w, int h, int bpp, int type);
    void alloc(int w, int h, int bpp);
    void alloc(int w, int h);
    void load(const std::string path);
    void loadRaw(const std::string path);
    void loadAlpha(const std::string path);
    void loadColorkey(const std::string path);
    void link(SDL_Surface* src);  // remove this ?
    void link(Surface* src);
    void unload();

    void enablePerPixelAlpha();
    void setColorKey();

    int getWidth() const;
    int getHeight() const;

protected:
    int setWidth(int w);
    int setHeight(int h);

private:
    SDL_Surface* internalLoad(std::string path);
    bool loaded = false;
    int width = 0;
    int height = 0;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
int Surface::getWidth() const
{
    return width;
}

inline
int Surface::getHeight() const
{
    return height;
}

inline
int Surface::setWidth(int w) 
{
    return (width = w);
}

inline
int Surface::setHeight(int h) 
{
    return (height = h);
}
} // namespace sdlc
#endif // SDLC_SURFACE_H
