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

#include "Input.h"

namespace sdlc {

int Input::update()
{
    // for regular input
    press_detected_ = false;
    int update_state;
    while ((update_state = SDL_PollEvent(&event_)))
        press_detected_ = true;

    // for autofire
    key_autofire_ = SDL_GetKeyState(NULL);

    return update_state;
}

bool Input::key_pressed(int key, AutofireKeystate autofire)
{
    if (autofire == AutofireKeystate::off) {
        if (event_.type == SDL_KEYDOWN && press_detected_) {
            if (event_.key.keysym.sym == key)
                return true;
            else return false;
        } else {
            return false;
        }
    } else {
        if (key_autofire_[key] == SDL_PRESSED)
            return true;
        else {
            return false;
        }
    }
}

bool Input::key_pressed(int key) 
{
    return key_pressed(key, AutofireKeystate::off);
}

bool Input::mouse_button_pressed()
{
    if (event_.type == SDL_MOUSEBUTTONDOWN && press_detected_) {
        return true;
    } else {
        return false;
    }
}
} // namespace sdlc
