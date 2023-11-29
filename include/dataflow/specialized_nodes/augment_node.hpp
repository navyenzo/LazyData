#ifndef INCLUDE_AUGMENT_NODE_HPP_
#define INCLUDE_AUGMENT_NODE_HPP_



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
const NodeStyling DEFAULT_AUGMENT_NODE_STYLING(ImVec4(135,167,220,255),
                                               ImVec4(165, 197, 250, 255),
                                               ImVec4(165, 197, 250, 255),
                                               ImVec4(165, 197, 250, 255),
                                               150.0f);
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This Node allows a user to augment multiple matrices together
//-------------------------------------------------------------------
class AugmentNode : public Node<AugmentNode>
{
public:

    AugmentNode(std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback)
    : Node<AugmentNode>(pin_deleted_link_manager_callback)
    {
        this->set_node_styling(DEFAULT_AUGMENT_NODE_STYLING);

        output_pin_.update_data(&resulting_matrix_);
        output_pin_.set_name("out");
        output_pin_.set_pin_type(PinType::Output);
        output_pin_.set_parent_node_id(this->get_id());

        input_pins_.reserve(20);

        add_input_pin();
        add_input_pin();
    }

    ~AugmentNode()
    {
        while(input_pins_.size() > 0)
            remove_last_input_pin();
        
        this->pin_deleted_link_manager_callback_(&output_pin_);
    }

    const std::string& get_node_type()const
    {
        return node_type;
    }



    Pin<MatrixType>* find_pin_using_id(int pin_id)
    {
        for(auto& pin : input_pins_)
        {
            if(pin.get_id() == pin_id)
            {
                return &pin;
            }
        }

        if(output_pin_.get_id() == pin_id)
            return &output_pin_;
        
        return nullptr;
    }

    int get_number_of_input_pins()const
    {
        return input_pins_.size();
    }

    int get_number_of_output_pins()const
    {
        return 1;
    }

    void add_input_pin()
    {
        input_pins_.emplace_back();
        input_pins_.back().set_pin_type(PinType::Input);
        input_pins_.back().set_name(std::to_string(input_pins_.size() - 1));
        input_pins_.back().set_parent_node_id(this->get_id());
        input_pins_.back().set_notify_parent_node_callback(std::bind(&AugmentNode::input_data_has_been_updated_callback, this));
    }

    void remove_last_input_pin()
    {
        if(input_pins_.size() > 0)
        {
            this->pin_deleted_link_manager_callback_(&input_pins_.back());
            input_pins_.erase(input_pins_.end() - 1);
        }
    }

    const std::vector< Pin<MatrixType> >& get_input_pins()const
    {
        return input_pins_;
    }



    void input_data_has_been_updated_callback()
    {
        resulting_matrix_.resize(0,0);
        
        for(auto& pin : input_pins_)
        {
            if(pin.get_data())
            {
                if(selected_augmentation_type_ == 0)
                {
                    MatrixType new_result = LazyMatrix::augment_by_rows(resulting_matrix_, *pin.get_data());
                    resulting_matrix_ = new_result;
                }
                else
                {
                    MatrixType new_result = LazyMatrix::augment_by_columns(resulting_matrix_, *pin.get_data());
                    resulting_matrix_ = new_result;
                }
            }
        }
        
        output_pin_.update_data(&resulting_matrix_);
    }
    
    
    
    void draw_input_pins()
    {
        for(auto& pin : input_pins_)
            pin.draw();
    }

    void draw_output_pins()
    {
        output_pin_.draw();
    }



    void draw_node_content()
    {
        ImGui::BeginGroup();
        
            // Add more input pins button
            ImGui::PushID(add_input_pins_button_id_);
            if(ImGui::Button("+"))
                add_input_pin();
            ImGui::PopID();
            
            // Remove input pins button
            ImGui::SameLine();
            if(this->get_number_of_input_pins() > 2)
            {
                ImGui::PushID(remove_input_pins_button_id_);
                if(ImGui::Button("-"))
                    this->remove_last_input_pin();
                ImGui::PopID();
            }

            // Type of operation selector
            ImGui::SameLine();
            ImGui::BeginGroup();
                ImGui::Text("Select what to augment");
                if(ImGui::Combo("", &selected_augmentation_type_, augmentation_types.data(), augmentation_types.size()))
                {
                    if(selected_augmentation_type_ != previously_selected_augmentation_type_)
                    {
                        previously_selected_augmentation_type_ = selected_augmentation_type_;
                        input_data_has_been_updated_callback();
                    }
                }
            ImGui::EndGroup();

        ImGui::EndGroup();
    }



    void save_to_json_internal(const std::string& node_name, nlohmann::json* json_file)
    {
        (*json_file)["nodes"][node_name]["type"] = node_type.c_str();

        (*json_file)["nodes"][node_name]["resulting matrix"] = resulting_matrix_.get_filename_of_memory_mapped_file();

        (*json_file)["nodes"][node_name]["selected augmentation type"] = selected_augmentation_type_;

        (*json_file)["nodes"][node_name]["number of input pins"] = input_pins_.size();

        for(int i = 0; i < input_pins_.size(); ++i)
        {
            (*json_file)["nodes"][node_name][std::string("input_pin ") + std::to_string(i)]["id"] = input_pins_[i].get_id();
        }
    }



private:

    int add_input_pins_button_id_ = LazyApp::UniqueID::generate_uuid_hash();
    int remove_input_pins_button_id_ = LazyApp::UniqueID::generate_uuid_hash();

    int selected_augmentation_type_ = 0;
    int previously_selected_augmentation_type_ = 0;

    MatrixType resulting_matrix_;

    std::vector< Pin<MatrixType> > input_pins_;
    Pin<MatrixType> output_pin_;

    static std::string node_type;
    static std::vector<const char*> augmentation_types;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
std::string AugmentNode::node_type = "Augment Node";
std::vector<const char*> AugmentNode::augmentation_types = {"rows", "columns"};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_AUGMENT_NODE_HPP_