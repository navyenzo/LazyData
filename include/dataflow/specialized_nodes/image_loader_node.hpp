#ifndef INCLUDE_IMAGE_LOADER_NODE_HPP_
#define INCLUDE_IMAGE_LOADER_NODE_HPP_



//-------------------------------------------------------------------
#include <imfilebrowser.h>

#include "../node_styling.hpp"
#include "../node.hpp"
#include "../matrix_table_ui.hpp"

#include <utils/file_browser.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
const NodeStyling DEFAULT_IMAGE_SOURCE_NODE_STYLING(ImVec4(135,100,167,255),
                                                    ImVec4(165, 130, 197, 255),
                                                    ImVec4(165, 130, 197, 255),
                                                    ImVec4(165, 130, 197, 255));
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This class represents a Node in a data flow node editor
//-------------------------------------------------------------------
class ImageLoaderNode : public Node<ImageLoaderNode>
{
public:

    ImageLoaderNode(std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback)
    : Node<ImageLoaderNode>(pin_deleted_link_manager_callback)
    {
        this->set_node_styling(DEFAULT_MATRIX_SOURCE_NODE_STYLING);

        output_pin_.update_data(&matrix_data_);
        output_pin_.set_name("out");
        output_pin_.set_pin_type(PinType::Output);
        output_pin_.set_parent_node_id(this->get_id());
    }

    ~ImageLoaderNode()
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
        if(ImGui::Button("Load Image"))
        {
            LazyApp::FileBrowserManager::open_file_browser(this->get_id(),
            { ".png", ".jpg", ".jpeg", ".bmp", ".tiff", ".tif", ".gif", ".svg" });
        }
        
        std::string selected_filename = LazyApp::FileBrowserManager::has_selected(this->get_id());

        if(!selected_filename.empty())
        {
            LazyMatrix::ImageMatrix<dlib::rgb_pixel> loaded_image(selected_filename);

            matrix_data_.resize(loaded_image.rows(), loaded_image.columns() * 3);    

            for(int i = 0; i < loaded_image.rows(); ++i)
            {
                for(int j = 0, k = 0; j < loaded_image.columns(); ++j, k+=3)
                {
                    matrix_data_(i,k) = loaded_image(i,j).red;
                    matrix_data_(i,k+1) = loaded_image(i,j).green;
                    matrix_data_(i,k+2) = loaded_image(i,j).blue;
                }
            }

            output_pin_.update_data(&matrix_data_);
        }

        ImGui::Dummy(ImVec2(0,30));

        draw_matrix_table(matrix_data_, page_index_, ImVec2(this->get_node_width(), -1), are_entries_editable_);
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
std::string ImageLoaderNode::node_type = "Matrix Source Node";
std::vector<const char*> ImageLoaderNode::matrix_generator_types = {"Constant", "Iota", "Random", "Sine-Wave"};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_IMAGE_LOADER_NODE_HPP_