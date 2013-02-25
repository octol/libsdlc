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
#include "Misc.h"
#include "Font.h"
#include "Sprite.h"

void BaseSurface::printSurfaceInfo() const
{
    printf("flags: %x\n", data->flags);
    printf("data->w: %d\n", data->w);
    printf("data->h: %d\n", data->h);
    printf("data->pitch: %d\n", data->pitch);
    printf("data->format->BitsPerPixel: %d\n", data->format->BitsPerPixel);
    printf("data->format->BytesPerPixel: %d\n", data->format->BytesPerPixel);
}

void BaseSurface::blit(int x, int y, SDL_Surface* src, SDL_Rect rect)
{
    //SDL_Rect dst_rect = { x, y, rect.w, rect.h };
    SDL_Rect dst_rect;

    dst_rect.x = static_cast<int16_t>(x);
    dst_rect.y = static_cast<int16_t>(y);
    dst_rect.w = rect.w;
    dst_rect.h = rect.h;

    if (SDL_BlitSurface(src, &rect, data, &dst_rect) != 0) {
        std::cerr << "Error: couldn't blit surface to screen: "
                  << SDL_GetError() << std::endl;
    }
}

void BaseSurface::blit(int x, int y, SDL_Surface* src)
{
    SDL_Rect dst_rect;

    dst_rect.x = static_cast<int16_t>(x);
    dst_rect.y = static_cast<int16_t>(y);
    dst_rect.w = static_cast<uint16_t>(data->w);
    dst_rect.h = static_cast<uint16_t>(data->h);

    if (SDL_BlitSurface(src, NULL, data, &dst_rect) != 0) {
        std::cerr << "Error: couldn't blit surface to screen: "
                  << SDL_GetError() << std::endl;
    }
}

void BaseSurface::blit(Sprite& sprite)
{
    SDL_Rect src_rect;

    src_rect.x = static_cast<int16_t>((sprite.currentFrame - 1) 
                                    * sprite.getWidth());
    src_rect.y = 0;
    src_rect.w = static_cast<uint16_t>(sprite.getWidth());
    src_rect.h = static_cast<uint16_t>(sprite.getHeight());

    blit((int)sprite.getX(), (int)sprite.getY(), sprite.data, src_rect);
}

void BaseSurface::setPix(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    Uint32 color = SDL_MapRGB(data->format, r, g, b);
    if (data->format->BitsPerPixel > 16)
//      *((Uint32 *)data->pixels + y * data->pitch / 4 + x) = color;
        *((Uint32*)data->pixels + ((y * data->pitch) >> 2) + x) = color;
//  else *((uint16_t *)data->pixels + y * data->pitch / 2 + x) = color;
    else *((uint16_t*)data->pixels + ((y * data->pitch) >> 1) + x) = color;
}

void BaseSurface::setPix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    Uint32 color = SDL_MapRGBA(data->format, r, g, b, a);
    *((Uint32*)data->pixels + ((y * data->pitch) >> 2) + x) = color;
}

#ifndef INLINE_FASTPIX
void BaseSurface::fastSetPix(int x, int y, uint8_t r, uint8_t g, uint8_t b)
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
#endif

void BaseSurface::blendPix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    // fÃ€rgny = ((A/255) * fÃ€rg) + (((255-A)/255) * fÃ€rggammal)
    uint8_t red, green, blue;
    getPix(x, y, &red, &green, &blue);

    red = ((a * (r - red)) >> 8) + red;
    green = ((a * (g - green)) >> 8) + green;
    blue = ((a * (b - blue)) >> 8) + blue;

    setPix(x, y, red, green, blue);
}

#ifndef INLINE_FASTPIX
void BaseSurface::fastBlendPix(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    // fÃ€rgny = ((A/255) * fÃ€rg) + (((255-A)/255) * fÃ€rggammal)
    uint8_t red = 0, green = 0, blue = 0;
    fastGetPix(x, y, &red, &green, &blue);

    red = ((a * (r - red)) >> 8) + red;
    green = ((a * (g - green)) >> 8) + green;
    blue = ((a * (b - blue)) >> 8) + blue;

    fastSetPix(x, y, red, green, blue);
}
#endif

