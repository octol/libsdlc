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
    if (anim_ticks_ == 0)     // first frame
        anim_ticks_ = SDL_GetTicks();

    else if (SDL_GetTicks() - anim_ticks_ > anim_speed_) {
        current_frame_++;
        if (current_frame_ > total_frames_) {
            current_frame_ = 1;
            current_iteration_++;

            if (current_iteration_ >= total_iterations_) {
                animation_active_ = false;
                current_iteration_ = 0;
                anim_ticks_ = 0;
            }
        }
        anim_ticks_ = SDL_GetTicks();
    }
}

void Sprite::initAnimation(int speed, int frames, int iterations)
{
    anim_speed_ = speed;
    total_frames_ = frames;
    total_iterations_ = iterations;

    setWidth(data->w / total_frames_);
    animation_active_ = true;
}

void Sprite::init_animation(int speed, int frames, int iterations)
{
    anim_speed_ = speed;
    total_frames_ = frames;
    total_iterations_ = iterations;

    setWidth(data->w / total_frames_);
    animation_active_ = true;
}

void Sprite::setCurrentAnimFrame(int num)
{
    current_frame_ = num;
}

void Sprite::set_current_anim_frame(int num)
{
    current_frame_ = num;
}

void Sprite::resetAnimTimer()
{
    anim_ticks_ = SDL_GetTicks();
}

void Sprite::reset_anim_timer()
{
    anim_ticks_ = SDL_GetTicks();
}

bool Sprite::animationActive() const
{
    return animation_active_;
}

bool Sprite::animation_active() const
{
    return animation_active_;
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
    return x_ = value;
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
    return y_ = value;
}

float Sprite::set_x(float value)
{
    SDL_Surface* screen = SDL_GetVideoSurface();
    if (lockedToScreen()) {
        if (value > (screen->w - getWidth())) {
            value = (float)(screen->w - getWidth());
        } else if (value < 0) {
            value = 0;
        }
    }
    return x_ = value;
}

float Sprite::set_y(float value)
{
    SDL_Surface* screen = SDL_GetVideoSurface();
    if (lockedToScreen()) {
        if (value > (screen->h - getHeight())) {
            value = (float)(screen->h - getHeight());
        } else if (value < 0) {
            value = 0;
        }
    }
    return y_ = value;
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

SDL_Rect Sprite::rect() const
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

SDL_Rect Sprite::reduced_rect() const
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
