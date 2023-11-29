#ifndef INCLUDE_ROI_NODE_HPP_
#define INCLUDE_ROI_NODE_HPP_



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
const NodeStyling DEFAULT_ROI_NODE_STYLING(ImVec4(30,220,50,255),
                                           ImVec4(60,250,80,255),
                                           ImVec4(60,250,80,255),
                                           ImVec4(60,250,80,255),
                                           150.0f);
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This Node allows a user to grab a Region Of Interest from an
// input matrix
//-------------------------------------------------------------------
class ROINode : public Node<ROINode>
{
public:

    ROINode(std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback)
    : Node<ROINode>(pin_deleted_link_manager_callback)
    {
        this->set_node_styling(DEFAULT_ROI_NODE_STYLING);
        
        output_pin_.update_data(&resulting_matrix_);
        output_pin_.set_name("out");
        output_pin_.set_pin_type(PinType::Output);
        output_pin_.set_parent_node_id(this->get_id());

        input_pin_.set_name("in");
        input_pin_.set_pin_type(PinType::Input);
        input_pin_.set_parent_node_id(this->get_id());
        input_pin_.set_notify_parent_node_callback(std::bind(&ROINode::input_data_has_been_updated_callback, this));
    }

    ~ROINode()
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
        if(input_pin_.get_data())
        {
            if(input_pin_.get_data()->size() > 0)
                resulting_matrix_ = LazyMatrix::roi(*input_pin_.get_data(), row1_, column1_, row2_, column2_);
            else
                resulting_matrix_.resize(0,0);
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

            ImGui::PushID(row1_id_);
            ImGui::InputInt("row 1", &row1_, 1, 20);
            ImGui::PopID();

            ImGui::PushID(column1_id_);
            ImGui::InputInt("column 1", &column1_, 1, 20);
            ImGui::PopID();

            ImGui::PushID(row2_id_);
            ImGui::InputInt("row 2", &row2_, 1, 20);
            ImGui::PopID();

            ImGui::PushID(column2_id_);
            ImGui::InputInt("columns 2", &column2_, 1, 20);
            ImGui::PopID();

            ImGui::Dummy(ImVec2(0,20));

            if(input_pin_.get_data())
            {
                if(ImGui::Button("Grab Region Of Interest (ROI)"))
                {
                    if(row1_ != previous_row1_ ||
                       column1_ != previous_column1_ ||
                       row2_ != previous_row2_ ||
                       column2_ != previous_column2_)
                    {
                        previous_row1_ = row1_;
                        previous_column1_ = column1_;
                        previous_row2_ = row2_;
                        previous_column2_ = column2_;

                        resulting_matrix_ = LazyMatrix::roi(*input_pin_.get_data(), row1_, column1_, row2_, column2_);

                        output_pin_.update_data(&resulting_matrix_);
                    }
                }
            }

        ImGui::EndGroup();
    }



    void save_to_json_internal(const std::string& node_name, nlohmann::json* json_file)
    {
        (*json_file)["nodes"][node_name]["type"] = node_type.c_str();

        (*json_file)["nodes"][node_name]["row1"] = row1_;
        (*json_file)["nodes"][node_name]["column1"] = column1_;
        
        (*json_file)["nodes"][node_name]["row2"] = row2_;
        (*json_file)["nodes"][node_name]["column2"] = column2_;

        (*json_file)["nodes"][node_name]["resulting matrix"] = resulting_matrix_.get_filename_of_memory_mapped_file();
    }



private:

    int row1_id_ = LazyApp::UniqueID::generate_uuid_hash();
    int column1_id_ = LazyApp::UniqueID::generate_uuid_hash();
    int row2_id_ = LazyApp::UniqueID::generate_uuid_hash();
    int column2_id_ = LazyApp::UniqueID::generate_uuid_hash();

    int row1_ = 0;
    int column1_ = 0;
    int row2_ = 0;
    int column2_ = 0;

    int previous_row1_ = 0;
    int previous_column1_ = 0;
    int previous_row2_ = 0;
    int previous_column2_ = 0;

    MatrixType resulting_matrix_;

    Pin<MatrixType> input_pin_;
    Pin<MatrixType> output_pin_;

    static std::string node_type;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
std::string ROINode::node_type = "Region Of Interest (ROI) Node";
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_ROI_NODE_HPP_