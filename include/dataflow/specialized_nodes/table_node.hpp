#ifndef INCLUDE_TABLE_NODE_HPP_
#define INCLUDE_TABLE_NODE_HPP_



//-------------------------------------------------------------------
#include <vector>

#include <implot.h>

#include "../node_styling.hpp"
#include "../node.hpp"
#include "../matrix_table_ui.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
const NodeStyling DEFAULT_TABLE_NODE_STYLING(ImVec4(70,20,50,255),
                                             ImVec4(100, 50, 80, 255),
                                             ImVec4(100, 50, 80, 255),
                                             ImVec4(100, 50, 80, 255),
                                             300.0f);
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This Node allows a user to augment multiple matrices together
//-------------------------------------------------------------------
class TableNode : public Node<TableNode>
{
public:

    TableNode(std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback)
    : Node<TableNode>(pin_deleted_link_manager_callback)
    {
        this->set_node_styling(DEFAULT_TABLE_NODE_STYLING);

        output_pin_.set_name("out");
        output_pin_.set_pin_type(PinType::Output);
        output_pin_.set_parent_node_id(this->get_id());

        input_pin_.set_name("in");
        input_pin_.set_pin_type(PinType::Input);
        input_pin_.set_parent_node_id(this->get_id());
        input_pin_.set_notify_parent_node_callback(std::bind(&TableNode::input_data_has_been_updated_callback, this));
    }

    ~TableNode()
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

            if(draw_matrix_table(*input_pin_.get_data(), page_index_, ImVec2(this->get_node_width(),this->get_node_width()), are_entries_editable_))
            {
                // This means the user updated the data manually by changinge it
                // directly in the table
                // We might want to make use of this information
            }
        }
    }



    void save_to_json_internal(const std::string& node_name, nlohmann::json* json_file)
    {
        (*json_file)["nodes"][node_name]["type"] = node_type.c_str();

        (*json_file)["nodes"][node_name]["are entries editable"] = are_entries_editable_;
    }



private:

    Pin<MatrixType> input_pin_;
    Pin<MatrixType> output_pin_;

    bool are_entries_editable_ = true;

    int page_index_ = 0;

    static std::string node_type;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
std::string TableNode::node_type = "Table Node";
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_TABLE_NODE_HPP_