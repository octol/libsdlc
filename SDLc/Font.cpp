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
#include <stdexcept>
#include "Surface.h"
#include "Font.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Font::Font()
{
    for (auto& i : gfx_) 
        i = std::make_shared<sdlc::Surface>();
}

Font::Font(const std::string path) : Font()
{
    load(path);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Font::load(const std::string path)
{
    Surface src;
    src.load_raw(path);

    int width = ((src.width() - 25) / 29);
    int height = ((src.height() - 2) / 3);

    // Set the rectangles around each font
    // The two-dimensional loop is due to the way the format of the font
    // image.
    SDL_Rect src_rect;
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 29; i++) {
            src_rect.x = i * (width + 1);
            src_rect.y = j * (height + 1);
            src_rect.w = width;
            src_rect.h = height;

            gfx_.at(map(i,j))->alloc(width, height);
            gfx_.at(map(i,j))->set_color_key();
            gfx_.at(map(i,j))->blit(0, 0, src, src_rect);
        }
    }

    // Space is after the last character as it is now part of the font
    // image.
    gfx_.at(map(8,2))->alloc(width, height);
    gfx_.at(map(8,2))->set_color_key();
    gfx_.at(map(8,2))->fill_rect(0, 0, width, height, 255, 0, 255);
}

// TODO: This can be made more elegant
Surface* Font::get_char(char c) const
{
    // a-z :26
    // 0-9 :10
    // . , ! ? : = ( ) :8
    // blank
    // total: 45

    int i = 0;
    int j = 0;

    if (c >= '0' && c <= '9') {
        i = (int)c - (int)'0';
        j = 1;
    } else if (c >= 'a' && c <= 'z') {
        i = (int)c - (int)'a';
    } else if (c >= 'A' && c <= 'Z') {
        i = (int)c - (int)'A';
    //} else if (c == u'å' || c == u'Å') { 
    //    i = 26;
    //} else if (c == u'ä' || c == u'Ä') {
    //    i = 27;
    //} else if (c == u'ö' || c == u'Ö') {
    //    i = 28;
    } else if (c == '.') {
        i = 0; j = 2;
    } else if (c == ',') {
        i = 1; j = 2;
    } else if (c == '!') {
        i = 2; j = 2;
    } else if (c == '?') {
        i = 3; j = 2;
    } else if (c == ':') {
        i = 4; j = 2;
    } else if (c == '=') {
        i = 5; j = 2;
    } else if (c == '(') {
        i = 6; j = 2;
    } else if (c == ')') {
        i = 7; j = 2;
    } else if (c == ' ') {
        return gfx_.at(map(8,2)).get();
    } else {
        throw std::invalid_argument("Undefined character");
    }
    return gfx_[map(i,j)].get();
}

unsigned int Font::map(int i, int j) const
{
    int offset = 0;

    // alpha: j = 0
    if (j == 0) {
        offset += 0;
    // numeric: j = 1
    } else if (j == 1) {
        offset += 26;
    // other: j = 2
    } else if (j == 2) {
        offset += 26 + 10;
    } else {
        throw std::invalid_argument("Unexpected input");
    }

    return i + offset;
}

} // namespace sdlc


