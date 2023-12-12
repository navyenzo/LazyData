#ifndef INCLUDE_UNARY_OPERATOR_NODE_HPP_
#define INCLUDE_UNARY_OPERATOR_NODE_HPP_



//-------------------------------------------------------------------
#include <vector>

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
const NodeStyling DEFAULT_UNARY_OPERATOR_NODE_STYLING(ImVec4(220,220,135,255),
                                                      ImVec4(250, 250, 165, 255),
                                                      ImVec4(250, 250, 165, 255),
                                                      ImVec4(250, 250, 165, 255),
                                                      ImVec2(100.0f,100.0f));
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This Node allows a user to augment multiple matrices together
//-------------------------------------------------------------------
class UnaryOperatorNode : public Node<UnaryOperatorNode>
{
public:

    UnaryOperatorNode(std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback)
    : Node<UnaryOperatorNode>(pin_deleted_link_manager_callback)
    {
        this->set_node_styling(DEFAULT_UNARY_OPERATOR_NODE_STYLING);
        
        output_pin_.update_data(&resulting_matrix_);
        output_pin_.set_name("out");
        output_pin_.set_pin_type(PinType::Output);
        output_pin_.set_parent_node_id(this->get_id());

        input_pin_.set_name("in");
        input_pin_.set_pin_type(PinType::Input);
        input_pin_.set_parent_node_id(this->get_id());
        input_pin_.set_notify_parent_node_callback(std::bind(&UnaryOperatorNode::input_data_has_been_updated_callback, this));
    }

    ~UnaryOperatorNode()
    {
        this->pin_deleted_link_manager_callback_(&input_pin_);
        this->pin_deleted_link_manager_callback_(&output_pin_);
    }

    const std::string& get_node_type()const
    {
        return node_type;
    }



    Pin<MatrixType>* find_pin_using_id(uintptr_t pin_id)
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
        if(input_pin_.get_data())
        {
            switch(selected_operation_type_)
            {
                default:
                case 0: // Transpose
                    resulting_matrix_ = LazyMatrix::transpose(*input_pin_.get_data());
                break;

                case 1: // Negate
                    resulting_matrix_ = -(*input_pin_.get_data());
                break;

                case 2: // sign
                    resulting_matrix_ = LazyMatrix::sign(*input_pin_.get_data());
                break;

                case 3: // abs
                    resulting_matrix_ = LazyMatrix::abs(*input_pin_.get_data());
                break;

                case 4: // sqrt
                    resulting_matrix_ = LazyMatrix::sqrt(*input_pin_.get_data());
                break;

                case 5: // exp (e^x)
                    resulting_matrix_ = LazyMatrix::exp(*input_pin_.get_data());
                break;

                case 6: // exp2 (2^x)
                    resulting_matrix_ = LazyMatrix::exp2(*input_pin_.get_data());
                break;
            }
        }
        else
        {
            resulting_matrix_.resize(0,0);
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
        ImGui::BeginGroup();

            // Type of operation selector
            ImGui::BeginGroup();
                ImGui::Text("Select operator type");
                if(ImGui::Combo("", &selected_operation_type_, operator_types.data(), operator_types.size()))
                {
                    if(selected_operation_type_ != previously_selected_operation_type_)
                    {
                        previously_selected_operation_type_ = selected_operation_type_;
                        input_data_has_been_updated_callback();
                    }
                }
            ImGui::EndGroup();

        ImGui::EndGroup();
    }



    void save_to_json_internal(const std::string& node_name, nlohmann::json* json_file)
    {
        (*json_file)["nodes"][node_name]["type"] = node_type.c_str();

        (*json_file)["nodes"][node_name]["selected operation type"] = selected_operation_type_;

        (*json_file)["nodes"][node_name]["resulting matrix"] = resulting_matrix_.get_filename_of_memory_mapped_file();
    }



private:

    int operation_type_selector_id_ = LazyApp::UniqueID::generate_uuid_hash();

    int selected_operation_type_ = 0;
    int previously_selected_operation_type_ = 0;

    MatrixType resulting_matrix_;

    Pin<MatrixType> input_pin_;
    Pin<MatrixType> output_pin_;

    static std::string node_type;
    static std::vector<const char*> operator_types;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
std::string UnaryOperatorNode::node_type = "Unary Operator Node";
std::vector<const char*> UnaryOperatorNode::operator_types = {"transpose", "negate", "sign", "abs", "sqrt", "exp", "exp2"};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_UNARY_OPERATOR_NODE_HPP_