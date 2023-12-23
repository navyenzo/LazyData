#ifndef INCLUDE_NODE_MANAGER_HPP_
#define INCLUDE_NODE_MANAGER_HPP_



//-------------------------------------------------------------------
#include <deque>
#include <variant>

#include "node.hpp"

#include "specialized_nodes/specialized_nodes.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Definition of Variant used to represent all nodes
//-------------------------------------------------------------------
using NodeVariantType = std::variant<
                                     AugmentNode,
                                     UnaryOperatorNode,
                                     MatrixSourceNode,
                                     ImageLoaderNode,
                                     CsvLoaderNode,
                                     TableNode,
                                     PlotNode,
                                     HeatMapNode,
                                     ROINode,
                                     SelectorNode
                                    >;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Functions used to visit nodes in the generic container
//-------------------------------------------------------------------
struct FindPinUsingID
{
    template<typename NodeType>

    Pin<MatrixType>* operator()(NodeType& node, int pin_id)
    {
        return node.find_pin_using_id(pin_id);
    }
};



struct DrawNode
{
    template<typename NodeType>

    void operator()(NodeType& node)
    {
        node.draw();
    }
};



struct GetNodeID
{
    template<typename NodeType>

    int operator()(NodeType& node)
    {
        return node.get_id();
    }
};



struct HandleNodeHovering
{
    template<typename NodeType>

    void operator()(NodeType& node)
    {
        node.handle_hovering();
    }
};



struct SaveNodeToJson
{
    template<typename NodeType>

    void operator()(NodeType& node, nlohmann::json* json_file)
    {
        node.save_to_json(json_file);
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Node Manager
//-------------------------------------------------------------------
class NodeManager
{
public:

    NodeManager()
    {
    }



    Pin<MatrixType>* find_pin_using_id(int pin_id)
    {
        Pin<MatrixType>* found_pin = nullptr;

        for(auto& node : nodes_)
        {
            found_pin = std::visit(FindPinUsingID{}, node, std::variant<int>(pin_id));

            if(found_pin)
                return found_pin;
        }

        return found_pin;
    }



    template<typename NodeType,
             typename... Arguments>

    NodeType& construct_and_add_node(const Arguments&... arguments)
    {
        nodes_.emplace_back(std::in_place_type<NodeType>, arguments...);
        return std::get<NodeType>(nodes_.back());
    }



    void draw()
    {
        for(auto& node : nodes_)
        {
            std::visit(DrawNode(), node);
        }
    }



    void remove_node(int node_id)
    {
        for(auto iter = nodes_.begin(); iter != nodes_.end(); ++iter)
        {
            if(std::visit(GetNodeID{}, *iter) == node_id)
            {
                nodes_.erase(iter);
                return;
            }
        }
    }



    void handle_hovering()
    {
        int hovered_node_id = 0;

        if(ImNodes::IsNodeHovered(&hovered_node_id))
        {
            for(auto& node : nodes_)
            {
                if(std::visit(GetNodeID{}, node) == hovered_node_id)
                {
                    std::visit(HandleNodeHovering(), node);
                    return;
                }
            }
        }
    }



    void handle_node_deletion()
    {
        int number_of_selected_nodes = ImNodes::NumSelectedNodes();
        
        if (number_of_selected_nodes > 0)
        {
            std::vector<int> selected_nodes(number_of_selected_nodes, 0);
            ImNodes::GetSelectedNodes(selected_nodes.data());

            for (const auto& node_id : selected_nodes)
                remove_node(node_id);
        }
    }



    nlohmann::json* save_to_json(nlohmann::json* json_file)
    {
        (*json_file)["nodes"]["number_of_nodes"] = nodes_.size();

        std::vector<int> node_ids;
        for(auto& node : nodes_)
            node_ids.push_back(std::visit(GetNodeID{}, node));

        (*json_file)["nodes"]["node IDs"] = node_ids;

        for(auto& node : nodes_)
            std::visit(SaveNodeToJson{}, node, std::variant<nlohmann::json*>(json_file));

        return json_file;
    }



    auto size()const
    {
        return nodes_.size();
    }



private:

    // Storage for different types of Nodes
    std::deque<NodeVariantType> nodes_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_NODE_MANAGER_HPP_