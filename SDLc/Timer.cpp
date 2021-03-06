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
#include "Timer.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction / Destruction
// -----------------------------------------------------------------------------

Timer::Timer()
{
    open();
}

Timer::~Timer()
{
    close();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Timer::close()
{
    if (SDL_WasInit(SDL_INIT_TIMER) == SDL_INIT_TIMER) 
        SDL_QuitSubSystem(SDL_INIT_TIMER);
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

int Timer::open()
{
    return SDL_InitSubSystem(SDL_INIT_TIMER);
}

} // namespace sdlc
