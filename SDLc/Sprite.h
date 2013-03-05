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

#ifndef SDLC_SPRITE_H
#define SDLC_SPRITE_H

#include "Surface.h"

namespace sdlc {

class Timer;

class Sprite : public Surface {
    friend class BaseSurface;

public:
    Sprite();
    virtual ~Sprite();

    // Main functions
    void update(const Timer& timer);
    void update(float frametime);

    // Sprite animation stuff
    void initAnimation(int speed, int frames, int iterations);
    void setCurrentAnimFrame(int num);  // set current frame
    void resetAnimTimer();              // reset the animation timer
    bool animationActive() const;

    // In the future:
    //int setAnimation(int which, int speed, int frame, int iterations);
    //int getAnimation();   // returns which animation is "active"
    //int setFrame(int which);
    //int getFrame();   // return which frame is "active"
    //bool animationActive();
    //void resetAnimTimer();

    // Sprite data
    std::string name = "Unknown sdlc::Sprite";
    float getX() const;
    float getY() const;
    float setX(float value);
    float setY(float value);
    void setPos(float x, float y);

    float getXVel();
    float setXVel(float value);
    float getYVel();
    float setYVel(float value);
    void  setVel(float x, float y);

    bool lockedToScreen();
    bool lockedToScreen(bool value);

    SDL_Rect getRect() const;
    SDL_Rect getReducedRect() const;

    // collision functions
    bool overlap(const Sprite& other);

protected:
    bool overlap(const SDL_Rect& rect1, const SDL_Rect& rect2);

private:
    // Animation data
    int totalFrames = 1;
    int currentFrame = 1;
    int totalIterations = 0; 
    int currentIteration = 0;
    unsigned int animSpeed = 0;
    unsigned int animTicks = 0;
    bool m_animationActive = false; // if the animation is running

    // Sprite data
    float m_x = 0; 
    float m_y = 0;
    float m_xVel = 0;
    float m_yVel = 0;
    bool m_lockedToScreen = false;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
float Sprite::getX() const 
{
    return m_x;
}

inline
float Sprite::getY() const 
{
    return m_y;
}

inline
void Sprite::setPos(float x, float y) 
{
    setX(x);
    setY(y);
}

inline
float Sprite::getXVel() 
{
    return m_xVel;
}

inline
float Sprite::setXVel(float value)  
{
    return m_xVel = value;
}

inline
float Sprite::getYVel()         
{
    return m_yVel;
}

inline
float Sprite::setYVel(float value)  
{
    return m_yVel = value;
}

inline
void Sprite::setVel(float x, float y)  
{
    setXVel(x);
    setYVel(y);
}

inline
bool Sprite::lockedToScreen()       
{
    return m_lockedToScreen;
}

inline
bool Sprite::lockedToScreen(bool value) 
{
    return m_lockedToScreen = value;
}
} // namespace sdlc
#endif // SDLC_SPRITE_H
