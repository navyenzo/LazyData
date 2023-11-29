#ifndef INCLUDE_ANIMATED_DRAWER_HPP_
#define INCLUDE_ANIMATED_DRAWER_HPP_



//-------------------------------------------------------------------
#include <chrono>
#include <algorithm>
#include <functional>

#include <timers/timer.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace LazyApp
//-------------------------------------------------------------------
namespace LazyApp
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// The class AnimatedDrawer defines a "variable width" that increases
// or decreases over time
//
// - It can be used to animate an opening drawer Menu by updating
//   the drawer's width in parallel
//-------------------------------------------------------------------
class AnimatedDrawer
{
public:

    AnimatedDrawer()
    {
    }

    void start_opening_drawer()
    {
        is_drawer_opening_ = true;
        begin_width_ = current_width_;

        drawer_timer_.start(std::bind(&AnimatedDrawer::animate, this, std::placeholders::_1),
                            0,
                            std::chrono::milliseconds(10));
    }

    void start_closing_drawer()
    {
        is_drawer_opening_ = false;
        begin_width_ = current_width_;

        drawer_timer_.start(std::bind(&AnimatedDrawer::animate, this, std::placeholders::_1),
                            0,
                            std::chrono::milliseconds(10));
    }



    bool is_drawer_opening()const
    {
        return is_drawer_opening_;
    }

    bool is_drawer_partially_open()const
    {
        return current_width_ > min_width_;
    }

    float get_current_width()const
    {
        return current_width_;
    }

    void set_min_width(float min_width)
    {
        min_width_ = min_width;
    }

    void set_max_width(float max_width)
    {
        max_width_ = max_width;
    }

    void set_opening_speed(float opening_speed)
    {
        opening_speed_ = std::abs(opening_speed);
    }

    void set_closing_speed(float closing_speed)
    {
        closing_speed_ = std::abs(closing_speed);
    }



private: // Private functions

    // Function that actually updates the drawer's width
    void animate(const std::chrono::duration<double>& time_lapse)
    {
        if(is_drawer_opening_)
            current_width_ = begin_width_ + time_lapse.count() * opening_speed_;
        else
            current_width_ = begin_width_ - time_lapse.count() * closing_speed_;

        if(current_width_ < min_width_)
        {
            current_width_ = min_width_;
            drawer_timer_.stop();
        }
        else if(current_width_ > max_width_)
        {
            current_width_ = max_width_;
            drawer_timer_.stop();
        }
    }



private: // Private variables

    float min_width_ = 20;
    float max_width_ = 260;

    float begin_width_ = 20;

    float current_width_ = 20;

    float opening_speed_ = 1;
    float closing_speed_ = 1;

    bool is_drawer_opening_ = true;

    Concurrent::Timer drawer_timer_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace LazyApp
//-------------------------------------------------------------------



#endif  // INCLUDE_ANIMATED_DRAWER_HPP_