#ifndef INCLUDE_LINK_HPP_
#define INCLUDE_LINK_HPP_



//-------------------------------------------------------------------
#include <nlohmann/json.hpp>

#include "pin.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Structure representing an individual link
// -- It links an output pin of a node to an input pin of another node
// -- The link keeps pointers to the output and input pins
//    -- NOTE:  It does NOT check the validity of the pointers at any
//       time during runtime
//-------------------------------------------------------------------
template<typename DataType>

class Link
{
public:

    Link<DataType>()
    {
    }

    // This function connects an output pin to an input pin and
    // signals the connected input pin that the output pin's data
    // has been updated
    void connect(Pin<DataType>* output_pin, Pin<DataType>* input_pin)
    {
        output_pin_ = output_pin;
        input_pin_ = input_pin;
        
        output_pin_->add_output_link(this);
        input_pin_->set_input_link(this);
        
        output_pin_updated_data(output_pin->get_data());
    }

    // This function disconnects the connected pins
    // which also signals the input pin that the
    // linked data has been updated/changed
    void disconnect()
    {
        output_pin_->remove_output_link(this);
        input_pin_->remove_input_link();
    }

    // Setters/Getters
    int get_id()const
    {
        return id_;
    }

    void set_id(int id)
    {
        id_ = id;
    }
    
    Pin<DataType>* get_output_pin()const
    {
        return output_pin_;
    }
    
    Pin<DataType>* get_input_pin()const
    {
        return input_pin_;
    }



    // Function used to notify the connected input
    // pin that the linked output pin's data has been
    // updated/changed
    void output_pin_updated_data(DataType* data)
    {
        input_pin_->update_data(data);
    }



    void draw()const
    {
        ImNodes::Link(id_, output_pin_->get_id(), input_pin_->get_id());
    }



    nlohmann::json* save_to_json(nlohmann::json* json_file)
    {
        std::string link_name = std::string("link ") + std::to_string(id_);

        (*json_file)["links"][link_name]["input pin"] = input_pin_->get_id();
        (*json_file)["links"][link_name]["output pin"] = output_pin_->get_id();

        (*json_file)["links"][link_name]["input pin parent node"] = input_pin_->get_parent_node_id();
        (*json_file)["links"][link_name]["output pin parent node"] = output_pin_->get_parent_node_id();

        return json_file;
    }


private:

    int id_ = LazyApp::UniqueID::generate_uuid_hash();
    Pin<DataType>* input_pin_ = nullptr;
    Pin<DataType>* output_pin_ = nullptr;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_LINK_HPP_