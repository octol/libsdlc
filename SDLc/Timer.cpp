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

#include "SDL.h"
#include "Misc.h"
#include "Timer.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Timer::Timer()
{
}

Timer::~Timer()
{
    if (!initialised)
        close();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

int Timer::init()
{
    int result;

    if (sdlinited == false) {
        result = SDL_Init(SDL_INIT_TIMER);
        atexit(SDL_Quit);
        sdlinited = true;
        return result;
    } else {
        result = SDL_InitSubSystem(SDL_INIT_TIMER);
    }
    initialised = true;
    return result;
}

void Timer::close()
{
    SDL_QuitSubSystem(SDL_INIT_TIMER);
    initialised = false;
}

void Timer::update(int frame_time_wait)
{
    // Run the first time
    if (start_ticks == 0) {
        start_ticks = getTicks();
        
    // The rest of the time
    } else {
        current_ticks = getTicks();

        while (current_ticks - start_ticks < frame_time_wait) {
            current_ticks = getTicks();
        }

        frame_time = current_ticks - start_ticks;
        fps = (1.0f / ((float)frame_time / 1000.0f));
        start_ticks = current_ticks;
    }
}

void Timer::update()
{
    update(0);
}

void Timer::resetTimer()
{
    start_ticks = getTicks();
}

Uint32 Timer::getTicks() const
{
    return SDL_GetTicks();
}
