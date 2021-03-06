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

Sprite::Sprite(std::string path) : Surface(path)
{ 
}

Sprite::Sprite(const Surface& surface) : Surface(surface)
{
}

Sprite::Sprite(int w, int h, int bpp, int type) : Surface(w, h, bpp, type)
{
}

Sprite::Sprite(int w, int h, int bpp) : Surface(w, h, bpp)
{
}

Sprite::Sprite(int w, int h) : Surface(w, h)
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
    update(timer.ticks(), timer.frame_time());
}

void Sprite::update(unsigned int ticks, float frametime)
{
    // update position
    set_x(x() + (x_vel() * frametime));
    set_y(y() + (y_vel() * frametime));

    // update animation
    if (anim_ticks_ == 0) {    // first frame
        anim_ticks_ = ticks;
    } else if (ticks - anim_ticks_ > anim_speed_) {
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
        anim_ticks_ = ticks;
    }
}

void Sprite::init_animation(int speed, int frames, int iterations)
{
    anim_speed_ = speed;
    total_frames_ = frames;
    total_iterations_ = iterations;

    set_width(data->w / total_frames_);
    animation_active_ = true;
}

void Sprite::set_current_anim_frame(int num)
{
    current_frame_ = num;
}

void Sprite::reset_anim_timer(const Timer& timer)
{
    anim_ticks_ = timer.ticks();
}

bool Sprite::animation_active() const
{
    return animation_active_;
}

float Sprite::set_x(float value)
{
    SDL_Surface* screen = SDL_GetVideoSurface();
    if (locked_to_screen()) {
        if (value > (screen->w - width())) {
            value = (float)(screen->w - width());
        } else if (value < 0) {
            value = 0;
        }
    }
    return x_ = value;
}

float Sprite::set_y(float value)
{
    SDL_Surface* screen = SDL_GetVideoSurface();
    if (locked_to_screen()) {
        if (value > (screen->h - height())) {
            value = (float)(screen->h - height());
        } else if (value < 0) {
            value = 0;
        }
    }
    return y_ = value;
}

SDL_Rect Sprite::rect() const
{
    SDL_Rect rect;

    rect.x = (int16_t)x();
    rect.y = (int16_t)y();
    rect.w = (uint16_t)width();
    rect.h = (uint16_t)height();

    return rect;
}

SDL_Rect Sprite::reduced_rect() const
{
    SDL_Rect rect = Sprite::rect();

    rect.x = (int16_t)(rect.x + ((float)rect.w * 0.25f));
    rect.y = (int16_t)(rect.y + ((float)rect.h * 0.25f));
    rect.w = (int16_t)(rect.w * 0.5f);
    rect.h = (int16_t)(rect.h * 0.5f);

    return rect;
}

bool Sprite::overlap(const Sprite& other) const
{
    return sdlc::overlap(rect(), other.rect());
}

// -----------------------------------------------------------------------------
// Private functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Free functions
// -----------------------------------------------------------------------------

bool overlap(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
    return ((rect1.x + rect1.w) > rect2.x && rect1.x < (rect2.x + rect2.w) && 
            (rect1.y + rect1.h) > rect2.y && rect1.y < (rect2.y + rect2.h));
}

} // namespace sdlc
