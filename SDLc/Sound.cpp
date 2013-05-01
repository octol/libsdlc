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

Sound::Sound() : ref_count_(new std::size_t(1))
{
}

Sound::Sound(std::string path) : Sound()
{
    unchecked_load(path);
}

// Copy
Sound::Sound(const Sound& sound) 
    : sound_(sound.sound_),
      ref_count_(sound.ref_count_)
{
    ++(*ref_count_); 
}

// Move
Sound::Sound(Sound&& sound) 
    : sound_(sound.sound_),
      ref_count_(sound.ref_count_)
{
    sound.sound_ = nullptr;
    sound.ref_count_ = nullptr;
    sound.ref_count_ = new std::size_t(1);
}

// Assignment
Sound& Sound::operator=(const Sound& rhs)
{
    if (this != &rhs) {
        if (--(*ref_count_) == 0) {
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
        if (--(*ref_count_) == 0) {
            Mix_FreeChunk(sound_);
            delete ref_count_;
        }

        sound_ = rhs.sound_;
        ref_count_ = rhs.ref_count_;

        rhs.sound_ = nullptr;
        rhs.ref_count_ = nullptr;
        rhs.ref_count_ = new std::size_t(1);
    }
    return *this;
}

Sound::~Sound()
{
    if (--(*ref_count_) == 0) {
        Mix_FreeChunk(sound_);
        delete ref_count_;
    }
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Sound::load(std::string path)
{
    reset();
    unchecked_load(path);
}

void Sound::reset()
{
    if (--(*ref_count_) == 0) {
        Mix_FreeChunk(sound_);
        delete ref_count_;
    }

    // Restore plain constructor state.
    sound_ = nullptr;
    ref_count_ = nullptr;
    ref_count_ = new std::size_t(1);
}

void Sound::play(int iterations)
{
    Mix_PlayChannel(channel_, sound_, iterations);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void Sound::unchecked_load(std::string path)
{
    sound_ = Mix_LoadWAV(path.c_str());
    if (sound_ == NULL) 
        throw std::runtime_error(SDL_GetError());
}
} // namespace sdlc
