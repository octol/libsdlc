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

namespace sdlc {

class Surface;

class Font {
public:
    Font();
    Font(const Font& font) = delete;
    Font& operator=(const Font& font) = delete;
    virtual ~Font();

    void load(const std::string path);
    Surface* get_char(char c) const;

private:
    // TODO: use stdlib container?
    Surface* gfx_[29][3];
    Surface* blank_ = nullptr;
};
} // namespace sdlc
#endif // SDLC_FONT_H
