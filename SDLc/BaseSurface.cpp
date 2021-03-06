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
#include "Font.h"
#include "Sprite.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Member functions
// -----------------------------------------------------------------------------

void BaseSurface::blit(int x, int y, const SDL_Surface* src, const SDL_Rect rect)
{
    SDL_Rect dst_rect;

    dst_rect.x = static_cast<int16_t>(x);
    dst_rect.y = static_cast<int16_t>(y);
    dst_rect.w = rect.w;
    dst_rect.h = rect.h;

    if (SDL_BlitSurface(const_cast<SDL_Surface*>(src), const_cast<SDL_Rect*>(&rect), data, &dst_rect) != 0) 
        throw std::runtime_error(SDL_GetError());
}

void BaseSurface::blit(int x, int y, const SDL_Surface* src)
{
    SDL_Rect dst_rect;

    dst_rect.x = static_cast<int16_t>(x);
    dst_rect.y = static_cast<int16_t>(y);
    dst_rect.w = static_cast<uint16_t>(data->w);
    dst_rect.h = static_cast<uint16_t>(data->h);

    if (SDL_BlitSurface(const_cast<SDL_Surface*>(src), NULL, data, &dst_rect) != 0) 
        throw std::runtime_error(SDL_GetError());
}

void BaseSurface::blit(const Sprite& sprite)
{
    SDL_Rect src_rect;

    src_rect.x = static_cast<int16_t>((sprite.current_frame_ - 1) 
                                      * sprite.width());
    src_rect.y = 0;
    src_rect.w = static_cast<uint16_t>(sprite.width());
    src_rect.h = static_cast<uint16_t>(sprite.height());

    blit((int)sprite.x(), (int)sprite.y(), sprite.data, src_rect);
}

void BaseSurface::set_pix(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t color = SDL_MapRGB(data->format, r, g, b);
    if (data->format->BitsPerPixel > 16) {
        *((uint32_t*)data->pixels + ((y * data->pitch) >> 2) + x) = color;
    } else {
        *((uint16_t*)data->pixels + ((y * data->pitch) >> 1) + x) = (uint16_t)color;
    }
}

void BaseSurface::set_pix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    uint32_t color = SDL_MapRGBA(data->format, r, g, b, a);
    *((uint32_t*)data->pixels + ((y * data->pitch) >> 2) + x) = color;
}

#ifndef INLINE_FASTPIX
void BaseSurface::fast_set_pix(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t color = SDL_MapRGB(data->format, r, g, b);
#ifndef OPTIMIZE_SCREEN_WIDTH_640
//  *((uint16_t *)data->pixels + y * data->w + x) = color;
    *((uint16_t*)data->pixels + ((y * data->pitch) >> 1) + x) = color;
#endif
#ifdef OPTIMIZE_SCREEN_WIDTH_640
    *((uint16_t*)data->pixels + y * 640 + x) = color;
#endif
}
#endif

#ifndef INLINE_FASTPIX
void BaseSurface::fast_set_pix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    uint32_t color = SDL_MapRGBA(data->format, r, g, b, a);
#ifndef OPTIMIZE_SCREEN_WIDTH_640
    *((uint32_t*)data->pixels + ((y * data->pitch) >> 2) + x) = color;
#endif
#ifdef OPTIMIZE_SCREEN_WIDTH_640
    *((uint32_t*)data->pixels + y * 640 + x) = color;
#endif
}
#endif

void BaseSurface::blend_pix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    // new color = ((A/255) * color) + (((255-A)/255) * old color)
    uint8_t red, green, blue;
    get_pix(x, y, &red, &green, &blue);

    red = (uint8_t)(((a * (r - red)) >> 8) + red);
    green = (uint8_t)(((a * (g - green)) >> 8) + green);
    blue = (uint8_t)(((a * (b - blue)) >> 8) + blue);

    set_pix(x, y, red, green, blue);
}

#ifndef INLINE_FASTPIX
void BaseSurface::fast_blend_pix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    // new color = ((A/255) * color) + (((255-A)/255) * old color)
    uint8_t red = 0, green = 0, blue = 0;
    fast_get_pix(x, y, &red, &green, &blue);

    red = ((a * (r - red)) >> 8) + red;
    green = ((a * (g - green)) >> 8) + green;
    blue = ((a * (b - blue)) >> 8) + blue;

    fast_set_pix(x, y, red, green, blue);
}
#endif

void BaseSurface::get_pix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b) const
{
    uint32_t pixel;
    if (data->format->BitsPerPixel > 16) {
        pixel = *((uint32_t*)data->pixels + ((y * data->pitch) >> 2) + x);
    } else { 
        pixel = *((uint16_t*)data->pixels + ((y * data->pitch) >> 1) + x);
    }
    SDL_GetRGB(pixel, data->format, r, g, b);
}

