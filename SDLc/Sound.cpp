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
#include <stdexcept>
#include "Sound.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Sound::Sound() : ref_count_(new int(0))
{
}

Sound::Sound(const std::string path) : Sound()
{
    load(path);
}

// Copy
Sound::Sound(const Sound& sound) : Sound()
{
    if (*sound.ref_count_ > 0) {
        sound_ = sound.sound_;
        delete ref_count_;
        ref_count_ = sound.ref_count_;

        ++(*ref_count_); 
    }
}

// Move
Sound::Sound(Sound&& sound) : ref_count_(sound.ref_count_)
{
    sound_ = sound.sound_;

    sound.sound_ = nullptr;
    sound.ref_count_ = new int(0);
}

// Assignment
Sound& Sound::operator=(const Sound& rhs)
{
    if (this != &rhs && *rhs.ref_count_ > 0) {
        if (--(*ref_count_) <= 0) {
            Mix_FreeChunk(sound_);
            delete ref_count_;
        }

        sound_ = rhs.sound_;
        ref_count_ = rhs.ref_count_;

        ++(*ref_count_); 
    }
    return *this;
}

// Move assignment
Sound& Sound::operator=(Sound&& rhs)
{
    if (this != &rhs) {
        if (--(*ref_count_) <= 0) {
            Mix_FreeChunk(sound_);
            delete ref_count_;
        }

        sound_ = rhs.sound_;
        ref_count_ = rhs.ref_count_;

        rhs.sound_ = nullptr;
        rhs.ref_count_ = new int(0);
    }
    return *this;
}

Sound::~Sound()
{
    // Note that *ref_count_ can be < 0 if we initialise without Sound 
    // and then call reset.
    if (--(*ref_count_) <= 0) {
        // Last reference
        Mix_FreeChunk(sound_);
        delete ref_count_;
        sound_ = nullptr;
        ref_count_ = nullptr;
    }

    sound_ = nullptr;
    ref_count_ = nullptr;
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Sound::load(std::string path)
{
    reset();
    sound_ = Mix_LoadWAV(path.c_str());
    if (sound_ == NULL) 
        throw std::runtime_error(SDL_GetError());
}

void Sound::reset()
{
    // Note that *ref_count_ can be < 0 after decrementing if we initialise
    // without Sound and then call reset.
    if (--(*ref_count_) <= 0) {
        Mix_FreeChunk(sound_);
        delete ref_count_;
    }

    // Restore plain constructor state.
    sound_ = nullptr;
    ref_count_ = new int(0);
}

void Sound::play(int iterations)
{
    Mix_PlayChannel(channel_, sound_, iterations);
}
} // namespace sdlc
