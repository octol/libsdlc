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

#include "Timer.h"
#include "Particle.h"

namespace sdlc {

Particle::Particle(float x, float y, float x_vel, float y_vel, 
                   int red, int green, int blue, float alpha) 
    : x_(x), y_(y), x_vel_(x_vel), y_vel_(y_vel),
      r_(red), g_(green), b_(blue), alpha_(alpha)
{
}

void Particle::update(const Timer& timer)
{
    // Update position
    set_x(x() + (x_vel()  * timer.frame_time()));
    set_y(y() + (y_vel()  * timer.frame_time()));

    // Update alpha
    set_alpha(alpha() - (fade_speed() * timer.frame_time()));
}

bool Particle::inside(float x1, float x2, float y1, float y2)
{
    return x() >= x1 && x() <= x2 && y() >= y1 && y() <= y2;
}

} // namespace sdlc
