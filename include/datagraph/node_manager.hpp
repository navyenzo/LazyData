//-------------------------------------------------------------------
/**
 * @file node_manager.hpp
 * @brief Defines the NodeManager class for the DataGraph namespace.
 *
 * The NodeManager class is responsible for managing nodes within the computational graph.
 * It provides functionalities to add, remove, and access nodes based on their unique IDs.
 * This encapsulation of node management enhances the modularity and maintainability of the graph.
 * 
 * Nodes can be added to the graph, and their removal is facilitated through their unique IDs.
 * The NodeManager also allows querying of nodes for further operations or data processing.
 * 
 * This class plays a crucial role in maintaining an organized and efficient structure 
 * within the computational graph system.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



#ifndef DATAGRAPH_NODE_MANAGER_HPP
#define DATAGRAPH_NODE_MANAGER_HPP



//-------------------------------------------------------------------
#include <unordered_map>
#include <memory>

#include "node.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class NodeManager
{
public:

    // Adds a node to the manager.
    void add_node(std::shared_ptr<Node> node)
    {
        int64_t id = node->get_id();
        nodes_[id] = node;
    }

    // Attempts to remove a node by its ID.
    bool remove_node(int64_t node_id)
    {
        auto it = nodes_.find(node_id);
        if (it != nodes_.end())
        {
            nodes_.erase(it);
            return true;
        }
        return false;
    }

    // Retrieves a node by its ID.
    std::shared_ptr<Node> get_node(int64_t node_id) const
    {
        auto it = nodes_.find(node_id);
        if (it != nodes_.end())
        {
            return it->second;
        }
        return nullptr;
    }

    // Methods to get the beginning/end iterators of the nodes map
    auto cbegin()const { return nodes_.cbegin(); }
    auto cend()const { return nodes_.cend(); }
    auto begin()const { return nodes_.begin(); }
    auto end()const { return nodes_.end(); }
    auto size()const { return nodes_.size(); }
    void clear() { nodes_.clear(); }



private:

    std::unordered_map<int64_t, std::shared_ptr<Node>> nodes_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // DATAGRAPH_NODE_MANAGER_HPP
