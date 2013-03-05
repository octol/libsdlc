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
#include "Sound.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Sound::Sound()
{
}

Sound::~Sound()
{
    unload();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Sound::load(std::string path)
{
    sound = Mix_LoadWAV(path.c_str());
    if (sound == NULL) {
        std::cerr << "Sound::load() " << SDL_GetError() << std::endl;
    }
    loaded = true;
}

void Sound::link(Sound* object)
{
    sound = object->sound;
}

void Sound::unload()
{
    if (loaded) {
        Mix_FreeChunk(sound);
        sound = 0;
    }
}

void Sound::play(int iterations)
{
    Mix_PlayChannel(channel, sound, iterations);
}
} // namespace sdlc
