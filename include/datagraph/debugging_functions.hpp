//-------------------------------------------------------------------
/**
 * @file debugging_functions.hpp
 * @brief Contains debugging utility functions for visualizing and analyzing the graph structure.
 *
 * This file is designed to provide various utility functions that aid in debugging
 * the graph structure. These functions visually represent the graph and its components
 * (like nodes and pins) in a clear and understandable format, facilitating easy
 * identification of issues and verification of graph integrity.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



#ifndef DATAGRAPH_DEBUGGING_FUNCTIONS_HPP
#define DATAGRAPH_DEBUGGING_FUNCTIONS_HPP



//-------------------------------------------------------------------
#include <iostream>
#include "graph.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Prints the structure of the graph in a textual format.
 *
 * Iterates through all the nodes and their pins in the graph,
 * displaying the connections between them. It visualizes the 
 * structure of the graph using simple text.
 *
 * @param graph The graph to be printed.
 */
//-------------------------------------------------------------------
inline void print_graph(const Graph& graph)
{
    std::cout << "-------------------------------------------------------------------\n";
    std::cout << "Graph Structure:\n";
    std::cout << "-------------------------------------------------------------------\n";

    const auto& node_manager = graph.get_node_manager();
    const auto& link_manager = graph.get_link_manager();

    for (auto it = node_manager.cbegin(); it != node_manager.cend(); ++it)
    {
        auto node = it->second;
        std::cout << "Node " << node->get_id() << "\n";
        std::cout << " +-[Pins]\n";

        // Output pins and their connections
        for (const auto& output_pin : node->get_output_pins())
        {
            auto connected_pins = link_manager.get_connected_input_pins(output_pin->get_id());
            if (!connected_pins.empty())
            {
                std::cout << "    +-[Output Pin " << output_pin->get_id() << "]\n";
                for (const auto& connected_pin : connected_pins)
                {
                    std::cout << "      |---> [Input Pin " << connected_pin->get_id() << " of Node " << connected_pin->get_node_id() << "]\n";
                }
            }
            else
            {
                std::cout << "    +-[Output Pin " << output_pin->get_id() << "] No connections\n";
            }
        }

        // Input pins and their source output pins
        for (const auto& input_pin : node->get_input_pins())
        {
            auto source_output_pin = link_manager.get_connected_output_pin(input_pin->get_id());
            if (source_output_pin)
            {
                std::cout << "    +-[Input Pin " << input_pin->get_id() << "] <--- Connected Output Pin [ID: " << source_output_pin->get_id() << "]\n";
            }
            else
            {
                std::cout << "    +-[Input Pin " << input_pin->get_id() << "] No connected output pin\n";
            }
        }

        std::cout << "\n";
    }

    std::cout << "-------------------------------------------------------------------\n";
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // DATAGRAPH_DEBUGGING_FUNCTIONS_HPP
