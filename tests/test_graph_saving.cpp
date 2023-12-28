//-------------------------------------------------------------------
/**
 * @file test_graph_saving.cpp
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

        // Perform checks to verify that the reconstructed graph matches the expected structure
        REQUIRE(reconstructed_graph->get_node_manager().size() == original_graph->get_node_manager().size());

        // TODO: Fix the test for chacking deserialization of node graph

        /*// Check if the number of links match
        const auto& original_link_manager = original_graph->get_link_manager();
        const auto& reconstructed_link_manager = reconstructed_graph->get_link_manager();
        REQUIRE(reconstructed_link_manager.get_links().size() == original_link_manager.get_links().size());

        // Further checks can include verifying the structure of connections in each node
        for (const auto& [node_id, original_node] : original_graph->get_node_manager())
        {
            auto reconstructed_node = reconstructed_graph->get_node_manager().get_node(node_id);
            REQUIRE(reconstructed_node != nullptr);

            // Check if output pins of original node are connected to the same number of input pins in the reconstructed graph
            for (const auto& original_output_pin : original_node->get_output_pins())
            {
                auto connected_input_pins = original_link_manager.get_connected_input_pins(original_output_pin->get_id());
                auto reconstructed_output_pin = reconstructed_node->get_pin_by_id(original_output_pin->get_id());

                REQUIRE(reconstructed_output_pin != nullptr);
                auto reconstructed_connected_input_pins = reconstructed_link_manager.get_connected_input_pins(reconstructed_output_pin->get_id());
                REQUIRE(connected_input_pins.size() == reconstructed_connected_input_pins.size());
            }

            // Similar checks can be done for input pins
        }*/
    }
}
//-------------------------------------------------------------------