void BaseSurface::getPix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b) const
{
    Uint32 pixel;
    if (data->format->BitsPerPixel > 16)
//      pixel = *((Uint32*)data->pixels + y * data->pitch / 4 + x);
        pixel = *((Uint32*)data->pixels + ((y * data->pitch) >> 2) + x);
//  else pixel = *((uint16_t*)data->pixels + y * data->pitch / 2 + x);
    else pixel = *((uint16_t*)data->pixels + ((y * data->pitch) >> 1) + x);
    SDL_GetRGB(pixel, data->format, r, g, b);
}

void BaseSurface::getPix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) const
{
    Uint32 pixel = *((Uint32*)data->pixels + ((y * data->pitch) >> 2) + x);
    SDL_GetRGBA(pixel, data->format, r, g, b, a);
}

#ifndef INLINE_FASTPIX
void BaseSurface::fastGetPix(int x, int y, uint8_t* r, uint8_t* g, uint8_t* b) const
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
#endif

// This implementation seems far too verbose. Need to make this more
// compact.
void BaseSurface::line(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b)
{
    int xPoint1, yPoint1, xPoint2, yPoint2;
    int dx, dy;
    int rValue;
    float m = (float)(y2 - y1) / (float)(x2 - x1);

    ////////////////////////////////////////////////////////////////////////////
    if (m >= 0 && m < 1) {
        if (x1 < x2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dx - dy) / 2);

        setPix(xPoint1, yPoint1, r, g, b);
        while (xPoint1 < xPoint2) {
            xPoint1++;
            rValue = rValue + dy;
            if (rValue >= dx) {
                rValue = rValue - dx;
                yPoint1 = yPoint1 + 1;
            }
            setPix(xPoint1, yPoint1, r, g, b);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m >= 1) {
        if (y1 < y2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dy - dx) / 2);

        setPix(xPoint1, yPoint1, r, g, b);
        while (yPoint1 < yPoint2) {
            yPoint1++;
            rValue = rValue + dx;
            if (rValue >= dy) {
                rValue = rValue - dy;
                xPoint1 = xPoint1 + 1;
            }
            setPix(xPoint1, yPoint1, r, g, b);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m < 0 && m > -1) {
        if (x1 < x2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dx - dy) / 2);

        setPix(xPoint1, yPoint1, r, g, b);
        while (xPoint1 < xPoint2) {
            xPoint1++;
            rValue = rValue - dy;
            if (rValue >= dx) {
                rValue = rValue - dx;
                yPoint1 = yPoint1 - 1;
            }
            setPix(xPoint1, yPoint1, r, g, b);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m <= -1) {
        if (y1 < y2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dy - dx) / 2);

        setPix(xPoint1, yPoint1, r, g, b);
        while (yPoint1 < yPoint2) {
            yPoint1++;
            rValue = rValue - dx;
            if (rValue >= dy) {
                rValue = rValue - dy;
                xPoint1 = xPoint1 - 1;
            }
            setPix(xPoint1, yPoint1, r, g, b);
        }
    }
}

