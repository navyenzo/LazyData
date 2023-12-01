#ifndef INCLUDE_MATRIX_SOURCE_NODE_HPP_
#define INCLUDE_MATRIX_SOURCE_NODE_HPP_



//-------------------------------------------------------------------
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
const NodeStyling DEFAULT_MATRIX_SOURCE_NODE_STYLING(
                                                     ImVec4(135,220,167,255), // title bar color
                                                     ImVec4(165, 250, 197, 255), // title bar hovered color
                                                     ImVec4(165, 250, 197, 255), // title bar selected color
                                                     ImVec4(165, 250, 197, 255), // node outline color
                                                     400 // node expanded width
                                                    );
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This class represents a Node in a data flow node editor
//-------------------------------------------------------------------
class MatrixSourceNode : public Node<MatrixSourceNode>
{
public:

    MatrixSourceNode(std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback)
    : Node<MatrixSourceNode>(pin_deleted_link_manager_callback)
    {
        this->set_node_styling(DEFAULT_MATRIX_SOURCE_NODE_STYLING);

        output_pin_.update_data(&matrix_data_);
        output_pin_.set_name("out");
        output_pin_.set_pin_type(PinType::Output);
        output_pin_.set_parent_node_id(this->get_id());
    }

    ~MatrixSourceNode()
    {
        this->pin_deleted_link_manager_callback_(&output_pin_);
    }

    const std::string& get_node_type()const
    {
        return node_type;
    }



    Pin<MatrixType>* find_pin_using_id(int pin_id)
    {
        if(output_pin_.get_id() == pin_id)
            return &output_pin_;
        
        return nullptr;
    }

    int get_number_of_input_pins()const
    {
        return 0;
    }

    int get_number_of_output_pins()const
    {
        return 1;
    }
    
    
    
    void draw_input_pins()
    {
    }

    void draw_output_pins()
    {
        output_pin_.draw();
    }

    void draw_node_content()
    {
        ImGui::InputInt("rows", &rows_, 1, 10, ImGuiInputTextFlags_CharsDecimal);
        ImGui::InputInt("cols", &columns_, 1, 10, ImGuiInputTextFlags_CharsDecimal);

        if(rows_ < 0)
            rows_ = 0;
        if(columns_ < 0)
            columns_ = 0;

        ImGui::Dummy(ImVec2(0,10));

        ImGui::Combo("Select...", &selected_matrix_generator_type_, matrix_generator_types.data(), matrix_generator_types.size());

        ImGui::Dummy(ImVec2(0,10));

        switch(selected_matrix_generator_type_)
        {
            default:
            case 0: // Constant Matrix
                ImGui::InputDouble("Value", &constant_generator_initial_value_);
            break;

            case 1: // Increasing/Decreasing Matrix
                ImGui::InputDouble("Initial Value", &iota_generator_initial_value_, 0.0, 0.0, "%lf", ImGuiInputTextFlags_CharsDecimal);
                ImGui::InputDouble("Step", &iota_generator_step_value_, 0.0, 0.0, "%lf", ImGuiInputTextFlags_CharsDecimal);
            break;

            case 2: // Random Matrix
                ImGui::InputDouble("Min Value", &random_generator_min_value_, 0.0, 0.0, "%lf", ImGuiInputTextFlags_CharsDecimal);
                ImGui::InputDouble("Max Value", &random_generator_max_value_, 0.0, 0.0, "%lf", ImGuiInputTextFlags_CharsDecimal);
            break;

            case 3: // Sine wave Matrix
                ImGui::InputDouble("Amplitude", &sine_wave_amplitude_, 0.0, 0.0, "%lf", ImGuiInputTextFlags_CharsDecimal);
                ImGui::InputDouble("Frequency (Hz)", &sine_wave_frequency_, 0.0, 0.0, "%lf", ImGuiInputTextFlags_CharsDecimal);
                ImGui::InputDouble("Phase Offset (rad)", &sine_wave_phase_offset_in_radians_, 0.0, 0.0, "%lf", ImGuiInputTextFlags_CharsDecimal);
                ImGui::InputDouble("y offset", &sine_wave_y_offset_, 0.0, 0.0, "%lf", ImGuiInputTextFlags_CharsDecimal);
                ImGui::InputDouble("dt (sec)", &sine_wave_delta_time_, 0.0, 0.0, "%lf", ImGuiInputTextFlags_CharsDecimal);
                ImGui::InputDouble("initial time (sec)", &sine_wave_initial_time_, 0.0, 0.0, "%lf", ImGuiInputTextFlags_CharsDecimal);
            break;
        }
        
        if(ImGui::Button("Generate Matrix"))
        {
            switch(selected_matrix_generator_type_)
            {
                default:
                case 0: // Constant Matrix
                    matrix_data_.create_matrix(rows_, columns_, constant_generator_initial_value_);
                break;

                case 1: // Increasing/Descreasing Matrix
                    matrix_data_ = LazyMatrix::Iota<double>(rows_, columns_, iota_generator_initial_value_, iota_generator_step_value_);
                break;

                case 2: // Random Matrix
                    matrix_data_ = LazyMatrix::RandomMatrix<double>(rows_, columns_, random_generator_min_value_, random_generator_max_value_);
                break;

                case 3: // Sine wave Matrix
                    matrix_data_ = LazyMatrix::SineWaveMatrix<double>(rows_, sine_wave_amplitude_, sine_wave_frequency_, sine_wave_phase_offset_in_radians_, sine_wave_y_offset_, sine_wave_delta_time_, sine_wave_initial_time_);
                break;
            }
            
            output_pin_.update_data(&matrix_data_);
        }

        ImGui::Dummy(ImVec2(0,30));

        draw_matrix_table(matrix_data_, page_index_, ImVec2(this->get_node_width(),this->get_node_width()), are_entries_editable_);
    }



