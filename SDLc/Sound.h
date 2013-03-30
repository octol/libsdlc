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

#ifndef SDLC_SOUND_H
#define SDLC_SOUND_H

#include <string>
#include "SDL_mixer.h"

namespace sdlc {

class Sound final {
public:
    Sound() {};
    Sound(const Sound& sound) = delete;
    Sound& operator=(Sound& sound) = delete;
    ~Sound();

    void load(std::string path);
    void link(Sound* object);
    void unload();
    void play(int iterations);

    int channel() const;
    int set_channel(int value);
    
private:
    Mix_Chunk* sound_ = nullptr;
    int channel_ = -1;
    bool loaded_ = false;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
int Sound::channel() const
{
    return channel_;
}

inline
int Sound::set_channel(int value)
{
    return channel_ = value;
}
} // namespace sdlc
#endif // SDLC_SOUND_H