// This implementation seems far too verbose. Need to make this more
// compact.
void BaseSurface::lineAA(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b)
{
    int xPoint1, yPoint1, xPoint2, yPoint2;
    int dx, dy;
    int rValue;
    uint8_t alpha;
    float m = (float)(y2 - y1) / (float)(x2 - x1);

    ////////////////////////////////////////////////////////////////////////////
    if (m >= 0 && m < 1) {
        if (x1 < x2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dx - dy) / 2);

        alpha = (uint8_t)(((float)rValue / (float)dx) * 255);
        blendPix(xPoint1, yPoint1, r, g, b, alpha);
        blendPix(xPoint1, yPoint1 - 1, r, g, b, 255 - alpha);
        while (xPoint1 < xPoint2) {
            xPoint1++;
            rValue = rValue + dy;
            if (rValue >= dx) {
                rValue = rValue - dx;
                yPoint1 = yPoint1 + 1;
            }
            alpha = (uint8_t)(((float)rValue / (float)dx) * 255);
            blendPix(xPoint1, yPoint1, r, g, b, alpha);
            blendPix(xPoint1, yPoint1 - 1, r, g, b, 255 - alpha);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m >= 1) {
        if (y1 < y2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dy - dx) / 2);

        alpha = (uint8_t)(((float)rValue / (float)dy) * 255);
        blendPix(xPoint1, yPoint1, r, g, b, alpha);
        blendPix(xPoint1 - 1, yPoint1, r, g, b, 255 - alpha);
        while (yPoint1 < yPoint2) {
            yPoint1++;
            rValue = rValue + dx;
            if (rValue >= dy) {
                rValue = rValue - dy;
                xPoint1 = xPoint1 + 1;
            }
            alpha = (uint8_t)(((float)rValue / (float)dy) * 255);
            blendPix(xPoint1, yPoint1, r, g, b, alpha);
            blendPix(xPoint1 - 1, yPoint1, r, g, b, 255 - alpha);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m < 0 && m > -1) {
        if (x1 < x2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dx - dy) / 2);

        alpha = (uint8_t)(((float)rValue / (float)dx) * 255);
        blendPix(xPoint1, yPoint1 - 1, r, g, b, alpha);
        blendPix(xPoint1, yPoint1, r, g, b, 255 - alpha);
        while (xPoint1 < xPoint2) {
            xPoint1++;
            rValue = rValue - dy;
            if (rValue >= dx) {
                rValue = rValue - dx;
                yPoint1 = yPoint1 - 1;
            }
            alpha = (uint8_t)(((float)rValue / (float)dx) * 255);
            blendPix(xPoint1, yPoint1 - 1, r, g, b, alpha);
            blendPix(xPoint1, yPoint1, r, g, b, 255 - alpha);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m <= -1) {
        if (y1 < y2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dy - dx) / 2);

        alpha = (uint8_t)(((float)rValue / (float)dy) * 255);
        blendPix(xPoint1 - 1, yPoint1, r, g, b, alpha);
        blendPix(xPoint1, yPoint1, r, g, b, 255 - alpha);
        while (yPoint1 < yPoint2) {
            yPoint1++;
            rValue = rValue - dx;
            if (rValue >= dy) {
                rValue = rValue - dy;
                xPoint1 = xPoint1 - 1;
            }
            alpha = (uint8_t)(((float)rValue / (float)dy) * 255);
            blendPix(xPoint1 - 1, yPoint1, r, g, b, alpha);
            blendPix(xPoint1, yPoint1, r, g, b, 255 - alpha);
        }
    }
}

// This implementation seems far too verbose. Need to make this more
// compact.
void BaseSurface::fastLine(int x1, int y1, int x2, int y2, \
                           uint8_t r, uint8_t g, uint8_t b)
{
    int xPoint1, yPoint1, xPoint2, yPoint2;
    int dx, dy;
    int rValue;
    float m = (float)(y2 - y1) / (float)(x2 - x1);

    ////////////////////////////////////////////////////////////////////////////
    if (m >= 0 && m < 1) {
        if (x1 < x2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dx - dy) / 2);

        fastSetPix(xPoint1, yPoint1, r, g, b);
        while (xPoint1 < xPoint2) {
            xPoint1++;
            rValue = rValue + dy;
            if (rValue >= dx) {
                rValue = rValue - dx;
                yPoint1 = yPoint1 + 1;
            }
            fastSetPix(xPoint1, yPoint1, r, g, b);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m >= 1) {
        if (y1 < y2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dy - dx) / 2);

        fastSetPix(xPoint1, yPoint1, r, g, b);
        while (yPoint1 < yPoint2) {
            yPoint1++;
            rValue = rValue + dx;
            if (rValue >= dy) {
                rValue = rValue - dy;
                xPoint1 = xPoint1 + 1;
            }
            fastSetPix(xPoint1, yPoint1, r, g, b);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m < 0 && m > -1) {
        if (x1 < x2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dx - dy) / 2);

        fastSetPix(xPoint1, yPoint1, r, g, b);
        while (xPoint1 < xPoint2) {
            xPoint1++;
            rValue = rValue - dy;
            if (rValue >= dx) {
                rValue = rValue - dx;
                yPoint1 = yPoint1 - 1;
            }
            fastSetPix(xPoint1, yPoint1, r, g, b);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m <= -1) {
        if (y1 < y2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dy - dx) / 2);

        fastSetPix(xPoint1, yPoint1, r, g, b);
        while (yPoint1 < yPoint2) {
            yPoint1++;
            rValue = rValue - dx;
            if (rValue >= dy) {
                rValue = rValue - dy;
                xPoint1 = xPoint1 - 1;
            }
            fastSetPix(xPoint1, yPoint1, r, g, b);
        }
    }
}

