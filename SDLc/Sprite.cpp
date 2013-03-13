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
#include "Screen.h"
#include "Sprite.h"

namespace sdlc {

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Sprite::update(const Timer& timer)
{
    update(timer.frame_time());
}

void Sprite::update(float frametime)
{
    // update position
    setX(getX() + (getXVel() * frametime));
    setY(getY() + (getYVel() * frametime));

    // update animation
    if (animTicks == 0)     // first frame
        animTicks = SDL_GetTicks();

    else if (SDL_GetTicks() - animTicks > animSpeed) {
        currentFrame++;
        if (currentFrame > totalFrames) {
            currentFrame = 1;
            currentIteration++;

            if (currentIteration >= totalIterations) {
                m_animationActive = false;
                currentIteration = 0;
                animTicks = 0;
            }
        }
        animTicks = SDL_GetTicks();
    }
}

void Sprite::initAnimation(int speed, int frames, int iterations)
{
    animSpeed = speed;
    totalFrames = frames;
    totalIterations = iterations;

    setWidth(data->w / totalFrames);
    m_animationActive = true;
}

void Sprite::setCurrentAnimFrame(int num)
{
    currentFrame = num;
}

void Sprite::resetAnimTimer()
{
    animTicks = SDL_GetTicks();
}

bool Sprite::animationActive() const
{
    return m_animationActive;
}

float Sprite::setX(float value)
{
    SDL_Surface* screen = SDL_GetVideoSurface();
    if (lockedToScreen()) {
        if (value > (screen->w - getWidth())) {
            value = (float)(screen->w - getWidth());
        } else if (value < 0) {
            value = 0;
        }
    }
    return (m_x = value);
}

float Sprite::setY(float value)
{
    SDL_Surface* screen = SDL_GetVideoSurface();
    if (lockedToScreen()) {
        if (value > (screen->h - getHeight())) {
            value = (float)(screen->h - getHeight());
        } else if (value < 0) {
            value = 0;
        }
    }
    return (m_y = value);
}

SDL_Rect Sprite::getRect() const
{
    SDL_Rect rect;

    rect.x = (int16_t)getX();
    rect.y = (int16_t)getY();
    rect.w = (uint16_t)getWidth();
    rect.h = (uint16_t)getHeight();

    return rect;
}

SDL_Rect Sprite::getReducedRect() const
{
    SDL_Rect rect = getRect();

    rect.x = rect.x + (int16_t)((float)rect.w * 0.25f);
    rect.y = rect.y + (int16_t)((float)rect.h * 0.25f);
    rect.w = (int16_t)(rect.w * 0.5f);
    rect.h = (int16_t)(rect.h * 0.5f);

    return rect;
}

bool Sprite::overlap(const Sprite& other)
{
    const SDL_Rect rect1 = getRect();
    const SDL_Rect rect2 = other.getRect();

    if (overlap(rect1, rect2))
        return true;
    else return false;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

bool Sprite::overlap(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
    if ((rect1.x + rect1.w) > rect2.x && rect1.x < (rect2.x + rect2.w) && 
        (rect1.y + rect1.h) > rect2.y && rect1.y < (rect2.y + rect2.h)) {
        return true;
    } else {
        return false;
    }
}
} // namespace sdlc
