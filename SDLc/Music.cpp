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
#include "Mixer.h"
#include "Music.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Music::Music() : ref_count_(new int(0))
{
}

Music::Music(const std::string path) : Music()
{
    load(path);
}

// Copy
Music::Music(const Music& music)
{
    if (*music.ref_count_> 0) {
        music_ = music.music_;
        ref_count_ = music.ref_count_;

        ++(*ref_count_); 
    }
}

// Move
Music::Music(Music&& music) : ref_count_(music.ref_count_)
{
    music_ = music.music_;

    music.music_ = nullptr;
    music.ref_count_ = new int(0);
}

// Assignment
Music& Music::operator=(const Music& rhs)
{
    if (this != &rhs && *rhs.ref_count_> 0) {
        if (--(*ref_count_) <= 0) {
            Mix_FreeMusic(music_);
            delete ref_count_;
        }

        music_ = rhs.music_;
        ref_count_ = rhs.ref_count_;

        ++(*ref_count_); 
    }
    return *this;
}

// Move assignment
Music& Music::operator=(Music&& rhs)
{
    if (this != &rhs) {
        if (--(*ref_count_) <= 0) {
            Mix_FreeMusic(music_);
            delete ref_count_;
        }

        music_ = rhs.music_;
        ref_count_ = rhs.ref_count_;

        rhs.music_ = nullptr;
        rhs.ref_count_ = new int(0);
    }
    return *this;
}

Music::~Music()
{
    //unload();
    
    // Note that *ref_count_ can be < 0 if we initialise without data
    // and then call unload.
    if (--(*ref_count_) <= 0) {
        // Last reference
        Mix_FreeMusic(music_);
        delete ref_count_;
        music_ = nullptr;
        ref_count_ = nullptr;
    }

    music_ = nullptr;
    ref_count_ = nullptr;
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Music::load(const std::string path)
{
    reset();
    music_ = Mix_LoadMUS(path.c_str());
    if (music_ == NULL) 
        throw std::runtime_error(SDL_GetError());
}

void Music::reset()
{
    // Note that *ref_count_ can be < 0 after decrementing if we initialise
    // without data and then call reset.
    if (--(*ref_count_) <= 0) {
        Mix_FreeMusic(music_);
        delete ref_count_;
    }

    // Restore plain constructor state.
    music_ = nullptr;
    ref_count_ = new int(0);
}

void Music::play(int iterations)
{
    Mix_PlayMusic(music_, iterations);
}
} // namespace sdlc
