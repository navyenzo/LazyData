//-------------------------------------------------------------------
/**
 * @file graph.hpp
 * @brief Defines the Graph class for the DataGraph namespace.
 *
 * The Graph class is the central component of the DataGraph library, managing
 * the structure and interactions within a computational graph. It utilizes NodeManager
 * for node-related operations and LinkManager for managing connections between nodes.
 * This class also includes a mechanism for propagating signals through the graph,
 * ensuring an efficient computation flow based on data changes.
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

    /**
     * @brief Adds a node to the graph.
     * @param node The node to add.
     */
    void add_node(std::shared_ptr<Node> node);

    /**
     * @brief Removes a node from the graph by its ID.
     * @param node_id The ID of the node to remove.
     * @return True if the node was removed, false otherwise.
     */
    bool remove_node(int64_t node_id);

    /**
     * @brief Clears the graph of all nodes and links.
     */
    void clear();

    /**
     * @brief Connects two pins in the graph.
     * @param pin1_id The ID of the first pin.
     * @param pin2_id The ID of the second pin.
     * @param should_check_for_cycles Flag to check for cycles during connection.
     * @return True if the pins were connected, false otherwise.
     */
    bool connect_pins(int64_t pin1_id, int64_t pin2_id, bool should_check_for_cycles = false);

    /**
     * @brief Removes a link between two pins in the graph.
     * @param output_pin_id The ID of the output pin.
     * @param input_pin_id The ID of the input pin.
     * @return True if the link was removed, false otherwise.
     */
    bool remove_link(int64_t output_pin_id, int64_t input_pin_id);

    /**
     * @brief Removes a link based on its ID
     * @param link_id The ID of the link to remove
     * @return True if the link was removed, false otherwise.
     */
    bool remove_link(int64_t link_id);

    /**
     * @brief Gets the graph's ID.
     * @return The graph's ID.
     */
    int64_t get_id() const;

    /**
     * @brief Propagates signals through the graph, updating nodes based on changes.
     */
    void propagate_signals();

    /**
     * @brief Triggers computation across the entire graph.
     */
    void compute();

    // Accessor methods for node and link managers
    const NodeManager& get_node_manager() const;
    const LinkManager& get_link_manager() const;

private:

    std::shared_ptr<BasePin> get_pin_by_id(int64_t id);
    bool has_cycle(std::shared_ptr<BasePin> output_pin, std::shared_ptr<BasePin> input_pin);
    bool dfs_check_cycle(int64_t current_node_id, int64_t target_node_id, std::unordered_set<int64_t>& visited);

    NodeManager node_manager_;
    LinkManager link_manager_;
    int64_t id_ = IncrementalID::get_id();
    std::unordered_set<int64_t> visited; // For cycle detection
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Graph::add_node(std::shared_ptr<Node> node)
{
    node_manager_.add_node(node);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Graph::remove_node(int64_t node_id)
{
    return node_manager_.remove_node(node_id);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Graph::clear()
{
    link_manager_.clear();
    node_manager_.clear();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Graph::connect_pins(int64_t pin1_id, int64_t pin2_id, bool should_check_for_cycles)
{
    auto pin1 = get_pin_by_id(pin1_id);
    auto pin2 = get_pin_by_id(pin2_id);

    if (!pin1 || !pin2)
    {
        return false; // One or both pins not found.
    }

    // Ensure pins are not of the same type and represent the same data type
    if ((pin1->is_input() == pin2->is_input()) || (pin1->get_data_type() != pin2->get_data_type()))
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Graph::remove_link(int64_t output_pin_id, int64_t input_pin_id)
{
    return link_manager_.remove_link(output_pin_id, input_pin_id);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Graph::remove_link(int64_t link_id)
{
    return link_manager_.remove_link(link_id);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline int64_t Graph::get_id() const
{
    return id_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Graph::propagate_signals()
{
    for (const auto& [node_id, node] : node_manager_)
    {
        if (node->needs_computation())
        {
            const auto& output_pins = node->get_output_pins();
        
            for (const auto& pin : output_pins)
            {
                const auto& connected_input_pins = link_manager_.get_connected_input_pins(pin->get_id());
            
                for (const auto& input_pin : connected_input_pins)
                {
                    auto connected_node = node_manager_.get_node(input_pin->get_node_id());
                
                    if (connected_node)
                    {
                        connected_node->increment_input_update_counter();
                    }
                }
            }
        }
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Graph::compute()
{
    for (const auto& [node_id, node] : node_manager_)
    {
        node->compute();
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline const NodeManager& Graph::get_node_manager() const
{
    return node_manager_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline const LinkManager& Graph::get_link_manager() const
{
    return link_manager_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline std::shared_ptr<BasePin> Graph::get_pin_by_id(int64_t id) 
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Graph::has_cycle(std::shared_ptr<BasePin> output_pin, std::shared_ptr<BasePin> input_pin) 
{
    visited.clear();
    return dfs_check_cycle(output_pin->get_node_id(), input_pin->get_node_id(), visited);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Graph::dfs_check_cycle(int64_t current_node_id, int64_t target_node_id, std::unordered_set<int64_t>& visited) 
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // DATAGRAPH_GRAPH_HPP
