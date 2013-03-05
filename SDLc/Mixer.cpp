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
#include "SDL.h"
#include "SDL_mixer.h"
#include "Misc.h"
#include "Mixer.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Mixer::Mixer()
{
    // Important that we do not initialise in the constructor.
}

Mixer::~Mixer()
{
    if (initialised)
        close();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

bool Mixer::init()
{
    if (sdlinited == false) {
        SDL_Init(SDL_INIT_AUDIO);
        atexit(SDL_Quit);
        sdlinited = true;
    } else {
        SDL_InitSubSystem(SDL_INIT_AUDIO);
    }

//  if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 
//                   MIX_DEFAULT_CHANNELS, 4096))
//  if(Mix_OpenAudio(11025, AUDIO_U8, 1, 512))
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024)) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        std::cerr << "Mixer::init(): " << SDL_GetError() << std::endl;
        return false;
    }
    initialised = true;
    return true;
}

void Mixer::close()
{
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

int Mixer::setMusicVolume(int value)
{
    musicVolume = value;
    if (musicVolume > 128)
        musicVolume = 128;
    else if (musicVolume < 0)
        musicVolume = 0;

    Mix_VolumeMusic(musicVolume);
    return musicVolume;
}

int Mixer::getMusicVolume() const
{
    return musicVolume;
}

int Mixer::setSoundVolume(int value)
{
    soundVolume = value;
    if (soundVolume > 128)
        soundVolume = 128;
    else if (soundVolume < 0)
        soundVolume = 0;

    Mix_Volume(-1, soundVolume);
    return soundVolume;
}

int Mixer::getSoundVolume() const
{
    return soundVolume;
}

int Mixer::fade_out_music(int ms)
{
    return Mix_FadeOutMusic(3000);
}
} // namespace sdlc

