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

void Input::update()
{
    // for regular input
    pressDetected = false;
    while (SDL_PollEvent(&event))
        pressDetected = true;

    // for autofire
    keyAutofire = SDL_GetKeyState(NULL);
}

bool Input::keyPressed(int key, AUTOFIRE_KEYSTATE autofire)
{
    if (autofire == NO_AUTOFIRE) {
        if (event.type == SDL_KEYDOWN && pressDetected) {
            if (event.key.keysym.sym == key)
                return true;
            else return false;
        } else {
            return false;
        }

    } else {
        if (keyAutofire[key] == SDL_PRESSED)
            return true;
        else {
            return false;
        }
    }
}

bool Input::mouseButtonPressed()
{
    if (event.type == SDL_MOUSEBUTTONDOWN && pressDetected) {
        return true;
    }
    else {
        return false;
    }
}
