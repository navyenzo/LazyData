//-------------------------------------------------------------------
/**
 * @file node.hpp
 * @brief Defines the Node class for the DataGraph namespace.
 *
 * The Node class represents a computational node in a graph. It manages
 * a collection of input and output pins, facilitating the creation of a flexible
 * and dynamic computational graph structure. Nodes can be connected through their pins
 * to form complex computational networks. This class provides methods to add pins to a node 
 * and retrieve a pin by its unique ID.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



#ifndef DATAGRAPH_NODE_HPP
#define DATAGRAPH_NODE_HPP



//-------------------------------------------------------------------
#include "pin.hpp"
#include <vector>
#include <memory>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class Graph; // Forward declaration.
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class Node
{
public:

    explicit Node()
    {
    }

    void add_input_pin(std::shared_ptr<BasePin> pin)
    {
        input_pins_.push_back(pin);
    }

    void add_output_pin(std::shared_ptr<BasePin> pin)
    {
        output_pins_.push_back(pin);
    }

    std::shared_ptr<BasePin> get_pin_by_id(int64_t pin_id) const
    {
        for (const auto& pin : input_pins_)
        {
            if (pin->get_id() == pin_id)
            {
                return pin;
            }
        }
        
        for (const auto& pin : output_pins_)
        {
            if (pin->get_id() == pin_id)
            {
                return pin;
            }
        }
        return nullptr;
    }

    void set_id(int64_t id)
    {
        id_= id;
    }

    int64_t get_id() const
    {
        return id_;
    }

    const std::vector<std::shared_ptr<BasePin>>& get_output_pins() const
    {
        return output_pins_;
    }

    const std::vector<std::shared_ptr<BasePin>>& get_input_pins() const
    {
        return input_pins_;
    }



private:
    
    int64_t id_ = IncrementalID::get_id();
    std::vector<std::shared_ptr<BasePin>> input_pins_;
    std::vector<std::shared_ptr<BasePin>> output_pins_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // DATAGRAPH_NODE_HPP