void BaseSurface::get_pix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) const
{
    uint32_t pixel = *((uint32_t*)data->pixels + ((y * data->pitch) >> 2) + x);
    SDL_GetRGBA(pixel, data->format, r, g, b, a);
}

#ifndef INLINE_FASTPIX
void BaseSurface::fast_get_pix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b) const
{
#ifndef OPTIMIZE_SCREEN_WIDTH_640
//  uint16_t pixel = *((uint16_t*)data->pixels + y * data->w + x);
    uint16_t pixel = *((uint16_t*)data->pixels + ((y * data->pitch) >> 1) + x);
#endif
#ifdef OPTIMIZE_SCREEN_WIDTH_640
    uint16_t pixel = *((uint16_t*)data->pixels + y * 640 + x);
#endif

    unsigned tmp;
    tmp = (pixel & 63488) >> 11;
    *r = (tmp << 3) + (tmp >> 5);
    tmp = (pixel & 2016) >> 5;
    *g = (tmp << 2) + (tmp >> 5);
    tmp = (pixel & 31) >> 0;
    *b = (tmp << 3) + (tmp >> 5);
}
#endif

#ifndef INLINE_FASTPIX
void BaseSurface::fast_get_pix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) const
{
#ifndef OPTIMIZE_SCREEN_WIDTH_640
    uint32_t pixel = *((uint32_t*)data->pixels + ((y * data->pitch) >> 1) + x);
#endif
#ifdef OPTIMIZE_SCREEN_WIDTH_640
    uint32_t pixel = *((uint32_t*)data->pixels + y * 640 + x);
#endif
    SDL_GetRGBA(pixel, data->format, r, g, b, a);
}
#endif

void BaseSurface::line(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b)
{
    int X1, Y1, X2, Y2, dx, dy, R;
    float m = (float)(y2 - y1) / (float)(x2 - x1);

    if ((x1 < x2) || (y1 < y2)) { 
        X1 = x1; Y1 = y1; X2 = x2; Y2 = y2; 
    } else { 
        X1 = x2; Y1 = y2; X2 = x1; Y2 = y1; 
    }
    dx = X2 - X1; 
    dy = Y2 - Y1;
    set_pix(X1, Y1, r, g, b);

    if (m > -1 && m < 1) {
        R = ((dx - dy) / 2);

        while (X1 < X2) {
            X1++; 
            R = (m >= 0) ? R + dy : R - dy;
            if (R >= dx) {
                Y1 = (m >=0) ? Y1 + 1 : Y1 - 1;
                R -= dx;
            }
            set_pix(X1, Y1, r, g, b);
        }
    }
    else if (m >= 1 || m <= -1) {
        R = -((dx - dy) / 2);

        while (Y1 < Y2) {
            Y1++;
            R = (m <= -1) ? R - dx : R + dx;
            if (R >= dy) {
                X1 = (m <= -1) ? X1 - 1 : X1 + 1;
                R -= dy;
            }
            set_pix(X1, Y1, r, g, b);
        }
    }
}

