//-------------------------------------------------------------------
/**
 * @file serializer.hpp
 * @brief Serializer class for saving graph components to JSON format.
 *
 * This file defines the Serializer class which provides static methods to serialize 
 * different components of a graph (like Nodes, Pins, Links, etc.) into a JSON format.
 * This is useful for saving the state of a graph and its components for later retrieval or analysis.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP



//-------------------------------------------------------------------
#include <nlohmann/json.hpp>
#include "pin.hpp"
#include "node.hpp"
#include "link.hpp"
#include "link_manager.hpp"
#include "node_manager.hpp"
#include "graph.hpp"
#include <memory>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class Serializer
 * @brief Provides functionality for serializing graph components to JSON.
 */
//-------------------------------------------------------------------
class Serializer 
{
public:

    /**
     * @brief Serializes a Graph object to JSON.
     * @param graph Reference to the Graph object.
     * @param json_file Reference to the JSON object for output.
     */
    static void save_to_json(const Graph& graph, nlohmann::json& json_file)
    {
        const NodeManager& node_manager = graph.get_node_manager();
        const LinkManager& link_manager = graph.get_link_manager();

        // Serialize nodes
        nlohmann::json nodes_json = nlohmann::json::array();
        for (const auto& node_pair : node_manager)
        {
            auto node = node_pair.second;
            nlohmann::json node_json;
            node_json["id"] = node->get_id();

            // Serialize output and input pins for each node
            nlohmann::json pins_json = nlohmann::json::array();
            for (const auto& pin : node->get_output_pins())
            {
                nlohmann::json pin_json = serialize_pin(pin, link_manager);
                pins_json.push_back(pin_json);
            }
            for (const auto& pin : node->get_input_pins())
            {
                nlohmann::json pin_json = serialize_pin(pin, link_manager);
                pins_json.push_back(pin_json);
            }
            node_json["pins"] = pins_json;
            nodes_json.push_back(node_json);
        }
        json_file["nodes"] = nodes_json;
        json_file["node_count"] = node_manager.size();

        // Serialize links
        nlohmann::json links_json = nlohmann::json::array();
        for (const auto& link : link_manager.get_links())
        {
            nlohmann::json link_json;
            link_json["id"] = link->get_id();
            link_json["output_pin_id"] = link->get_output_pin()->get_id();
            link_json["input_pin_id"] = link->get_input_pin()->get_id();
            links_json.push_back(link_json);
        }
        json_file["links"] = links_json;
        json_file["link_count"] = links_json.size();
    }



    static void load_from_json(Graph& graph, const nlohmann::json& json_file)
    {
        graph.clear(); // Clear existing graph data

        // Temporary mapping from old IDs (JSON) to new IDs (created objects)
        std::unordered_map<int64_t, std::shared_ptr<Node>> node_map;
        std::unordered_map<int64_t, std::shared_ptr<BasePin>> pin_map;

        // Reconstruct nodes and pins
        if (json_file.contains("nodes"))
        {
            for (const auto& node_json : json_file["nodes"])
            {
                auto new_node = std::make_shared<Node>();
                graph.add_node(new_node);
                node_map[node_json["id"]] = new_node;

                for (const auto& pin_json : node_json["pins"])
                {
                    // Determine the pin type and create the pin
                    std::shared_ptr<BasePin> new_pin;
                    if (pin_json["type"] == "Output")
                    {
                        new_pin = std::make_shared<Pin<int>>(new_node.get(), PinType::Output); // Assuming Pin<int>
                        new_node->add_output_pin(new_pin);
                    }
                    else
                    {
                        new_pin = std::make_shared<Pin<int>>(new_node.get(), PinType::Input); // Assuming Pin<int>
                        new_node->add_input_pin(new_pin);
                    }
                    pin_map[pin_json["id"]] = new_pin;
                }
            }
        }

        // Reconstruct links
        if (json_file.contains("links"))
        {
            for (const auto& link_json : json_file["links"])
            {
                int64_t output_pin_id = link_json["output_pin_id"];
                int64_t input_pin_id = link_json["input_pin_id"];

                auto output_pin = pin_map[output_pin_id];
                auto input_pin = pin_map[input_pin_id];

                if (output_pin && input_pin)
                {
                    graph.connect_pins(output_pin->get_id(), input_pin->get_id());
                }
            }
        }
    }



private:

    static nlohmann::json serialize_pin(const std::shared_ptr<BasePin>& pin, const LinkManager& link_manager)
    {
        nlohmann::json pin_json;
        pin_json["id"] = pin->get_id();
        pin_json["type"] = pin->is_input() ? "Input" : "Output";
        pin_json["data_type"] = pin->type().name();
        pin_json["node_id"] = pin->get_node_id();

        if (pin->is_output())
        {
            auto connected_pins = link_manager.get_connected_input_pins(pin->get_id());
            nlohmann::json connected_pins_json = nlohmann::json::array();
            for (const auto& connected_pin : connected_pins)
            {
                connected_pins_json.push_back({{"id", connected_pin->get_id()}});
            }
            pin_json["connected_pins"] = connected_pins_json;
        }
        else // if input
        {
            auto connected_pin = link_manager.get_connected_output_pin(pin->get_id());
            if (connected_pin)
            {
                pin_json["connected_pin"] = {{"id", connected_pin->get_id()}};
            }
        }
        return pin_json;
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // SERIALIZER_HPP
