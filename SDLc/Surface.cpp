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
#include "Screen.h"
#include "Surface.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Surface::Surface()
{
}

Surface::~Surface()
{
    unload();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Surface::alloc(int w, int h, int bpp, int type)
{
    SDL_Surface* screen = SDL_GetVideoSurface();
    SDL_Surface* surface = SDL_CreateRGBSurface(type, w, h, bpp, 
                                                screen->format->Rmask, 
                                                screen->format->Gmask,
                                                screen->format->Bmask,
                                                screen->format->Amask);

    data = SDL_DisplayFormat(surface);
    SDL_FreeSurface(surface);

    setWidth(data->w);
    setHeight(data->h);
    loaded = true;
}

void Surface::alloc(int w, int h, int bpp)
{
    SDL_Surface* screen = SDL_GetVideoSurface();

    if (screen->flags == SDL_SWSURFACE || 
        screen->flags == (SDL_SWSURFACE | SDL_FULLSCREEN)) {
        alloc(w, h, bpp, SDL_SWSURFACE);

    } else if (screen->flags == SDL_HWSURFACE || 
               screen->flags == (SDL_HWSURFACE | SDL_FULLSCREEN) || 
               screen->flags == (SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN))
        alloc(w, h, bpp, SDL_HWSURFACE);

    else {
        std::cout << "Surface::alloc() Undefined videomode" << std::endl;
    }
}

void Surface::alloc(int w, int h)
{
    SDL_Surface* screen = SDL_GetVideoSurface();
    alloc(w, h, screen->format->BitsPerPixel);
}

void Surface::load(const std::string path)
{
    uint8_t r[4], g[4], b[4], a[4];

    Surface surface;
    surface.loadAlpha(path);
    int w = surface.getWidth();
    int h = surface.getHeight();
    surface.lock();
    surface.getPix(0, 0, &r[0], &g[0], &b[0], &a[0]);
    surface.getPix(w - 1, 0, &r[1], &g[1], &b[1], &a[1]);
    surface.getPix(0, h - 1, &r[2], &g[2], &b[2], &a[2]);
    surface.getPix(w - 1, h - 1, &r[3], &g[3], &b[3], &a[3]);
    surface.unlock();

    int i;
    bool pinkfound = false;
    bool alphafound = false;
    for (i = 0; i < 4; i++) {
        //if(r[i] == 255 && g[i] == 0 && b[i] == 255)
        if (r[i] >= 248 && g[i] == 0 && b[i] >= 248)
            pinkfound = true;
        if (a[i] != 255)
            alphafound = true;
    }

    if (pinkfound)
        loadColorkey(path);
    else if (alphafound)
        loadAlpha(path);
    else loadRaw(path);
}

void Surface::loadRaw(const std::string path)
{
    SDL_Surface* surface = internalLoad(path);
    data = SDL_DisplayFormat(surface);
    SDL_FreeSurface(surface);
}

void Surface::loadAlpha(const std::string path)
{
    SDL_Surface* surface = internalLoad(path);
    data = SDL_DisplayFormatAlpha(surface);
    SDL_FreeSurface(surface);
}

void Surface::loadColorkey(const std::string path)
{
    loadRaw(path);
    setColorKey();
}

void Surface::link(SDL_Surface* src)
{
    data = src;
    width = data->w;
    height = data->h;
}

void Surface::link(Surface* src)
{
    link(src->data);
}

void Surface::unload()
{
    if (loaded) {
        SDL_FreeSurface(data);
        loaded = false;
    }
}

void Surface::enablePerPixelAlpha()
{
    SDL_Surface* surface;
    surface = SDL_DisplayFormatAlpha(data);
    SDL_FreeSurface(data);
    data = surface;
}

void Surface::setColorKey()
{
    SDL_Surface* screen = SDL_GetVideoSurface();
    SDL_SetColorKey(data, SDL_SRCCOLORKEY | SDL_RLEACCEL, 
                    SDL_MapRGB(screen->format, 255, 0, 255));
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

SDL_Surface* Surface::internalLoad(std::string path)
{
    SDL_Surface* surface;

#ifndef USE_SDL_IMAGE
    surface = SDL_LoadBMP(path.c_str());
#endif
#ifdef USE_SDL_IMAGE
    surface = IMG_Load(path.c_str());
#endif

    if (surface == NULL)
        std::cerr << "Surface::internalLoad() " << SDL_GetError() << std::endl;

    width = surface->w;
    height = surface->h;
    loaded = true;

    return surface;
}
