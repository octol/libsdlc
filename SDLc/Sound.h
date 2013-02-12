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

class Sound {
public:
    Sound();
    Sound(const Sound& sound) = delete;
    Sound& operator=(Sound& sound) = delete;
    virtual ~Sound();

    void load(std::string path);
    void link(Sound* object);
    void unload();
    void play(int iterations);

    int getChannel() const;
    int setChannel(int value);

private:
    Mix_Chunk* sound = nullptr;
    int channel = -1;
    bool loaded = false;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
int Sound::getChannel() const
{
    return channel;
}

inline
int Sound::setChannel(int value)
{
    return (channel = value);
}

#endif // SDLC_SOUND_H
