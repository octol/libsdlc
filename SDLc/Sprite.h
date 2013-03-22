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
    Sprite() = default;
    virtual ~Sprite() {};

    // Main functions
    void update(const Timer& timer);
    void update(float frametime);

    // Sprite animation stuff
    void init_animation(int speed, int frames, int iterations);
    void set_current_anim_frame(int num);  // set current frame
    void reset_anim_timer();              // reset the animation timer
    bool animation_active() const;
    
    // Sprite data
    std::string name = "Unknown sdlc::Sprite";
    float x() const;
    float y() const;
    float set_x(float value);
    float set_y(float value);
    void set_pos(float x, float y);

    float x_vel();
    float y_vel();
    float set_x_vel(float value);
    float set_y_vel(float value);
    void  set_vel(float x, float y);

    bool locked_to_screen();
    bool set_locked_to_screen(bool value);

    SDL_Rect rect() const;
    SDL_Rect reduced_rect() const;
  
    // collision functions
    bool overlap(const Sprite& other);

    // DEPRECATED
    void initAnimation(int speed, int frames, int iterations);
    void setCurrentAnimFrame(int num);  // set current frame
    void resetAnimTimer();              // reset the animation timer
    bool animationActive() const;

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

protected:
    bool overlap(const SDL_Rect& rect1, const SDL_Rect& rect2);

private:
    // Animation data
    int total_frames_ = 1;
    int current_frame_ = 1;
    int total_iterations_ = 0; 
    int current_iteration_ = 0;
    unsigned int anim_speed_ = 0;
    unsigned int anim_ticks_ = 0;
    bool animation_active_ = false; // if the animation is running

    // Sprite data
    float x_ = 0; 
    float y_ = 0;
    float x_vel_ = 0;
    float y_vel_ = 0;
    bool locked_to_screen_ = false;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
float Sprite::getX() const 
{
    return x_;
}

inline
float Sprite::getY() const 
{
    return y_;
}

inline
void Sprite::setPos(float x, float y) 
{
    setX(x);
    setY(y);
}

inline
float Sprite::x() const 
{
    return x_;
}

inline
float Sprite::y() const 
{
    return y_;
}

inline
void Sprite::set_pos(float x, float y) 
{
    set_x(x);
    set_y(y);
}

inline
float Sprite::getXVel() 
{
    return x_vel_;
}

inline
float Sprite::x_vel() 
{
    return x_vel_;
}

inline
float Sprite::setXVel(float value)  
{
    return x_vel_ = value;
}

inline
float Sprite::set_x_vel(float value)  
{
    return x_vel_ = value;
}

inline
float Sprite::getYVel()         
{
    return y_vel_;
}

inline
float Sprite::y_vel()         
{
    return y_vel_;
}

inline
float Sprite::setYVel(float value)  
{
    return y_vel_ = value;
}

inline
float Sprite::set_y_vel(float value)  
{
    return y_vel_ = value;
}

inline
void Sprite::setVel(float x, float y)  
{
    setXVel(x);
    setYVel(y);
}

inline
void Sprite::set_vel(float x, float y)  
{
    set_x_vel(x);
    set_y_vel(y);
}

inline
bool Sprite::lockedToScreen()       
{
    return locked_to_screen_;
}

inline
bool Sprite::locked_to_screen()       
{
    return locked_to_screen_;
}

inline
bool Sprite::lockedToScreen(bool value) 
{
    return locked_to_screen_ = value;
}

inline
bool Sprite::set_locked_to_screen(bool value) 
{
    return locked_to_screen_ = value;
}
} // namespace sdlc
#endif // SDLC_SPRITE_H
