#ifndef INCLUDE_SELECTOR_NODE_HPP_
#define INCLUDE_SELECTOR_NODE_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <unordered_map>

#include "../node_styling.hpp"
#include "../node.hpp"
#include "../selector_ui.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
const NodeStyling DEFAULT_SELECTOR_NODE_STYLING(ImVec4(120,60,50,255),
                                                ImVec4(150, 90, 80, 255),
                                                ImVec4(150, 90, 80, 255),
                                                ImVec4(150, 90, 80, 255),
                                                ImVec2(100.0f, 100.0f));
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This Node allows a user to augment multiple matrices together
//-------------------------------------------------------------------
class SelectorNode : public Node<SelectorNode>
{
public:

    SelectorNode(std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback)
    : Node<SelectorNode>(pin_deleted_link_manager_callback)
    {
        this->set_node_styling(DEFAULT_SELECTOR_NODE_STYLING);
        
        output_pin_.update_data(&resulting_matrix_);
        output_pin_.set_name("out");
        output_pin_.set_pin_type(PinType::Output);
        output_pin_.set_parent_node_id(this->get_id());

        input_pin_.set_name("in");
        input_pin_.set_pin_type(PinType::Input);
        input_pin_.set_parent_node_id(this->get_id());
        input_pin_.set_notify_parent_node_callback(std::bind(&SelectorNode::input_data_has_been_updated_callback, this));
    }

    ~SelectorNode()
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
        if(!input_pin_.get_data())
            resulting_matrix_.resize(0,0);

        else if(input_pin_.get_data()->size() == 0)
            resulting_matrix_.resize(0,0);

        else if(selector_ui_.get_selected_rows_vector().size() == 0 ||
                selector_ui_.get_selected_columns_vector().size() == 0)
        {
            resulting_matrix_.resize(0,0);
        }
        else
        {
            resulting_matrix_ = LazyMatrix::select_rows_and_columns(*input_pin_.get_data(),
                                                                    selector_ui_.get_selected_rows_vector(),
                                                                    selector_ui_.get_selected_columns_vector());
        }
        
        output_pin_.update_data(&resulting_matrix_);
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
                if(selector_ui_.draw(*input_pin_.get_data(), true, true, true))
                {
                    if(selector_ui_.get_selected_rows_vector().size() == 0 || selector_ui_.get_selected_columns_vector().size() == 0)
                    {
                        resulting_matrix_.resize(0,0);
                    }
                    else
                    {
                        resulting_matrix_ = LazyMatrix::select_rows_and_columns(*input_pin_.get_data(),
                                                                                selector_ui_.get_selected_rows_vector(),
                                                                                selector_ui_.get_selected_columns_vector());
                    }
                }
            }
        }
    }



    void save_to_json_internal(const std::string& node_name, nlohmann::json* json_file)
    {
        (*json_file)["nodes"][node_name]["type"] = node_type.c_str();

        selector_ui_.save_to_json_internal(node_name, "selector ui", json_file);
    }



private:

    MatrixType resulting_matrix_;

    SelectorUI selector_ui_;

    Pin<MatrixType> input_pin_;
    Pin<MatrixType> output_pin_;

    static std::string node_type;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
std::string SelectorNode::node_type = "Selector Node";
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_SELECTOR_NODE_HPP_