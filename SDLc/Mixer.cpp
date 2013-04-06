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
// Member Functions
// -----------------------------------------------------------------------------

int Mixer::init()
{
    return SDL_Init(SDL_INIT_AUDIO);
}

int Mixer::open()
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);

//  if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 
//                   MIX_DEFAULT_CHANNELS, 4096))
//  if(Mix_OpenAudio(11025, AUDIO_U8, 1, 512))

    int r = 0;
    if ((r = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024))) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        std::cerr << "Mixer::open(): " << SDL_GetError() << std::endl;
    } else {
        initialised_ = true;
    }

    return r;
}

void Mixer::close()
{
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    initialised_ = false;
}

void Mixer::quit()
{
    SDL_Quit();
}

int Mixer::set_music_volume(int value)
{
    music_volume_ = value;
    if (music_volume_ > 128)
        music_volume_ = 128;
    else if (music_volume_ < 0)
        music_volume_ = 0;

    Mix_VolumeMusic(music_volume_);
    return music_volume_;
}

int Mixer::music_volume() const
{
    return music_volume_;
}

int Mixer::set_sound_volume(int value)
{
    sound_volume_ = value;
    if (sound_volume_ > 128)
        sound_volume_ = 128;
    else if (sound_volume_ < 0)
        sound_volume_ = 0;

    Mix_Volume(-1, sound_volume_);
    return sound_volume_;
}

int Mixer::sound_volume() const
{
    return sound_volume_;
}

int Mixer::fade_out_music(int ms)
{
    //return Mix_FadeOutMusic(3000);
    return Mix_FadeOutMusic(ms);
}
} // namespace sdlc

