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

namespace sdlc {

class Timer;

class Particle {
public:
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

    // DEPRECATED
    float getX() const;
    float getY() const;
    float setX(float value);
    float setY(float value);
    float getXVel() const;
    float getYVel() const;
    float setXVel(float value);
    float setYVel(float value);

    float getAlpha() const;
    float setAlpha(float value);

    float getFadeSpeed() const;
    float setFadeSpeed(float value);
    int getR() const;
    int getG() const;
    int getB() const;
    int setR(int value);
    int setB(int value);
    int setG(int value);

    // This is because the particles are in preallocated arrays
    bool active() const;
    bool active(bool value);

private:
    float x_ = 0;
    float y_ = 0;
    float x_vel_ = 0;
    float y_vel_ = 0;
    float alpha_ = 255;
    float fade_speed_ = 0;
    int r_ = 0;
    int g_ = 0;
    int b_ = 0;
    bool active_ = false;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
float Particle::getX() const
{
    return x_;
}

inline
float Particle::getY() const
{
    return y_;
}

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
float Particle::setX(float value)
{
    return x_ = value;
}

inline
float Particle::setY(float value)
{
    return y_ = value;
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
float Particle::getXVel() const
{
    return x_vel_;
}

inline
float Particle::getYVel() const
{
    return y_vel_;
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
float Particle::setXVel(float value)
{
    return x_vel_ = value;
}

inline
float Particle::setYVel(float value)
{
    return y_vel_ = value;
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
float Particle::getAlpha() const
{
    return alpha_;
}

inline
float Particle::setAlpha(float value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return (alpha_ = value);
}

inline
float Particle::alpha() const
{
    return alpha_;
}

inline
float Particle::set_alpha(float value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return (alpha_ = value);
}

inline
float Particle::getFadeSpeed() const
{
    return fade_speed_;
}

inline
float Particle::setFadeSpeed(float value)
{
    return fade_speed_ = value;
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
int Particle::getR() const
{
    return r_;
}

inline
int Particle::getG() const
{
    return g_;
}

inline
int Particle::getB() const 
{
    return b_;
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
int Particle::setR(int value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return r_ = value;
}

inline
int Particle::setG(int value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return g_ = value;
}

inline
int Particle::setB(int value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return b_ = value;
}

inline
int Particle::set_r(int value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return r_ = value;
}

inline
int Particle::set_g(int value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return g_ = value;
}

inline
int Particle::set_b(int value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return b_ = value;
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
