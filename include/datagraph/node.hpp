//-------------------------------------------------------------------
/**
 * @file node.hpp
 * @brief Defines the Node class for the DataGraph namespace.
 *
 * The Node class represents a computational node in a graph, managing a collection
 * of input and output pins. This class facilitates creating a dynamic computational graph
 * structure, where nodes can be connected through their pins to form complex computational
 * networks. It includes methods for pin management and computation triggering.
 *
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



#ifndef DATAGRAPH_NODE_HPP
#define DATAGRAPH_NODE_HPP



//-------------------------------------------------------------------
#include <vector>
#include <memory>

#include "pin.hpp"
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

    /**
     * @brief Adds an input pin to the node.
     * @param pin The input pin to add.
     */
    void add_input_pin(std::shared_ptr<BasePin> pin);

    /**
     * @brief Adds an output pin to the node.
     * @param pin The output pin to add.
     */
    void add_output_pin(std::shared_ptr<BasePin> pin);

    /**
     * @brief Retrieves a pin by its ID.
     * @param pin_id The ID of the pin.
     * @return Shared pointer to the requested pin, or nullptr if not found.
     */
    std::shared_ptr<BasePin> get_pin_by_id(int64_t pin_id) const;

    /**
     * @brief Sets the node's ID.
     * @param id The new ID for the node.
     */
    void set_id(int64_t id);

    /**
     * @brief Gets the node's ID.
     * @return The node's ID.
     */
    int64_t get_id() const;

    /**
     * @brief Increments the input update counter, indicating new data on input pins.
     */
    void increment_input_update_counter();

    /**
     * @brief Increments the output update counter, indicating updated output data.
     */
    void increment_output_update_counter();

    /**
     * @brief Determines if the node needs to recompute its output.
     * @return True if the node needs computation, false otherwise.
     */
    bool needs_computation() const;

    /**
     * @brief Performs computation if the node's inputs have been updated.
     */
    virtual void compute();

    // Accessor methods for pins
    const std::vector<std::shared_ptr<BasePin>>& get_output_pins() const;
    const std::vector<std::shared_ptr<BasePin>>& get_input_pins() const;



private:

    int64_t id_ = IncrementalID::get_id();
    std::vector<std::shared_ptr<BasePin>> input_pins_;
    std::vector<std::shared_ptr<BasePin>> output_pins_;
    int input_update_counter = 0;
    int output_update_counter = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Node::add_input_pin(std::shared_ptr<BasePin> pin)
{
    input_pins_.push_back(pin);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Node::add_output_pin(std::shared_ptr<BasePin> pin)
{
    output_pins_.push_back(pin);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline std::shared_ptr<BasePin> Node::get_pin_by_id(int64_t pin_id) const
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Node::set_id(int64_t id)
{
    id_ = id;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline int64_t Node::get_id() const
{
    return id_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Node::increment_input_update_counter()
{
    input_update_counter++;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Node::increment_output_update_counter()
{
    output_update_counter++;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Node::needs_computation() const
{
    return input_update_counter > 0;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Node::compute()
{
    if (needs_computation())
    {
        // Compute logic here
        output_update_counter++;
        input_update_counter = 0;
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline const std::vector<std::shared_ptr<BasePin>>& Node::get_output_pins() const
{
    return output_pins_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline const std::vector<std::shared_ptr<BasePin>>& Node::get_input_pins() const
{
    return input_pins_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // DATAGRAPH_NODE_HPP































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