void BaseSurface::line_aa(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b)
{
    int X1, Y1, X2, Y2, dx, dy, R;
    float m = (float)(y2 - y1) / (float)(x2 - x1);
    uint8_t alpha;

    if ((x1 < x2) || (y1 < y2)) {
        X1 = x1; Y1 = y1; X2 = x2; Y2 = y2; 
    } else { 
        X1 = x2; Y1 = y2; X2 = x1; Y2 = y1; 
    }

    dx = X2 - X1; dy = Y2 - Y1;
    if (m > -1 && m < 1) {
        R = ((dx - dy) / 2);

        alpha = (uint8_t)(((float)R / (float)dx) * 255);
        if (m >= 0) {
            blend_pix(X1, Y1, r, g, b, alpha);
            blend_pix(X1, Y1 - 1, r, g, b, (uint8_t)(255 - alpha));
        } else {
            blend_pix(X1, Y1 - 1, r, g, b, alpha);
            blend_pix(X1, Y1, r, g, b, (uint8_t)(255 - alpha));
        }
        while (X1 < X2) {
            X1++; 
            R = (m >= 0) ? R + dy : R - dy;
            if (R >= dx) {
                Y1 = (m >= 0) ? Y1 + 1 : Y1 - 1;
                R -= dx;
            }
            alpha = (uint8_t)(((float)R / (float)dx) * 255);
            if (m >= 0) {
                blend_pix(X1, Y1, r, g, b, alpha);
                blend_pix(X1, Y1 - 1, r, g, b, (uint8_t)(255 - alpha));
            } else {
                blend_pix(X1, Y1 - 1, r, g, b, alpha);
                blend_pix(X1, Y1, r, g, b, (uint8_t)(255 - alpha));
            }
        }
    }
    else if (m >= 1 || m <= -1) {
        R = -((dx - dy) / 2);

        alpha = (uint8_t)(((float)R / (float)dy) * 255);
        if (m >= 1) {
            blend_pix(X1, Y1, r, g, b, alpha);
            blend_pix(X1 - 1, Y1, r, g, b, (uint8_t)(255 - alpha));
        } else {
            blend_pix(X1 - 1, Y1, r, g, b, alpha);
            blend_pix(X1, Y1, r, g, b, (uint8_t)(255 - alpha));
        }
        while (Y1 < Y2) {
            Y1++;
            R = (m <= -1) ? R - dx : R + dx;
            if (R >= dy) {
                X1 = (m <= -1) ? X1 - 1 : X1 + 1;
                R -= dy;
            }
            if (m >= 1) {
                blend_pix(X1, Y1, r, g, b, alpha);
                blend_pix(X1 - 1, Y1, r, g, b, (uint8_t)(255 - alpha));
            } else {
                blend_pix(X1 - 1, Y1, r, g, b, alpha);
                blend_pix(X1, Y1, r, g, b, (uint8_t)(255 - alpha));
            }
        }
    }
}

void BaseSurface::fill_rect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b)
{
    SDL_Rect tmp;

    tmp.x = static_cast<int16_t>(x);
    tmp.y = static_cast<int16_t>(y);
    tmp.w = static_cast<uint16_t>(w);
    tmp.h = static_cast<uint16_t>(h);

    SDL_FillRect(data, &tmp, SDL_MapRGB(data->format, r, g, b));
}

void BaseSurface::print(int x, int y, std::string text, uint8_t r, uint8_t g, uint8_t b)
{
    lock();
    uint32_t color = SDL_MapRGB(data->format, r, g, b);
    for (auto& c: text) {
       draw_char(x, y, c, color);
       x += 8;
    }
    unlock();
}

void BaseSurface::print(int x, int y, std::string text, Font& font)
{
    for (auto& c : text) {
        blit(x, y, *font[c]);
        x += font[c]->width();
    }
}

void BaseSurface::print(int x, int y, uint32_t value, uint8_t r, uint8_t g, uint8_t b)
{
    print(x, y, std::to_string(value), r, g, b);
}

void BaseSurface::print(int x, int y, uint32_t value, Font& font)
{
    print(x, y, std::to_string(value), font);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

// Returns a character used for fixed font printing when we don't want to
// load the font from a bitmap.
void BaseSurface::draw_char(int x, int y, char c, uint32_t color)
{
    extern unsigned char fontdata[8 * 256];

    int ix, iy, k;
    uint8_t* charpos;
    uint8_t bits[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    uint8_t* bitpos;
    uint8_t* curpos;
    int xx, yy;

    // Variable setup
    k = (uint8_t)c;
    k = k * 8;
    charpos = fontdata;
    charpos = charpos + k;

    // Drawing loop 
    curpos = (uint8_t*)data->pixels;
    xx = x;
    yy = y;
    for (iy = 0; iy < 8; iy++) {
        bitpos = bits;
        for (ix = 0; ix < 8; ix++) {
            if ((*charpos & *bitpos) == *bitpos) {
                if (data->format->BitsPerPixel > 16)
                    *((uint32_t*)data->pixels + yy * data->pitch / 4 + xx) = color;
                else
                    *((uint16_t*)data->pixels + yy * data->pitch / 2 + xx) = (uint16_t)color;
            }

            bitpos++;
            curpos += 4;
            xx++;
        }
        charpos++;
        yy++;
        xx -= 8;
    }
}

// -----------------------------------------------------------------------------
// Free functions
// -----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const BaseSurface& surface)
{
    os.setf(std::ios::hex, std::ios::basefield);
    os << "flags: " << (int)surface.data->flags << std::endl;
    os.unsetf(std::ios::hex);
    os << "data->w: " << surface.data->w << std::endl;
    os << "data->h: " << surface.data->h << std::endl;
    os << "data->pitch: " << surface.data->pitch << std::endl;
    os << "data->format->BitsPerPixel: " << surface.data->format->BitsPerPixel << std::endl;
    os << "data->format->BytesPerPixel: " << surface.data->format->BytesPerPixel;

    return os;
}

} // namespace sdlc
