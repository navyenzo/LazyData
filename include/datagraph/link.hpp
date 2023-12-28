//-------------------------------------------------------------------
/**
 * @file link.hpp
 * @brief Defines the Link class for the DataGraph namespace.
 *
 * The Link class represents a connection between two pins in the graph: an output pin and an input pin.
 * It encapsulates the details of this connection, including the identities of the connected pins.
 * This class is a key component in managing the data flow within the computational graph, ensuring that
 * the connections between nodes are correctly represented and accessible.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



#ifndef DATAGRAPH_LINK_HPP
#define DATAGRAPH_LINK_HPP



//-------------------------------------------------------------------
#include <memory>
#include "pin.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class Link
{
public:

    Link(std::shared_ptr<BasePin> output_pin, std::shared_ptr<BasePin> input_pin) : output_pin_(output_pin), input_pin_(input_pin)
    {}

    int64_t get_id() const
    {
        return id_;
    }

    std::shared_ptr<BasePin> get_input_pin() const
    {
        return input_pin_;
    }

    std::shared_ptr<BasePin> get_output_pin() const
    {
        return output_pin_;
    }



private:
    
    int64_t id_ = IncrementalID::get_id();
    std::shared_ptr<BasePin> output_pin_;
    std::shared_ptr<BasePin> input_pin_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // DATAGRAPH_LINK_HPP
