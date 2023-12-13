#ifndef INCLUDE_PLOT_NODE_HPP_
#define INCLUDE_PLOT_NODE_HPP_



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
const NodeStyling DEFAULT_PLOT_NODE_STYLING(ImVec4(220,20,50,255),
                                            ImVec4(250, 50, 80, 255),
                                            ImVec4(250, 50, 80, 255),
                                            ImVec4(250, 50, 80, 255),
                                            ImVec2(400.0f, 300.0f));
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This Node allows a user to augment multiple matrices together
//-------------------------------------------------------------------
class PlotNode : public Node<PlotNode>
{
public:

    PlotNode(std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback)
    : Node<PlotNode>(pin_deleted_link_manager_callback)
    {
        this->set_node_styling(DEFAULT_PLOT_NODE_STYLING);

        output_pin_.set_name("out");
        output_pin_.set_pin_type(PinType::Output);
        output_pin_.set_parent_node_id(this->get_id());

        input_pin_.set_name("in");
        input_pin_.set_pin_type(PinType::Input);
        input_pin_.set_parent_node_id(this->get_id());
        input_pin_.set_notify_parent_node_callback(std::bind(&PlotNode::input_data_has_been_updated_callback, this));
    }

    ~PlotNode()
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

        if(input_pin_.get_data())
        {
            x_axis_selector_ui_.draw(*input_pin_.get_data(), false, true, false, "", "pick one x-axis");
            y_axes_selector_ui_.draw(*input_pin_.get_data(), false, true, true, "", "pick y-axes");
            
            x_axis_ = LazyMatrix::transpose( LazyMatrix::select_columns(*input_pin_.get_data(), x_axis_selector_ui_.get_selected_columns_vector()) );
            y_axes_ = LazyMatrix::transpose( LazyMatrix::select_columns(*input_pin_.get_data(), y_axes_selector_ui_.get_selected_columns_vector()) );
        }
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
                ImGui::Dummy(ImVec2(0,30));

                    ImGui::BeginGroup();

                    if(x_axis_selector_ui_.draw(*input_pin_.get_data(), false, true, false, "", "pick one x-axis"))
                    {
                        x_axis_ = LazyMatrix::transpose( LazyMatrix::select_columns(*input_pin_.get_data(), x_axis_selector_ui_.get_selected_columns_vector()) );
                    }
                    
                    ImGui::Spacing();

                    if(y_axes_selector_ui_.draw(*input_pin_.get_data(), false, true, true, "", "pick y-axes"))
                    {
                        y_axes_ = LazyMatrix::transpose( LazyMatrix::select_columns(*input_pin_.get_data(), y_axes_selector_ui_.get_selected_columns_vector()) );
                    }

                    ImGui::EndGroup();

                ImGui::PopItemWidth();
            }

            ImGui::SameLine();

            ImGui::BeginGroup();

                if (ImPlot::BeginPlot("Data", this->get_node_size()))
                {
                    ImPlot::SetupAxes("x","f(x)");
                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);

                    if(y_axes_.size() > 0)
                    {
                        for(int i = 0; i < y_axes_.rows(); ++i)
                        {
                            if(x_axis_.size() > 0)
                            {
                                ImPlot::PlotLine((std::string("line: ") + std::to_string(i)).c_str(),
                                                 &x_axis_(0,0),
                                                 &y_axes_(i,0),
                                                 y_axes_.columns());
                            }
                            else
                            {
                                ImPlot::PlotLine((std::string("line: ") + std::to_string(i)).c_str(),
                                                 &y_axes_(i,0),
                                                 y_axes_.columns());
                            }
                        }
                    }

                    // Get the current plot limits
                    plotting_rect_limits_ = ImPlot::GetPlotLimits();

                    ImPlot::EndPlot();
                }
            
            ImGui::EndGroup();
        }
    }



    void save_to_json_internal(const std::string& node_name, nlohmann::json* json_file)
    {
        (*json_file)["nodes"][node_name]["type"] = node_type.c_str();

        x_axis_selector_ui_.save_to_json_internal(node_name, "x axis selector ui", json_file);
        y_axes_selector_ui_.save_to_json_internal(node_name, "y axis selector ui", json_file);
    }



private:

    MatrixType x_axis_;
    MatrixType y_axes_;

    MatrixType plotting_matrix_;

    ImPlotRect plotting_rect_limits_;

    SelectorUI x_axis_selector_ui_;
    SelectorUI y_axes_selector_ui_;

    Pin<MatrixType> input_pin_;
    Pin<MatrixType> output_pin_;

    static std::string node_type;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
std::string PlotNode::node_type = "Plot Node";
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_PLOT_NODE_HPP_