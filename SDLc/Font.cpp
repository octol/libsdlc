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
#include "Surface.h"
#include "Font.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Font::Font()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 29; j++)
            gfx_[j][i] = nullptr;
}

Font::~Font()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 29; j++)
            delete gfx_[j][i];
    delete blank_;
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

    // set the rectangles around each font
    int i = 0, j = 0;
    SDL_Rect src_rect;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 29; j++) {
            src_rect.x = j * (width + 1);
            src_rect.y = i * (height + 1);
            src_rect.w = width;
            src_rect.h = height;

            gfx_[j][i] = new Surface;
            gfx_[j][i]->alloc(width, height);
            gfx_[j][i]->set_color_key();
            gfx_[j][i]->blit(0, 0, src, src_rect);
        }
    }

    blank_ = new Surface;
    blank_->alloc(width, height);
    blank_->set_color_key();
    blank_->fill_rect(0, 0, width, height, 255, 0, 255);
}

Surface* Font::get_char(char c) const
{
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
        return blank_;
    } else {
        std::cout << "warning: undefined char passed to Font::getChar()" 
                  << std::endl;
        return blank_;
    }
    return gfx_[i][j];
}
} // namespace sdlc


