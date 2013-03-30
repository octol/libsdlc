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
#include "Mixer.h"
#include "Music.h"

// TODO: in which namespace should mixer be?
extern sdlc::Mixer* mixer;

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Music::Music()
{
}

Music::~Music()
{
    unload();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Music::load(const std::string path)
{
    music_ = Mix_LoadMUS(path.c_str());
    if (music_ == NULL) {
        std::cerr << "Music::load() " << SDL_GetError() << std::endl;
    }

    loaded_ = true;
}

void Music::link(Music* object)
{
    music_ = object->music_;
}

void Music::unload()
{
    if (loaded_) {
        //Mix_HaltMusic();
        Mix_FreeMusic(music_);
        music_ = nullptr;
        loaded_ = false;
    }
}

void Music::play(int iterations)
{
    Mix_PlayMusic(music_, iterations);
    mixer->set_music_volume(mixer->music_volume());
}
} // namespace sdlc
