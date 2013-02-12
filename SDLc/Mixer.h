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

class Mixer {
public:
    Mixer();  
    virtual ~Mixer();

    bool init();
    void close();

    int setMusicVolume(int value);
    int getMusicVolume() const;
    int setSoundVolume(int value);
    int getSoundVolume() const;

private:
    bool initialised = false;
    int musicVolume = 128;
    int soundVolume = 128;
};
#endif // SDLC_MIXER_H
