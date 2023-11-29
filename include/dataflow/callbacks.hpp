#ifndef INCLUDE_CALLBACKS_HPP_
#define INCLUDE_CALLBACKS_HPP_



//-------------------------------------------------------------------
#include <functional>
#include <vector>
#include <unordered_map>
#include <atomic>

#include "constants_and_defaults.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename CallbackReturnType,
         typename...CallbackArguments>

class Callbacks
{
public:

    using CallbackType = std::function<CallbackReturnType(CallbackArguments...arguments)>;

    Callbacks()
    {
    }

    int get_number_of_registered_callbacks()const
    {
        return callbacks_.size();
    }

    int register_callback(CallbackType callback)
    {
        int callback_id = UniqueID::get_id();
        callbacks_[callback_id] = callback;
        return callback_id;
    }

    bool deregister_callback(int callback_id)
    {
        auto found_callback = callbacks_.find(callback_id);

        if(found_callback == callbacks_.end())
            return false;
        
        callbacks_.erase(found_callback);

        return true;
    }

    void deregister_all_callbacks()
    {
        callbacks_.clear();
    }

    void operator()(const CallbackArguments&... arguments)const
    {
        for(const auto& callback : callbacks_)
        {
            callback(arguments...);
        }
    }

    void emit(const CallbackArguments&... arguments)const
    {
        for(const auto& callback : callbacks_)
        {
            callback(arguments...);
        }
    }

private:

    std::unordered_map<int, CallbackType> callbacks_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_CALLBACKS_HPP_