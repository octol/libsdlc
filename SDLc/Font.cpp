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
}

Font::Font(std::string path) : Font()
{
    load(path);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Font::load(std::string path)
{
    Surface src;
    src.load_raw(path);
    int w = ((src.width() - 25) / 29);
    int h = ((src.height() - 2) / 3);

    // Create the surfaces for each character and then copy the actual
    // characters.
    gfx_ = copy_array(allocate_array(gfx_, w, h), src, w, h);

    // Setup hash map
    loc_ = set_char_hash();
}

Surface* Font::get_char(char c) const
{
    return gfx_[loc_.at(c)].get();
}

Surface* Font::operator[](char c) const
{
    return get_char(c);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

Font::CharArray& Font::allocate_array(CharArray& char_array, int width, int height) const
{
    for (auto& c : char_array) {
        c = std::make_shared<sdlc::Surface>(width,height);
        c->set_color_key();
    }
    return char_array;
}

Font::CharArray& Font::copy_array(CharArray& char_array, const sdlc::Surface& src, int width, int height) const
{
    for (int j = 0; j < FONT_GRID_H; j++) {
        for (int i = 0; i < FONT_GRID_W; i++) {
            SDL_Rect src_rect;
            src_rect.x = (int16_t)(i * (width + 1));
            src_rect.y = (int16_t)(j * (height + 1));
            src_rect.w = (int16_t)width;
            src_rect.h = (int16_t)height;
            char_array.at(map(i,j))->blit(0, 0, src, src_rect);
        }
    }
    return char_array;
}

Font::CharMap Font::set_char_hash() const
{
    CharMap loc;

    for (char c = 'a'; c <= 'z'; ++c) 
        loc[c] = map((int)c - (int)'a', 0);
    for (char c = 'A'; c <= 'A'; ++c)
        loc[c] = map((int)c - (int)'A', 0);
    for (char c = '0'; c <= '9'; ++c)
        loc[c] = map((int)c - (int)'0', 1);

    std::string misc = ".,!?:=() ";
    for (std::string::size_type i = 0; i != misc.size(); ++i)
        loc[misc[i]] = map((int)i, 2);

    return std::move(loc);
}

unsigned int Font::map(int i, int j) const
{
    return i + j*FONT_GRID_W;
}

} // namespace sdlc


