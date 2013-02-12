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

class Timer {
public:
    Timer();
    virtual ~Timer();

    int init();
    void close();

    // Updates the fps and frametime.
    void update(int frame_time_wait);
    void update();

    // In case we want to limit the FPS. Should be called after Timer::update()
    //int limitFPS(float fps);

    void resetTimer();
    float getFPS();
    float getFrametime() const;

    // Get the number of ticks (ms) since init.
    virtual Uint32 getTicks() const;

private:
    bool initialised = false;

    // Keeps track of the frame rate timing.
    //int frame = 0;
    int start_ticks = 0;
    int current_ticks = 0;

    // These gets updated from this info, not necessarily every frame.
    float fps = 0;
    float frame_time = 0;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
float Timer::getFPS()
{
    return fps;
}

inline
float Timer::getFrametime() const 
{
    return (float)frame_time / 1000.0f;
}

#endif // SDLC_TIMER_H
