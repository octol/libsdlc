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

#ifndef SDLC_BASE_SURFACE_H
#define SDLC_BASE_SURFACE_H

#include <string>
#include "SDL.h"

// Speeds up pixel ops on 640x* displays
//#define OPTIMIZE_SCREEN_WIDTH_640

// Might give slight speed gain
#define INLINE_FASTPIX

namespace sdlc {

class Font;
class Sprite;
class Surface;

class BaseSurface {
public:
    BaseSurface() {};
    BaseSurface(const BaseSurface& base_surface) = delete;
    const BaseSurface& operator=(const BaseSurface& base_surface) = delete;
    virtual ~BaseSurface() {};

    void printSurfaceInfo() const;

    void lock();
    void unlock();
    void blit(int x, int y, SDL_Surface* src, SDL_Rect rect);
    void blit(int x, int y, SDL_Surface* src);
    void blit(int x, int y, BaseSurface& src, SDL_Rect rect);
    void blit(int x, int y, BaseSurface& src);
    void blit(Sprite& sprite);

    // Drawing primitives
    void setPix(int x, int y, uint8_t r, uint8_t g, uint8_t b);  
    void blendPix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a); 
    void getPix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b) const; 

    // These are 16 bit only.
    void fastSetPix(int x, int y, uint8_t r, uint8_t g, uint8_t b); 
    void fastBlendPix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a); 
    void fastGetPix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b) const; 

    // These are 32 bit only.
    void setPix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a); 
    void fastSetPix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a); 
    void getPix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) const; 
    void fastGetPix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) const;

    void line(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b);
    void lineAA(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b);

    // These are 16 bit only
    void fastLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b);
    void fastLineAA(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b);

    void fillRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b);

    // The alpha value of the entire surface
    uint8_t setAlpha(uint8_t value);
    uint8_t getAlpha() const;

    // Basic printing. Uses fixed font if not specified.
    // Note that max(value) = 999 999 999.
    void print(int x, int y, const std::string text, uint8_t r, uint8_t g, uint8_t b);
    void print(int x, int y, const std::string text, Font& font);
    void print(int x, int y, Uint32 value, uint8_t r, uint8_t g, uint8_t b);
    void print(int x, int y, Uint32 value, Font& font); 

    virtual int getWidth() const;
    virtual int getHeight() const; 
    int getDepth() const;

    // Main pixel/surface data.
    // Almost all functions in the class work with this data. We make no
    // attempt to hide it, as we want to allow direct manipulation.
    SDL_Surface* data = nullptr;

private:
    void drawChar(int x, int y, char c, Uint32 color);   // used by print()
    uint8_t m_alpha = 255;  // surface alpha value, buffered
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
void BaseSurface::lock() 
{
    if (SDL_MUSTLOCK(data)) 
        SDL_LockSurface(data);
}

inline
void BaseSurface::unlock() 
{
    if (SDL_MUSTLOCK(data)) 
        SDL_UnlockSurface(data);
}

inline
void BaseSurface::blit(int x, int y, BaseSurface& src, SDL_Rect rect) 
{
    blit(x, y, src.data, rect);
}

inline
void BaseSurface::blit(int x, int y, BaseSurface& src)   
{
    blit(x, y, src.data);
}


#ifdef INLINE_FASTPIX
inline
void BaseSurface::fastSetPix(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t color = static_cast<uint16_t>(SDL_MapRGB(data->format, r, g, b));
#ifndef OPTIMIZE_SCREEN_WIDTH_640
    *((uint16_t*)data->pixels + ((y * data->pitch) >> 1) + x) = color;
#endif
#ifdef OPTIMIZE_SCREEN_WIDTH_640
    *((uint16_t*)data->pixels + y * 640 + x) = color;
#endif
}

inline
void BaseSurface::fastSetPix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    Uint32 color = SDL_MapRGBA(data->format, r, g, b, a);
#ifndef OPTIMIZE_SCREEN_WIDTH_640
    *((Uint32*)data->pixels + ((y * data->pitch) >> 2) + x) = color;
#endif
#ifdef OPTIMIZE_SCREEN_WIDTH_640
    *((Uint32*)data->pixels + y * 640 + x) = color;
#endif
}

inline
void BaseSurface::fastBlendPix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    uint8_t red = 0, green = 0, blue = 0;
    fastGetPix(x, y, &red, &green, &blue);
    red = ((a * (r - red)) >> 8) + red;
    green = ((a * (g - green)) >> 8) + green;
    blue = ((a * (b - blue)) >> 8) + blue;
    fastSetPix(x, y, red, green, blue);
}

inline
void BaseSurface::fastGetPix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b) const
{
#ifndef OPTIMIZE_SCREEN_WIDTH_640
    uint16_t pixel = *((uint16_t*)data->pixels + ((y * data->pitch) >> 1) + x);
#endif
#ifdef OPTIMIZE_SCREEN_WIDTH_640
    uint16_t pixel = *((uint16_t*)data->pixels + y * 640 + x);
#endif

    uint8_t tmp;
    tmp = (pixel & 63488) >> 11;
    *r = (uint8_t)(tmp << 3) + (tmp >> 5);
    tmp = (pixel & 2016) >> 5;
    *g = (uint8_t)(tmp << 2) + (tmp >> 5);
    tmp = (pixel & 31) >> 0;
    *b = (uint8_t)(tmp << 3) + (tmp >> 5);
}

inline
void BaseSurface::fastGetPix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) const
{
#ifndef OPTIMIZE_SCREEN_WIDTH_640
    Uint32 pixel = *((Uint32*)data->pixels + ((y * data->pitch) >> 1) + x);
#endif
#ifdef OPTIMIZE_SCREEN_WIDTH_640
    Uint32 pixel = *((Uint32*)data->pixels + y * 640 + x);
#endif
    SDL_GetRGBA(pixel, data->format, r, g, b, a);
}
#endif // INLINE_FASTPIX

inline
uint8_t BaseSurface::setAlpha(uint8_t value) 
{
    SDL_SetAlpha(data, SDL_SRCALPHA, value);
    return (m_alpha = value);
}

inline
uint8_t BaseSurface::getAlpha() const
{
    return m_alpha;
}

inline
int BaseSurface::getWidth() const
{
    return data->w;
}

inline
int BaseSurface::getHeight() const
{
    return data->h;
}

inline
int BaseSurface::getDepth() const   
{
    return data->format->BitsPerPixel;
}
} // namespace sdlc
#endif // SDLC_BASE_SURFACE_H
