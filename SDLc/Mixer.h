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

#ifndef SDLC_MIXER_H
#define SDLC_MIXER_H

namespace sdlc {

class Mixer {
public:
    int init();
    int open();
    void close();
    void quit();

    int set_music_volume(int value);
    int music_volume() const;
    int set_sound_volume(int value);
    int sound_volume() const;

    int fade_out_music(int ms);

private:
    bool initialised_ = false;
    int music_volume_ = 128;
    int sound_volume_ = 128;
};
} // namespace sdlc
#endif // SDLC_MIXER_H
