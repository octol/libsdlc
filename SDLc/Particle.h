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
    float x = 0;
    float y = 0;
    float xVel = 0;
    float yVel = 0;
    float alpha = 255;
    float fadeSpeed = 0;
    int r = 0;
    int g = 0;
    int b = 0;
    bool m_active = false;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
float Particle::getX() const
{
    return x;
}

inline
float Particle::getY() const
{
    return y;
}

inline
float Particle::setX(float value)
{
    return (x = value);
}

inline
float Particle::setY(float value)
{
    return (y = value);
}

inline
float Particle::getXVel() const
{
    return xVel;
}

inline
float Particle::getYVel() const
{
    return yVel;
}

inline
float Particle::setXVel(float value)
{
    return (xVel = value);
}

inline
float Particle::setYVel(float value)
{
    return (yVel = value);
}

inline
float Particle::getAlpha() const
{
    return alpha;
}

inline
float Particle::setAlpha(float value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return (alpha = value);
}

inline
float Particle::getFadeSpeed() const
{
    return fadeSpeed;
}

inline
float Particle::setFadeSpeed(float value)
{
    return (fadeSpeed = value);
}

inline
int Particle::getR() const
{
    return r;
}

inline
int Particle::getG() const
{
    return g;
}

inline
int Particle::getB() const 
{
    return b;
}

inline
int Particle::setR(int value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return (r = value);
}

inline
int Particle::setB(int value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return (g = value);
}

inline
int Particle::setG(int value)
{
    if (value > 255)     value = 255;
    else if (value < 0)  value = 0;
    return (b = value);
}

inline
bool Particle::active() const
{
    return m_active;
}

inline
bool Particle::active(bool value)
{
    return (m_active = value);
}
} // namespace sdlc
#endif // SDLC_PARTICLE_H
