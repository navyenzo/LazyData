#ifndef INCLUDE_PIN_HPP_
#define INCLUDE_PIN_HPP_


//-------------------------------------------------------------------
#include <string>
#include <vector>
#include <functional>
#include <memory>

#include "constants_and_defaults.hpp"
#include <app/toggle_button.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Represents a Pin in a data flow node editor.
 * 
 * This class manages connections in a node-based data flow architecture,
 * allowing data to be passed between nodes. It supports both input and output
 * types and handles interactions with linked nodes.
 * 
 * @tparam DataType The type of data this pin holds.
 */
//-------------------------------------------------------------------
template<typename DataType>
class Pin
{
public:

    /**
     * @brief Construct a new Pin object.
     */
    Pin()
    {
        this->initialize_toggle_button();
        output_links_.reserve(10);
    }

    /**
     * @brief Destroy the Pin object, clearing all connections.
     */
    ~Pin()
    {
        output_links_.clear();
        remove_input_link();
    }

    int get_id() const { return id_; }
    void set_id(int id) { id_ = id; }
    uintptr_t get_parent_node_id() const { return parent_node_id_; }
    void set_parent_node_id(uintptr_t parent_node_id) { parent_node_id_ = parent_node_id; }
    const std::string& get_name() const { return name_; }
    void set_name(const std::string& name) { name_ = name; }
    PinType get_pin_type() const { return pin_type_; }
    void set_pin_type(PinType pin_type) { pin_type_ = pin_type; }

    /**
     * @brief Check if the pin is currently connected.
     * 
     * @return true If the pin has active connections.
     * @return false If the pin has no active connections.
     */
    bool is_connected() const
    {
        if (pin_type_ == PinType::Input)
        {
            return input_link_ != nullptr;
        }
        else 
        {
            return !output_links_.empty();
        }
    }

    /**
     * @brief Check if the pin can be connected to another pin.
     * 
     * @return true If the pin can be connected.
     * @return false If the pin cannot be connected.
     */
    bool can_pin_be_connected()const
    {
        return pin_type_ == PinType::Input ? !is_connected() : true;
    }

    void set_notify_parent_node_callback(std::function<void(void)> callback)
    {
        notify_parent_node_callback_ = callback;
    }

    /**
     * @brief Update the data associated with this pin and notify connected nodes.
     * 
     * @param data New data to associate with this pin.
     */
    void update_data(DataType* data)
    {
        data_ = data;
        if (pin_type_ == PinType::Input && notify_parent_node_callback_)
        {
            notify_parent_node_callback_();
        }
        else
        {
            for (auto& link : output_links_)
            {
                link->output_pin_updated_data(data_);
            }
        }
    }

    DataType* get_data() { return data_; }

    void add_output_link(Link<DataType>* output_link) { output_links_.push_back(output_link); }
    void set_input_link(Link<DataType>* input_link) { input_link_ = input_link; }
    void remove_input_link() { input_link_ = nullptr; update_data(nullptr); }

    void remove_output_link(Link<DataType>* output_link)
    {
        for(auto iter = output_links_.begin(); iter != output_links_.end(); ++iter)
        {
            if((*iter)->get_id() == output_link->get_id())
            {
                output_links_.erase(iter);
                return;
            }
        }
    }
    


    void draw()
    {
        apply_pin_style<DataType>(is_connected());

        if(pin_type_ == PinType::Input)
        {
            ImNodes::BeginInputAttribute(id_, pick_pin_shape<DataType>(is_connected()));

            if(data_)
                ImGui::TextColored(ImVec4(1.0,1.0,0.0,1.0), "(%ix%i)", data_->rows(), data_->columns());
            else
                ImGui::TextColored(ImVec4(1.0,1.0,0.0,1.0), "(%ix%i)", 0, 0);
            
            ImNodes::EndInputAttribute();
        }
        else
        {
            ImNodes::BeginOutputAttribute(id_, pick_pin_shape<DataType>(is_connected()));

            std::string pin_data_size = "(0x0)";

            if(data_)
            {
                pin_data_size = "(";
                pin_data_size += std::to_string(data_->rows());
                pin_data_size += "x";
                pin_data_size += std::to_string(data_->columns());
                pin_data_size += ")";
            }

            toggle_button_->draw(&should_linked_pins_be_updated_if_data_changes_, pin_data_size);

            ImNodes::EndOutputAttribute();
        }
        
        ImNodes::PopColorStyle();
    }



private:

    // Function used to initialize the toggle button
    // used to pause/resume output pins notifying that
    // their underlying data has changed
    void initialize_toggle_button()
    {
        // First get the path to the executable's resources sub-folder
        auto executable_parent_path = LazyMatrix::get_absolute_path_of_executable_parent_directory();
        std::string resources_path = executable_parent_path.string() + "/resources/images/";

        // We then create the toggle button
        toggle_button_ = std::make_shared<LazyApp::ToggleButton>(resources_path + std::string("on_toggle.png"),
                                                                 resources_path + std::string("off_toggle.png"),
                                                                 ImVec2(30,30));
    }


    int id_ = LazyApp::UniqueID::generate_uuid_hash();
    int parent_node_id_ = 0;

    std::string name_;
    PinType pin_type_ = PinType::Output;

    DataType* data_ = nullptr;
    std::function<void(void)> notify_parent_node_callback_;

    bool should_linked_pins_be_updated_if_data_changes_ = true;
    int update_checkbox_id_ = LazyApp::UniqueID::generate_uuid_hash();

    Link<DataType>* input_link_ = nullptr;
    std::vector<Link<DataType>*> output_links_;

    std::shared_ptr<LazyApp::ToggleButton> toggle_button_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_PIN_HPP_
