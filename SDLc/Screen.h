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

#ifndef SDLC_SCREEN_H
#define SDLC_SCREEN_H

#include <cassert>
#include <array>
#include "BaseSurface.h"

namespace sdlc {

int init();
int init(uint32_t flags);
void quit();

class Screen final : public BaseSurface {
    friend class Surface;

public:
    Screen();
    Screen(int w, int h, int bpp, int type);
    virtual ~Screen();

    void open(int w, int h, int bpp, int type);
    void close();

    int show_cursor(bool toggle);
    void set_caption(std::string title);

    // Use either update_area() + flip() or just flip_all()
    void update_area(int x, int y, int w, int h);
    void flip();
    void flip_all();

private:
    // data needed by update_area() and flip().
    int update_i_ = 0;
    std::array<SDL_Rect,256> update_r_;

    // internal function to wrap SDL_GetVideoSurface()
    static SDL_Surface* video_surface();
};

std::ostream& operator<<(std::ostream&, const Screen&);

} // namespace sdlc
#endif // SDLC_SCREEN_H