// This implementation seems far too verbose. Need to make this more
// compact.
void BaseSurface::fastLineAA(int x1, int y1, int x2, int y2, \
                             uint8_t r, uint8_t g, uint8_t b)
{
    int xPoint1, yPoint1, xPoint2, yPoint2;
    int dx, dy;
    int rValue;
    uint8_t alpha;
    float m = (float)(y2 - y1) / (float)(x2 - x1);

    ////////////////////////////////////////////////////////////////////////////
    if (m >= 0 && m < 1) {
        if (x1 < x2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dx - dy) / 2);

        alpha = (uint8_t)(((float)rValue / (float)dx) * 255);
        fastBlendPix(xPoint1, yPoint1, r, g, b, alpha);
        fastBlendPix(xPoint1, yPoint1 - 1, r, g, b, 255 - alpha);
        while (xPoint1 < xPoint2) {
            xPoint1++;
            rValue = rValue + dy;
            if (rValue >= dx) {
                rValue = rValue - dx;
                yPoint1 = yPoint1 + 1;
            }
            alpha = (uint8_t)(((float)rValue / (float)dx) * 255);
            fastBlendPix(xPoint1, yPoint1, r, g, b, alpha);
            fastBlendPix(xPoint1, yPoint1 - 1, r, g, b, 255 - alpha);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m >= 1) {
        if (y1 < y2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dy - dx) / 2);

        alpha = (uint8_t)(((float)rValue / (float)dy) * 255);
        fastBlendPix(xPoint1, yPoint1, r, g, b, alpha);
        fastBlendPix(xPoint1 - 1, yPoint1, r, g, b, 255 - alpha);
        while (yPoint1 < yPoint2) {
            yPoint1++;
            rValue = rValue + dx;
            if (rValue >= dy) {
                rValue = rValue - dy;
                xPoint1 = xPoint1 + 1;
            }
            alpha = (uint8_t)(((float)rValue / (float)dy) * 255);
            fastBlendPix(xPoint1, yPoint1, r, g, b, alpha);
            fastBlendPix(xPoint1 - 1, yPoint1, r, g, b, 255 - alpha);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m < 0 && m > -1) {
        if (x1 < x2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dx - dy) / 2);

        alpha = (uint8_t)(((float)rValue / (float)dx) * 255);
        fastBlendPix(xPoint1, yPoint1 - 1, r, g, b, alpha);
        fastBlendPix(xPoint1, yPoint1, r, g, b, 255 - alpha);
        while (xPoint1 < xPoint2) {
            xPoint1++;
            rValue = rValue - dy;
            if (rValue >= dx) {
                rValue = rValue - dx;
                yPoint1 = yPoint1 - 1;
            }
            alpha = (uint8_t)(((float)rValue / (float)dx) * 255);
            fastBlendPix(xPoint1, yPoint1 - 1, r, g, b, alpha);
            fastBlendPix(xPoint1, yPoint1, r, g, b, 255 - alpha);
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    if (m <= -1) {
        if (y1 < y2) {
            xPoint1 = x1;
            yPoint1 = y1;
            xPoint2 = x2;
            yPoint2 = y2;
        } else {
            xPoint1 = x2;
            yPoint1 = y2;
            xPoint2 = x1;
            yPoint2 = y1;
        }

        dx = xPoint2 - xPoint1;
        dy = yPoint2 - yPoint1;
        rValue = ((dy - dx) / 2);

        alpha = (uint8_t)(((float)rValue / (float)dy) * 255);
        fastBlendPix(xPoint1 - 1, yPoint1, r, g, b, alpha);
        fastBlendPix(xPoint1, yPoint1, r, g, b, 255 - alpha);
        while (yPoint1 < yPoint2) {
            yPoint1++;
            rValue = rValue - dx;
            if (rValue >= dy) {
                rValue = rValue - dy;
                xPoint1 = xPoint1 - 1;
            }
            alpha = (uint8_t)(((float)rValue / (float)dy) * 255);
            fastBlendPix(xPoint1 - 1, yPoint1, r, g, b, alpha);
            fastBlendPix(xPoint1, yPoint1, r, g, b, 255 - alpha);
        }
    }
}

void BaseSurface::fillRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b)
{
    SDL_Rect tmp;

    tmp.x = static_cast<int16_t>(x);
    tmp.y = static_cast<int16_t>(y);
    tmp.w = static_cast<uint16_t>(w);
    tmp.h = static_cast<uint16_t>(h);

    SDL_FillRect(data, &tmp, SDL_MapRGB(data->format, r, g, b));
}

