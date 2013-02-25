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

#include "BaseSurface.h"

class Screen : public BaseSurface {
public:
    virtual ~Screen();

    void init(int w, int h, int bpp, int type);
    void close();
    void printVideoInfo();
    int show_cursor(bool toggle);
    void set_caption(std::string title);

    // Use either UpdateArea() + Flip() or just FlipAll()
    void updateArea(int x, int y, int w, int h);
    void flip();
    void flipAll();

private:
    int depth = 0;
    int updateI = 0;            // used by UpdateArea() and Flip()
    SDL_Rect updateR[256];      // used by UpdateArea() and Flip()
};
#endif // SDLC_SCREEN_H
