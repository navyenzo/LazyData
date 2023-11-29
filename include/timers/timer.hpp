#ifndef INCLUDE_TIMER_HPP_
#define INCLUDE_TIMER_HPP_



//-------------------------------------------------------------------
#include <functional>
#include <chrono>
#include <atomic>
#include <thread>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace Concurrent
//-------------------------------------------------------------------
namespace Concurrent
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Class for creating a timer with fixed intervals and callback functionality.
 * 
 * This class allows setting up a timer that can either run for a specified number of firings
 * or for a specified duration. It can also be used for single-shot timing events. 
 * The callback function provided by the user is called at each firing.
 */
//-------------------------------------------------------------------
class Timer
{
public:

    using CallbackType = std::function<void(const std::chrono::duration<double>&)>;

    Timer();
    ~Timer();

    bool is_running()const;
    
    void stop();
    
    void wait_until_timer_stops();

    void start(CallbackType timer_callback,
               int number_of_firings = 0,
               const std::chrono::duration<double>& firing_interval = std::chrono::milliseconds(10),
               const std::chrono::duration<double>& timer_duration = std::chrono::seconds(0));

    void start_single_shot(CallbackType timer_callback);

    

private:

    void looping_timer_function();
    void single_shot_timer_function();

    std::atomic<bool> should_timer_be_stopped_ = ATOMIC_VAR_INIT(true);
    std::thread timer_thread_;

    CallbackType timer_callback_;
    
    int number_of_firings_ = 0;
    std::chrono::duration<double> firing_interval_ = std::chrono::milliseconds(10);
    std::chrono::duration<double> timer_duration_ = std::chrono::seconds(0);

    std::thread::id currently_running_thread_id_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline Timer::Timer()
{
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline Timer::~Timer()
{
    stop();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Timer::is_running()const
{
    return timer_thread_.joinable();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Timer::start(CallbackType timer_callback,
                         int number_of_firings,
                         const std::chrono::duration<double>& firing_interval,
                         const std::chrono::duration<double>& timer_duration)
{
    stop();

    timer_callback_ = timer_callback;

    number_of_firings_ = number_of_firings;
    firing_interval_ = firing_interval;
    timer_duration_ = timer_duration;

    should_timer_be_stopped_ = false;

    timer_thread_ = std::thread(&Timer::looping_timer_function, std::ref(*this));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Timer::start_single_shot(CallbackType timer_callback)
{
    stop();

    timer_callback_ = timer_callback;

    should_timer_be_stopped_ = false;

    timer_thread_ = std::thread(&Timer::single_shot_timer_function, std::ref(*this));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Timer::stop()
{
    should_timer_be_stopped_ = true;

    // We make sure that the thread calling the
    // stop function is not the same thread as
    // the timing thread, otherwise we'd crash
    if(std::this_thread::get_id() != currently_running_thread_id_)
    {
        if(timer_thread_.joinable())
            timer_thread_.join();
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Timer::wait_until_timer_stops()
{
    if(timer_thread_.joinable())
        timer_thread_.join();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Timer::looping_timer_function()
{
    currently_running_thread_id_ = std::this_thread::get_id();

    auto time_when_timer_started = std::chrono::steady_clock::now();
    auto time_when_loop_iteration_started = time_when_timer_started;
    std::chrono::duration<double> timer_duration = std::chrono::seconds(0);
    
    int number_of_firings = 0;

    while(!should_timer_be_stopped_)
    {
        time_when_loop_iteration_started = std::chrono::steady_clock::now();
        timer_duration = time_when_loop_iteration_started - time_when_timer_started;

        ++number_of_firings;

        // Check if timer has run for longer than
        // the specified amount of time
        if(timer_duration_ > std::chrono::seconds(0) && timer_duration > timer_duration_)
            should_timer_be_stopped_ = true;
        else
        {
            // Invoke the timer callback
            timer_callback_(timer_duration);

            // Sleep for as long as we need to
            // in order to firing at the specified
            // interval
            std::this_thread::sleep_until(time_when_loop_iteration_started + firing_interval_);
        }

        if(number_of_firings > number_of_firings_ && number_of_firings_ > 0)
            should_timer_be_stopped_ = true;
    }

    // Thread has finished, so we detach if we can
    if(timer_thread_.joinable())
        timer_thread_.detach();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Timer::single_shot_timer_function()
{
    currently_running_thread_id_ = std::this_thread::get_id();

    timer_callback_(std::chrono::duration<double>(0));

    should_timer_be_stopped_ = true;

    // Thread has finished, so we detach if we can
    if(timer_thread_.joinable())
        timer_thread_.detach();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace Concurrent
//-------------------------------------------------------------------



#endif  // INCLUDE_TIMER_HPP_