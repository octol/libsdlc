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

#ifndef SDLC_PARTICLE_H
#define SDLC_PARTICLE_H

#include "Misc.h"

namespace sdlc {

class Timer;

class Particle {
public:
    Particle() = default;
    Particle(float x, float y, float x_vel, float y_vel, 
             int red, int green, int blue, float alpha);

    void update(const Timer& timer);

    float x() const;
    float y() const;
    float set_x(float value);
    float set_y(float value);
    float x_vel() const;
    float y_vel() const;
    float set_x_vel(float value);
    float set_y_vel(float value);

    float alpha() const;
    float set_alpha(float value);

    float fade_speed() const;
    float set_fade_speed(float value);
    int r() const;
    int g() const;
    int b() const;
    int set_r(int value);
    int set_b(int value);
    int set_g(int value);

    bool inside(float x1, float x2, float y1, float y2) const;

    // This is useful if the particles exist in preallocated arrays
    bool active() const;
    bool active(bool value);

private:
    bool active_ = false;

    // position
    float x_ = 0;
    float y_ = 0;
    float x_vel_ = 0;
    float y_vel_ = 0;

    // color
    float fade_speed_ = 0;
    int r_ = 0;
    int g_ = 0;
    int b_ = 0;
    float alpha_ = 255;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
float Particle::x() const
{
    return x_;
}

inline
float Particle::y() const
{
    return y_;
}

inline
float Particle::set_x(float value)
{
    return x_ = value;
}

inline
float Particle::set_y(float value)
{
    return y_ = value;
}

inline
float Particle::x_vel() const
{
    return x_vel_;
}

inline
float Particle::y_vel() const
{
    return y_vel_;
}

inline
float Particle::set_x_vel(float value)
{
    return x_vel_ = value;
}

inline
float Particle::set_y_vel(float value)
{
    return y_vel_ = value;
}

inline
float Particle::alpha() const
{
    return alpha_;
}

inline
float Particle::set_alpha(float value)
{
    return alpha_ = bound(value, 0.0f, 255.0f);
}

inline
float Particle::fade_speed() const
{
    return fade_speed_;
}

inline
float Particle::set_fade_speed(float value)
{
    return fade_speed_ = value;
}

inline
int Particle::r() const
{
    return r_;
}

inline
int Particle::g() const
{
    return g_;
}

inline
int Particle::b() const 
{
    return b_;
}

inline
int Particle::set_r(int value)
{
    return r_ = bound(value, 0, 255);
}

inline
int Particle::set_g(int value)
{
    return g_ = bound(value, 0, 255);
}

inline
int Particle::set_b(int value)
{
    return b_ = bound(value, 0, 255);
}

inline
bool Particle::active() const
{
    return active_;
}

inline
bool Particle::active(bool value)
{
    return active_ = value;
}
} // namespace sdlc
#endif // SDLC_PARTICLE_H
