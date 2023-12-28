//-------------------------------------------------------------------
/**
 * @file graph.hpp
 * @brief Defines the Graph class for the DataGraph namespace.
 *
 * The Graph class is the central component of the DataGraph library, 
 * managing the structure and interactions within a computational graph. 
 * It utilizes NodeManager for node-related operations and LinkManager for 
 * managing connections between nodes. This class includes a mechanism 
 * for cycle detection in the graph to ensure that connections do not create 
 * circular dependencies. It provides high-level functionalities such as adding 
 * nodes, connecting pins, and querying the graph structure. The cycle detection 
 * is optional and can be enabled during the pin connection process.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



#ifndef DATAGRAPH_GRAPH_HPP
#define DATAGRAPH_GRAPH_HPP



//-------------------------------------------------------------------
#include <unordered_set>
#include <memory>

#include "node_manager.hpp"
#include "link_manager.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph 
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class Graph 
{
public:

    Graph()
    {
    }

    void add_node(std::shared_ptr<Node> node) 
    {
        node_manager_.add_node(node);
    }

    bool remove_node(int64_t node_id) 
    {
        return node_manager_.remove_node(node_id);
    }

    bool connect_pins(int64_t pin1_id, int64_t pin2_id, bool should_check_for_cycles = false)
    {
        auto pin1 = get_pin_by_id(pin1_id);
        auto pin2 = get_pin_by_id(pin2_id);

        if (!pin1 || !pin2)
        {
            return false; // One or both pins not found.
        }

        // Ensure pins are not of the same type and represent the same data type
        if ((pin1->is_input() == pin2->is_input()) || (pin1->type() != pin2->type()))
        {
            return false; // Invalid connection attempt.
        }

        // Determine input and output pins
        std::shared_ptr<BasePin> input_pin = pin1->is_input() ? pin1 : pin2;
        std::shared_ptr<BasePin> output_pin = pin1->is_output() ? pin1 : pin2;

        // Check if the input pin is already connected
        if (link_manager_.is_pin_connected(input_pin->get_id()))
        {
            return false; // Input pin is already connected to an output pin.
        }

        if (should_check_for_cycles && has_cycle(output_pin, input_pin))
        {
            return false; // Cycle detected, connection not allowed.
        }

        return link_manager_.create_link(output_pin, input_pin);
    }

    int64_t get_id() const 
    {
        return id_;
    }

    const NodeManager& get_node_manager()const
    {
        return node_manager_;
    }

    const LinkManager& get_link_manager()const
    {
        return link_manager_;
    }



private:

    std::shared_ptr<BasePin> get_pin_by_id(int64_t id) 
    {
        for (auto it = node_manager_.cbegin(); it != node_manager_.cend(); ++it) 
        {
            auto pin = it->second->get_pin_by_id(id);
            if (pin) 
            {
                return pin;
            }
        }
        return nullptr;
    }

    bool has_cycle(std::shared_ptr<BasePin> output_pin, std::shared_ptr<BasePin> input_pin) 
    {
        visited.clear();
        return dfs_check_cycle(output_pin->get_node_id(), input_pin->get_node_id(), visited);
    }

    bool dfs_check_cycle(int64_t current_node_id, int64_t target_node_id, std::unordered_set<int64_t>& visited) 
    {
        if (current_node_id == target_node_id)
        {
            return true; // Cycle detected.
        }

        if (visited.find(current_node_id) != visited.end())
        {
            return true; // Node already visited.
        }

        visited.insert(current_node_id);

        auto current_node = node_manager_.get_node(current_node_id);
        
        if (current_node)
        {
            // Iterate over output pins of the target node
            auto output_pins_of_target = node_manager_.get_node(target_node_id)->get_output_pins();
            for (const auto& output_pin : output_pins_of_target)
            {
                // Retrieve all input pins connected to this output pin
                auto connected_input_pins = link_manager_.get_connected_input_pins(output_pin->get_id());

                for (const auto& input_pin : connected_input_pins)
                {
                    // Get the parent node ID of each connected input pin
                    int64_t parent_node_id = input_pin->get_node_id();
                    
                    if (dfs_check_cycle(parent_node_id, target_node_id, visited))
                    {
                        return true;
                    }
                }
            }
        }
        
        return false;
    }

    NodeManager node_manager_;
    LinkManager link_manager_;
    int64_t id_ = IncrementalID::get_id();
    std::unordered_set<int64_t> visited; // Member variable for cycle detection
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // DATAGRAPH_GRAPH_HPP
