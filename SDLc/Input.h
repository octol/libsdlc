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

#ifndef SDLC_INPUT_H
#define SDLC_INPUT_H

#include "SDL.h"

namespace sdlc {

enum AUTOFIRE_KEYSTATE { AUTOFIRE, NO_AUTOFIRE };

class Input {
public:
    void update();
    bool key_pressed(int key, AUTOFIRE_KEYSTATE autofire);
    bool mouse_button_pressed();

    // DEPRECATED
    bool keyPressed(int key, AUTOFIRE_KEYSTATE autofire);
    bool mouseButtonPressed();

private:
    SDL_Event event_;
    bool press_detected_ = false;
    unsigned char* key_autofire_ = nullptr;
};
} // namespace sdlc
#endif // SDLC_INPUT_H
