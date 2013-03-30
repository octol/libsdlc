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

#ifndef SDLC_MUSIC_H
#define SDLC_MUSIC_H

#include <string>
#include "SDL_mixer.h"

namespace sdlc {

class Music {
public:
    Music();
    Music(const Music& music) = delete;
    Music& operator=(const Music& music) = delete;
    virtual ~Music();

    void load(const std::string path);
    void link(Music* object);
    void unload();
    void play(int iterations);

private:
    Mix_Music* music_ = nullptr;
    bool loaded_ = false;
};
} // namespace sdlc
#endif // SDLC_MUSIC_H
