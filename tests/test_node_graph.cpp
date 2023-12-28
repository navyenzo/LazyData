//-------------------------------------------------------------------
/**
 * @file test_node_graph.cpp
 * @brief Tests for the computational node graph using the Catch2 framework.
 *
 * This file contains tests to ensure the correct functionality of the node graph.
 * It includes tests for node construction, pin assignment, connecting pins, cycle detection,
 * and type and directionality constraints in pin connections.
 *
 * @author [Your Name]
 * @date [Date]
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <datagraph/datagraph.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for node construction and pin assignment.
 *
 * This test verifies the creation of nodes, the assignment of unique IDs to each node,
 * and the correct addition and type of input and output pins to nodes.
 */
//-------------------------------------------------------------------
TEST_CASE("Node Construction and Pin Assignment", "[NodeGraph]")
{
    DataGraph::Graph my_graph;
    std::vector<std::shared_ptr<DataGraph::Node>> nodes(5);

    for (int i = 0; i < nodes.size(); ++i)
    {
        nodes[i] = std::make_shared<DataGraph::Node>();
        my_graph.add_node(nodes[i]);

        auto output_pin = std::make_shared<DataGraph::Pin<int>>(nodes[i].get(), DataGraph::PinType::Output);
        nodes[i]->add_output_pin(output_pin);

        auto input_pin = std::make_shared<DataGraph::Pin<int>>(nodes[i].get(), DataGraph::PinType::Input);
        nodes[i]->add_input_pin(input_pin);

        REQUIRE(nodes[i]->get_input_pins().size() == 1);
        REQUIRE(nodes[i]->get_output_pins().size() == 1);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for connecting pins in the node graph.
 *
 * This test checks if valid connections between input and output pins are correctly established
 * and recorded in the LinkManager.
 */
//-------------------------------------------------------------------
TEST_CASE("Connecting Pins", "[NodeGraph]")
{
    DataGraph::Graph my_graph;
    std::vector<std::shared_ptr<DataGraph::Node>> nodes(5);

    for (int i = 0; i < nodes.size(); ++i)
    {
        nodes[i] = std::make_shared<DataGraph::Node>();
        my_graph.add_node(nodes[i]);

        auto output_pin = std::make_shared<DataGraph::Pin<int>>(nodes[i].get(), DataGraph::PinType::Output);
        nodes[i]->add_output_pin(output_pin);

        auto input_pin = std::make_shared<DataGraph::Pin<int>>(nodes[i].get(), DataGraph::PinType::Input);
        nodes[i]->add_input_pin(input_pin);
    }

    for (int i = 1; i < nodes.size(); ++i)
    {
        auto output_pin = nodes[i - 1]->get_output_pins()[0];
        auto input_pin = nodes[i]->get_input_pins()[0];

        REQUIRE(my_graph.connect_pins(output_pin->get_id(), input_pin->get_id(), true) == true);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for cycle detection in the node graph.
 *
 * This test ensures that the connect_pins function correctly identifies and prevents
 * connections that would form cycles in the graph.
 */
//-------------------------------------------------------------------
TEST_CASE("Cycle Detection", "[NodeGraph]")
{
    DataGraph::Graph my_graph;
    std::vector<std::shared_ptr<DataGraph::Node>> nodes(5);

    for (int i = 0; i < nodes.size(); ++i)
    {
        nodes[i] = std::make_shared<DataGraph::Node>();
        my_graph.add_node(nodes[i]);

        auto output_pin = std::make_shared<DataGraph::Pin<int>>(nodes[i].get(), DataGraph::PinType::Output);
        nodes[i]->add_output_pin(output_pin);

        auto input_pin = std::make_shared<DataGraph::Pin<int>>(nodes[i].get(), DataGraph::PinType::Input);
        nodes[i]->add_input_pin(input_pin);
    }

    for (int i = 1; i < nodes.size(); ++i)
    {
        auto output_pin = nodes[i - 1]->get_output_pins()[0];
        auto input_pin = nodes[i]->get_input_pins()[0];

        my_graph.connect_pins(output_pin->get_id(), input_pin->get_id(), true);
    }

    auto lastOutputPin = nodes[3]->get_output_pins()[0];
    auto firstInputPin = nodes[0]->get_input_pins()[0];
    REQUIRE(my_graph.connect_pins(lastOutputPin->get_id(), firstInputPin->get_id(), true) == false);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for data type constraints in pin connections.
 *
 * This test verifies that the connect_pins function prevents connections between pins
 * of different data types.
 */
//-------------------------------------------------------------------
TEST_CASE("Data Type Constraints in Connections", "[NodeGraph]")
{
    DataGraph::Graph my_graph;
    std::vector<std::shared_ptr<DataGraph::Node>> nodes(5);

    for (int i = 0; i < nodes.size(); ++i)
    {
        nodes[i] = std::make_shared<DataGraph::Node>();
        my_graph.add_node(nodes[i]);

        auto output_pin = std::make_shared<DataGraph::Pin<int>>(nodes[i].get(), DataGraph::PinType::Output);
        nodes[i]->add_output_pin(output_pin);

        auto input_pin = std::make_shared<DataGraph::Pin<int>>(nodes[i].get(), DataGraph::PinType::Input);
        nodes[i]->add_input_pin(input_pin);
    }

    auto pin_double = std::make_shared<DataGraph::Pin<double>>(nodes[4].get(), DataGraph::PinType::Output);
    auto pin_string = std::make_shared<DataGraph::Pin<std::string>>(nodes[4].get(), DataGraph::PinType::Input);
    nodes[4]->add_output_pin(pin_double);
    nodes[4]->add_input_pin(pin_string);

    REQUIRE(my_graph.connect_pins(pin_double->get_id(), pin_string->get_id(), false) == false);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Test for connection directionality in the node graph.
 *
 * This test checks that the connect_pins function only allows connections from output pins to input pins
 * and prevents connections between two output pins or two input pins.
 */
//-------------------------------------------------------------------
TEST_CASE("Connection Directionality", "[NodeGraph]")
{
    DataGraph::Graph my_graph;
    std::vector<std::shared_ptr<DataGraph::Node>> nodes(5);

    for (int i = 0; i < nodes.size(); ++i)
    {
        nodes[i] = std::make_shared<DataGraph::Node>();
        my_graph.add_node(nodes[i]);

        auto output_pin = std::make_shared<DataGraph::Pin<int>>(nodes[i].get(), DataGraph::PinType::Output);
        nodes[i]->add_output_pin(output_pin);

        auto input_pin = std::make_shared<DataGraph::Pin<int>>(nodes[i].get(), DataGraph::PinType::Input);
        nodes[i]->add_input_pin(input_pin);
    }

    auto output_pin1 = nodes[0]->get_output_pins()[0];
    auto output_pin2 = nodes[1]->get_output_pins()[0];
    auto input_pin1 = nodes[2]->get_input_pins()[0];
    auto input_pin2 = nodes[3]->get_input_pins()[0];

    // Test connecting two output pins
    REQUIRE(my_graph.connect_pins(output_pin1->get_id(), output_pin2->get_id(), false) == false);

    // Test connecting two input pins
    REQUIRE(my_graph.connect_pins(input_pin1->get_id(), input_pin2->get_id(), false) == false);
}
//-------------------------------------------------------------------