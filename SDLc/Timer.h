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

#ifndef SDLC_TIMER_H
#define SDLC_TIMER_H

#include "SDL.h"

namespace sdlc {

class Timer {
public:
    Timer();
    virtual ~Timer();

    void close();

    // Updates the fps and frametime.
    void update(uint32_t frame_time_wait);
    void update();

    void reset_timer();

    // Get the number of ticks (ms) since init.
    virtual uint32_t ticks() const;

    void delay(uint32_t ms);

    float fps();
    float frame_time() const;

private:
    int open();

    // Keeps track of the frame rate timing.
    //int frame = 0;
    uint32_t start_ticks_ = 0;
    uint32_t current_ticks_ = 0;

    // These gets updated from this info, not necessarily every frame.
    float fps_ = 0;
    uint32_t frame_ticks_ = 0;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
float Timer::fps()
{
    return fps_;
}

inline
float Timer::frame_time() const 
{
    return static_cast<float>(frame_ticks_) / 1000.0f;
}
} // namespace sdlc
#endif // SDLC_TIMER_H
