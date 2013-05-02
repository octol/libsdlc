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

#ifndef SDLC_FONT_H
#define SDLC_FONT_H

#include <string>
#include <memory>
#include <unordered_map>

namespace sdlc {

const int FONT_GRID_W = 29;
const int FONT_GRID_H = 3;
const int FONT_GRID_LENGTH = FONT_GRID_W*FONT_GRID_H;

class Surface;

class Font {
public:
    Font();
    explicit Font(std::string path);

    void load(std::string path);
    Surface* get_char(char c) const;
    Surface* operator[](char c) const;

private:
    typedef std::shared_ptr<sdlc::Surface> SharedSurface;
    typedef std::array<SharedSurface,FONT_GRID_LENGTH> CharArray;
    typedef std::unordered_map<char,int> CharMap;

    CharArray& allocate_array(CharArray&, int width, int height) const;
    CharArray& copy_array(CharArray&, const sdlc::Surface&, int width, int height) const;
    CharMap set_char_hash() const;
    unsigned int map(int i, int j) const;

    CharArray gfx_;
    CharMap loc_;
};
} // namespace sdlc
#endif // SDLC_FONT_H
