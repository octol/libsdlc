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

namespace sdlc {

class Surface;

class Font {
public:
    Font();
    explicit Font(std::string path);

    int load(const std::string path);
    Surface* get_char(char c) const;

private:
    unsigned int map(int i, int j) const;

    std::array<std::shared_ptr<sdlc::Surface>,256> gfx_;
};
} // namespace sdlc
#endif // SDLC_FONT_H