    void save_to_json_internal(const std::string& node_name, nlohmann::json* json_file)
    {
        (*json_file)["nodes"][node_name]["type"] = node_type.c_str();

        (*json_file)["nodes"][node_name]["selected matrix generator type"] = selected_matrix_generator_type_;
        (*json_file)["nodes"][node_name]["rows"] = rows_;
        (*json_file)["nodes"][node_name]["columns"] = columns_;

        (*json_file)["nodes"][node_name]["constant generator initial value"] = constant_generator_initial_value_;

        (*json_file)["nodes"][node_name]["iota generator initial value"] = iota_generator_initial_value_;
        (*json_file)["nodes"][node_name]["iota generator step value"] = iota_generator_step_value_;

        (*json_file)["nodes"][node_name]["random generator min value"] = random_generator_min_value_;
        (*json_file)["nodes"][node_name]["random generator max value"] = random_generator_max_value_;

        (*json_file)["nodes"][node_name]["sine wave amplitude"] = sine_wave_amplitude_;
        (*json_file)["nodes"][node_name]["sine wave frequency"] = sine_wave_frequency_;
        (*json_file)["nodes"][node_name]["sine wave phase offset in radians"] = sine_wave_phase_offset_in_radians_;
        (*json_file)["nodes"][node_name]["sine wave y offset"] = sine_wave_y_offset_;
        (*json_file)["nodes"][node_name]["sine wave delta time"] = sine_wave_delta_time_;
        (*json_file)["nodes"][node_name]["sine wave initial time"] = sine_wave_initial_time_;

        (*json_file)["nodes"][node_name]["matrix data"] = matrix_data_.get_filename_of_memory_mapped_file();
    }



private:

    Pin<MatrixType> output_pin_;

    MatrixType matrix_data_;

    int page_index_ = 0;

    bool are_entries_editable_ = true;

    int selected_matrix_generator_type_ = 0;
    static std::vector<const char*> matrix_generator_types;

    int rows_ = 0;
    int columns_ = 0;

    double constant_generator_initial_value_ = 0;

    double iota_generator_initial_value_ = 0;
    double iota_generator_step_value_ = 1;

    double random_generator_min_value_ = 0;
    double random_generator_max_value_ = 1;

    double sine_wave_amplitude_ = 1;
    double sine_wave_frequency_ = 1;
    double sine_wave_phase_offset_in_radians_ = 0;
    double sine_wave_y_offset_ = 0;
    double sine_wave_delta_time_ = 0.1;
    double sine_wave_initial_time_ = 0;

    static std::string node_type;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
std::string MatrixSourceNode::node_type = "Matrix Source Node";
std::vector<const char*> MatrixSourceNode::matrix_generator_types = {"Constant", "Iota", "Random", "Sine-Wave"};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_MATRIX_SOURCE_NODE_HPP_