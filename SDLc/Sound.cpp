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

Sound::~Sound()
{
    unload();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Sound::load(std::string path)
{
    sound_ = Mix_LoadWAV(path.c_str());
    if (sound_ == NULL) {
        std::cerr << "Sound::load() " << SDL_GetError() << std::endl;
    }
    loaded_ = true;
}

void Sound::link(Sound* object)
{
    sound_ = object->sound_;
}

void Sound::unload()
{
    if (loaded_) {
        Mix_FreeChunk(sound_);
        sound_ = nullptr;
    }
}

void Sound::play(int iterations)
{
    Mix_PlayChannel(channel_, sound_, iterations);
}
} // namespace sdlc
