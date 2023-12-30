//-------------------------------------------------------------------
/**
 * @file test_graph_serialize_deserialize.cpp
 * @brief Test suite for graph serialization functionality.
 *
 * This file contains tests to ensure the correct serialization of graph components
 * such as Pins, Nodes, Links, etc., into a JSON format using the Serializer class.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <catch2/catch_all.hpp>
#include <typeinfo>

#include <datagraph/datagraph.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("Graph Serialization", "[Serializer]")
{
    SECTION("Serialize and Deserialize Graph to/from JSON")
    {
        // Create a simple graph structure
        auto original_graph = std::make_shared<DataGraph::Graph>();
        auto node1 = std::make_shared<DataGraph::Node>();
        auto node2 = std::make_shared<DataGraph::Node>();
        auto pin1 = std::make_shared<DataGraph::Pin<int>>(node1.get(), DataGraph::PinType::Output);
        auto pin2 = std::make_shared<DataGraph::Pin<int>>(node2.get(), DataGraph::PinType::Input);

        node1->add_output_pin(std::static_pointer_cast<DataGraph::BasePin>(pin1));
        node2->add_input_pin(std::static_pointer_cast<DataGraph::BasePin>(pin2));
        original_graph->add_node(node1);
        original_graph->add_node(node2);
        original_graph->connect_pins(pin1->get_id(), pin2->get_id());

        // JSON object to hold the serialization result
        nlohmann::json json_file;

        // Serialize the Graph
        DataGraph::Serializer::save_to_json(*original_graph, json_file);

        // Check if the JSON object has the correct structure and data
        REQUIRE(json_file.contains("nodes"));
        REQUIRE(json_file["nodes"].is_array());
        REQUIRE(json_file["nodes"].size() == 2);
        REQUIRE(json_file["nodes"][0].contains("pins"));
        REQUIRE(json_file["nodes"][1].contains("pins"));
        REQUIRE(json_file.contains("links"));
        REQUIRE(json_file["links"].is_array());
        REQUIRE(json_file["links"].size() >= 1);

        // Deserialize the graph from the JSON file
        auto reconstructed_graph = std::make_shared<DataGraph::Graph>();
        DataGraph::Serializer::load_from_json(*reconstructed_graph, json_file);

        // Check the number of nodes in both graphs
        REQUIRE(reconstructed_graph->get_node_manager().size() == original_graph->get_node_manager().size());
        REQUIRE(reconstructed_graph->get_link_manager().get_links().size() == original_graph->get_link_manager().get_links().size());

        // Enhanced sorting function for nodes with weighted input and output pins
        auto node_sorter = [](const std::shared_ptr<DataGraph::Node>& node1, const std::shared_ptr<DataGraph::Node>& node2)
        {
            auto score1 = node1->get_input_pins().size() * 1000 + node1->get_output_pins().size() * 0.001;
            auto score2 = node2->get_input_pins().size() * 1000 + node2->get_output_pins().size() * 0.001;
            return score1 < score2;
        };

        std::vector<std::shared_ptr<DataGraph::Node>> original_nodes, reconstructed_nodes;
        
        for (const auto& [id, node] : original_graph->get_node_manager())
        {
            original_nodes.push_back(node);
        }

        for (const auto& [id, node] : reconstructed_graph->get_node_manager())
        {
            reconstructed_nodes.push_back(node);
        }

        std::sort(original_nodes.begin(), original_nodes.end(), node_sorter);
        std::sort(reconstructed_nodes.begin(), reconstructed_nodes.end(), node_sorter);

        // Compare the sorted nodes
        for (size_t i = 0; i < original_nodes.size(); ++i)
        {
            REQUIRE(original_nodes[i]->get_input_pins().size() == reconstructed_nodes[i]->get_input_pins().size());
            REQUIRE(original_nodes[i]->get_output_pins().size() == reconstructed_nodes[i]->get_output_pins().size());
        }
    }
}
//-------------------------------------------------------------------
