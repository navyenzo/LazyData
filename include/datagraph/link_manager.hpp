//-------------------------------------------------------------------
/**
 * @file link_manager.hpp
 * @brief Defines the LinkManager class for the DataGraph namespace.
 *
 * The LinkManager class is responsible for managing the connections (links) between pins in the graph.
 * It maintains a record of all links and provides functionalities to create new links and query existing ones.
 * This class plays a key role in ensuring the integrity of the graph's structure, particularly in the context
 * of preventing cycles during link creation.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



#ifndef DATAGRAPH_LINK_MANAGER_HPP
#define DATAGRAPH_LINK_MANAGER_HPP



//-------------------------------------------------------------------
#include <vector>
#include <memory>
#include <unordered_map>
#include "link.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class LinkManager
{
public:

    /**
     * @brief Constructs the LinkManager object.
     */
    LinkManager()
    {
    }

    /**
     * @brief Creates a link between an output pin and an input pin.
     *
     * @param output_pin The output pin.
     * @param input_pin The input pin.
     * @return true If the link is successfully created.
     * @return false If the link creation fails due to incompatible types.
     */
    bool create_link(std::shared_ptr<BasePin> output_pin, std::shared_ptr<BasePin> input_pin)
    {
        if (output_pin->type() != input_pin->type())
        {
            return false; // Incompatible pin types.
        }

        auto link = std::make_shared<Link>(output_pin, input_pin);
        links_.push_back(link);

        // Record the connection
        output_to_input_[output_pin->get_id()].push_back(input_pin);
        input_to_output_[input_pin->get_id()] = output_pin;
        return true;
    }

    /**
     * @brief Gets the input pins connected to a specified output pin.
     *
     * @param output_pin_id The ID of the output pin.
     * @return std::vector<std::shared_ptr<BasePin>> A vector of connected input pins.
     */
    std::vector<std::shared_ptr<BasePin>> get_connected_input_pins(int64_t output_pin_id) const
    {
        auto it = output_to_input_.find(output_pin_id);
        if (it != output_to_input_.end())
        {
            return it->second;
        }
        return {}; // Return an empty vector if no connections are found
    }

    /**
     * @brief Gets the output pin connected to a specified input pin.
     *
     * @param input_pin_id The ID of the input pin.
     * @return std::shared_ptr<BasePin> The connected output pin, or nullptr if not found.
     */
    std::shared_ptr<BasePin> get_connected_output_pin(int64_t input_pin_id) const
    {
        auto it = input_to_output_.find(input_pin_id);
        if (it != input_to_output_.end())
        {
            return it->second;
        }
        return nullptr; // Return null if no connection is found
    }

    /**
     * @brief Checks if a given pin is connected.
     *
     * @param pin_id The ID of the pin to check.
     * @return true If the pin is connected.
     * @return false If the pin is not connected.
     */
    bool is_pin_connected(int64_t pin_id) const
    {
        for (const auto& link : links_)
        {
            if (link->get_input_pin()->get_id() == pin_id)
            {
                return true; // The input pin is already connected.
            }
        }
        
        return false; // The input pin is not connected.
    }

    const std::vector<std::shared_ptr<Link>>& get_links()const
    {
        return links_;
    }

    void clear()
    {
        links_.clear();
        output_to_input_.clear();
        input_to_output_.clear();
    }



private:
    
    std::vector<std::shared_ptr<Link>> links_;
    std::unordered_map<int64_t, std::vector<std::shared_ptr<BasePin>>> output_to_input_; // Map to track connections
    std::unordered_map<int64_t, std::shared_ptr<BasePin>> input_to_output_; // Map for reverse lookup
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // DATAGRAPH_LINK_MANAGER_HPP