void BaseSurface::print(int x, int y, const std::string text,
                        uint8_t r, uint8_t g, uint8_t b)
{
    lock();
    Uint32 color = SDL_MapRGB(data->format, r, g, b);
    //char* cursor = &text[0];
    const char* cursor = text.c_str();
    std::string::size_type length = text.length();

    for (size_t i = 0; i < length; i++) {
        drawChar(x, y, *cursor, color);
        cursor++;
        x += 8;
    }
    unlock();
}

void BaseSurface::print(int x, int y, std::string text, Font& font)
{
    char* cursor = &text[0];
    std::string::size_type length = text.length();

    for (size_t i = 0; i < length; i++) {
        blit(x, y, *font.getChar(*cursor));
        x += (font.getChar(*cursor))->getWidth();

        cursor++;
    }
}

void BaseSurface::print(int x, int y, Uint32 value, uint8_t r, uint8_t g, uint8_t b)
{
    print(x, y, std::to_string(value), r, g, b);
}

void BaseSurface::print(int x, int y, Uint32 value, Font& font)
{
    print(x, y, std::to_string(value), font);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void BaseSurface::drawChar(int x, int y, char c, Uint32 color)
{
    extern unsigned char fontdata[8 * 256];

    int ix, iy, k;
    uint8_t* charpos;
    uint8_t bits[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
    uint8_t* bitpos;
    uint8_t* curpos;
    int xTmp, yTmp;

    /* Variable setup */
    k = (uint8_t)c;
    k = k * 8;
    charpos = fontdata;
    charpos = charpos + k;

    /* Drawing loop */
    curpos = (uint8_t*)data->pixels;
    xTmp = x;
    yTmp = y;
    for (iy = 0; iy < 8; iy++) {
        bitpos = bits;
        for (ix = 0; ix < 8; ix++) {
            if ((*charpos & *bitpos) == *bitpos) {
                if (data->format->BitsPerPixel > 16)
                    *((Uint32*)data->pixels + yTmp * data->pitch / 4 + xTmp) 
                        = color;
                else
                    *((uint16_t*)data->pixels + yTmp * data->pitch / 2 + xTmp) 
                        = color;
            }

            bitpos++;
            curpos += 4;
            xTmp++;
        }
        charpos++;
        yTmp++;
        xTmp = xTmp - 8;
    }
}
