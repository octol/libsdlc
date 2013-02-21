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
    if (!initialised_)
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
    initialised_ = true;
    return result;
}

void Timer::close()
{
    SDL_QuitSubSystem(SDL_INIT_TIMER);
    initialised_ = false;
}

void Timer::update(uint32_t frame_time_wait)
{
    // Run the first time
    if (start_ticks_ == 0) {
        start_ticks_ = ticks();
        
    // The rest of the time
    } else {
        current_ticks_ = ticks();

        while (current_ticks_ - start_ticks_ < frame_time_wait) {
            current_ticks_ = ticks();
        }

        frame_ticks_ = current_ticks_ - start_ticks_;
        fps_ = (1.0f / ((float)frame_ticks_ / 1000.0f));
        start_ticks_ = current_ticks_;
    }
}

void Timer::update()
{
    update(0);
}

void Timer::reset_timer()
{
    start_ticks_ = ticks();
}

uint32_t Timer::ticks() const
{
    return SDL_GetTicks();
}

void Timer::delay(uint32_t ms)
{
    SDL_Delay(ms);
}
