#ifndef INCLUDE_HEAT_MAP_NODE_HPP_
#define INCLUDE_HEAT_MAP_NODE_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <unordered_map>

#include <implot.h>

#include "../node_styling.hpp"
#include "../node.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
const NodeStyling DEFAULT_HEAT_MAP_NODE_STYLING(ImVec4(220,  70, 170, 255),
                                                ImVec4(250, 100, 200, 255),
                                                ImVec4(250, 100, 200, 255),
                                                ImVec4(250, 100, 200, 255),
                                                250.0f);
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This Node allows a user to augment multiple matrices together
//-------------------------------------------------------------------
class HeatMapNode : public Node<HeatMapNode>
{
public:

    HeatMapNode(std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback)
    : Node<HeatMapNode>(pin_deleted_link_manager_callback)
    {
        this->set_node_styling(DEFAULT_HEAT_MAP_NODE_STYLING);

        output_pin_.set_name("out");
        output_pin_.set_pin_type(PinType::Output);
        output_pin_.set_parent_node_id(this->get_id());

        input_pin_.set_name("in");
        input_pin_.set_pin_type(PinType::Input);
        input_pin_.set_parent_node_id(this->get_id());
        input_pin_.set_notify_parent_node_callback(std::bind(&HeatMapNode::input_data_has_been_updated_callback, this));
    }

    ~HeatMapNode()
    {
        this->pin_deleted_link_manager_callback_(&input_pin_);
        this->pin_deleted_link_manager_callback_(&output_pin_);
    }

    const std::string& get_node_type()const
    {
        return node_type;
    }



    Pin<MatrixType>* find_pin_using_id(int pin_id)
    {
        if(input_pin_.get_id() == pin_id)
            return &input_pin_;

        if(output_pin_.get_id() == pin_id)
            return &output_pin_;
        
        return nullptr;
    }

    int get_number_of_input_pins()const
    {
        return 1;
    }

    int get_number_of_output_pins()const
    {
        return 1;
    }



    void input_data_has_been_updated_callback()
    {
        output_pin_.update_data(input_pin_.get_data());

        if(input_pin_.get_data())
            output_pin_.set_name("out");
        else
            output_pin_.set_name("out (0x0)");

        // if(input_pin_.get_data())
        // {
        //     x_axis_selector_ui_.draw(*input_pin_.get_data(), false, true, false, "", "pick one x-axis");
        //     y_axes_selector_ui_.draw(*input_pin_.get_data(), false, true, true, "", "pick y-axes");
            
        //     x_axis_ = LazyMatrix::transpose( LazyMatrix::select_columns(*input_pin_.get_data(), x_axis_selector_ui_.get_selected_columns_vector()) );
        //     y_axes_ = LazyMatrix::transpose( LazyMatrix::select_columns(*input_pin_.get_data(), y_axes_selector_ui_.get_selected_columns_vector()) );
        // }
    }
    
    
    
    void draw_input_pins()
    {
        input_pin_.draw();
    }

    void draw_output_pins()
    {
        output_pin_.draw();
    }

    void draw_node_content()
    {        
        if(input_pin_.get_data())
        {
            if(input_pin_.get_data()->size() > 0)
            {
                if(ImPlot::BeginPlot("##Heatmap2", ImVec2(-1,-1))) /*ImVec2(this->node_styling_.get_node_maximum_width(), this->node_styling_.get_node_maximum_width())))*/
                {
                    ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
                    ImPlot::SetupAxesLimits(-1,1,-1,1);
                    ImPlot::PlotHeatmap("heat1",&(*input_pin_.get_data())(0,0), input_pin_.get_data()->rows(), input_pin_.get_data()->columns(), 0, 1, NULL);
                    ImPlot::EndPlot();
                }
            }
        }
    }



    void save_to_json_internal(const std::string& node_name, nlohmann::json* json_file)
    {
        (*json_file)["nodes"][node_name]["type"] = node_type.c_str();
    }



private:

    Pin<MatrixType> input_pin_;
    Pin<MatrixType> output_pin_;

    static std::string node_type;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
std::string HeatMapNode::node_type = "Heat Map Node";
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_HEAT_MAP_NODE_HPP_