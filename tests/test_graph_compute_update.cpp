//-------------------------------------------------------------------
/**
 * @file test_graph_compute_update.cpp
 * @brief Test suite for graph computation and update propagation.
 *
 * Tests the propagation of compute signals and updates in the DataGraph structure.
 * Verifies whether changes in inputs trigger the correct computation and signal updates
 * across connected nodes.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
#include <catch2/catch_all.hpp>
#include <datagraph/datagraph.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Mock Node Classes for Testing
//-------------------------------------------------------------------
class MockNode : public DataGraph::Node
{
public:

    void compute() override
    {
        // Example computation: Increment a value in the output pin
        if (!get_output_pins().empty())
        {
            auto output_pin = std::dynamic_pointer_cast<DataGraph::Pin<int>>(get_output_pins().front());
            if (output_pin)
            {
                auto current_data = output_pin->get_data();
            
                if (!current_data)
                {
                    current_data = std::make_shared<int>(0);
                }

                (*current_data)++;
                output_pin->set_data(current_data);
            }
        }
    }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Test Case
//-------------------------------------------------------------------
TEST_CASE("Graph Compute and Update Propagation", "[Graph][Compute]")
{
    SECTION("Signal Propagation and Node Computation")
    {
        // Create a simple graph structure
        DataGraph::Graph graph;
        auto node1 = std::make_shared<MockNode>();
        auto node2 = std::make_shared<MockNode>();
        auto input_pin = std::make_shared<DataGraph::Pin<int>>(node1.get(), DataGraph::PinType::Input);
        auto output_pin = std::make_shared<DataGraph::Pin<int>>(node2.get(), DataGraph::PinType::Output);

        node1->add_input_pin(input_pin);
        node2->add_output_pin(output_pin);
        graph.add_node(node1);
        graph.add_node(node2);
        graph.connect_pins(input_pin->get_id(), output_pin->get_id());

        // Modify input data of node1
        auto input_data = std::make_shared<int>(10);
        std::dynamic_pointer_cast<DataGraph::Pin<int>>(input_pin)->set_data(input_data);

        // Trigger signal propagation and computation
        graph.propagate_signals();
        graph.compute();

        // Verify updated data in node2's output
        auto updated_output_data = std::dynamic_pointer_cast<DataGraph::Pin<int>>(output_pin)->get_data();
        REQUIRE(updated_output_data != nullptr);
        REQUIRE(*updated_output_data == 1); // Assuming MockNode increments the output value
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
TEST_CASE("Link Removal and Node Update Trigger", "[Graph][LinkRemoval]")
{
    SECTION("Node Computation Triggered on Link Removal")
    {
        // Create graph with connected nodes
        DataGraph::Graph graph;
        auto node1 = std::make_shared<MockNode>();
        auto node2 = std::make_shared<MockNode>();
        auto input_pin = std::make_shared<DataGraph::Pin<int>>(node1.get(), DataGraph::PinType::Input);
        auto output_pin = std::make_shared<DataGraph::Pin<int>>(node2.get(), DataGraph::PinType::Output);

        node1->add_input_pin(input_pin);
        node2->add_output_pin(output_pin);
        graph.add_node(node1);
        graph.add_node(node2);
        graph.connect_pins(input_pin->get_id(), output_pin->get_id());

        // Simulate data change on node1's input
        auto input_data = std::make_shared<int>(10);
        std::dynamic_pointer_cast<DataGraph::Pin<int>>(input_pin)->set_data(input_data);

        // Propagate signals and compute
        graph.propagate_signals();
        graph.compute();

        // Remove the link between nodes
        graph.remove_link(input_pin->get_id(), output_pin->get_id());

        // Check if node1's input update counter is incremented due to link removal
        REQUIRE(node1->needs_computation());

        // Trigger computation again and verify node1's behavior
        graph.compute();

        // Verify updated data in node1 to confirm computation occurred
        // (Assuming MockNode's compute logic changes the state in some way)
        // REQUIRE( ... ); // Add appropriate checks based on MockNode's logic
    }
}
//-------------------------------------------------------------------

