//-------------------------------------------------------------------
/**
 * @file pin.hpp
 * @brief This file contains the implementation of the Pin class for the DataGraph project.
 * 
 * The Pin class represents a connection point in a node, which can be either an input or an output pin.
 * It manages connections to other pins and the flow of data between them. Each pin is associated with a specific data type.
 *
 * @author Vincenzo Barbato
 * @contact GitHub Project: https://github.com/navyenzo/LazyData.git
 *                LinkedIn: https://www.linkedin.com/in/vincenzobarbato/
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_PIN_HPP_
#define INCLUDE_PIN_HPP_


//-------------------------------------------------------------------
#include <string>
#include <vector>
#include <functional>
#include <memory>

#include <utils/unique_id.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Forward declarations
//-------------------------------------------------------------------
Class Node;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Represents a Pin in a data graph
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
    Pin();

    /**
     * @brief Destroy the Pin object, clearing all connections.
     */
    ~Pin();



    /**
     * @brief Getters and setters for the Pin properties.
     * 
     * Includes ID, parent node, name, and pin type.
     */   
    
    const std::string& get_id() const;
    void set_id(const std::string& id);

    void set_parent_node(Node* parent_node);
    Node* get_parent_node();
    const Node* get_parent_node() const;
    const std::string& get_parent_node_id() const;
    
    const std::string& get_name() const { return name_; }
    void set_name(const std::string& name) { name_ = name; }
    
    PinType get_pin_type() const { return pin_type_; }
    void set_pin_type(PinType pin_type) { pin_type_ = pin_type; }
    
    DataType* get_data();



    /**
     * @brief Check if the pin is currently connected.
     * 
     * @return true If the pin has active connections.
     * @return false If the pin has no active connections.
     */
    bool is_connected() const;

    /**
     * @brief Check if the pin can be connected to another pin.
     * 
     * @return true If the pin can be connected.
     * @return false If the pin cannot be connected.
     */
    bool can_pin_be_connected()const;
    


    /**
     * @brief Update the data associated with this pin and notify connected nodes.
     * 
     * @param data New data to associate with this pin.
     */
    void update_data(DataType* data);



    /**
     * @brief Methods to manage output and input links for the pin.
     */
    void add_output_link(Link<DataType>* output_link);
    void set_input_link(Link<DataType>* input_link);
    void remove_input_link();
    void remove_output_link(Link<DataType>* output_link);



private:

    std::string id_ = LazyApp::UniqueID::generate_uuid_string();

    Node* parent_node_ = nullptr;

    std::string name_;
    PinType pin_type_ = PinType::Output;

    DataType* data_ = nullptr;
    
    Link<DataType>* input_link_ = nullptr;
    std::unordered_map<std::string, Link<DataType>*> output_link_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function implementations
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>
inline Pin<DataType>::Pin()
{
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>
inline Pin<DataType>::~Pin()
{
    output_links_.clear();
    remove_input_link();
}
//-------------------------------------------------------------------




//-------------------------------------------------------------------
template<typename DataType>
inline const std::string& Pin<DataType>::get_id() const
{
    return id_;
}

template<typename DataType>
inline void Pin<DataType>::set_id(const std::string& id)
{
    id_ = id;
}

void set_parent_node(Node* parent_node);
Node* get_parent_node();
const Node* get_parent_node() const;
const std::string& get_parent_node_id() const;



template<typename DataType>
inline void Pin<DataType>::set_parent_node(Node* parent_node)
{
    parent_node_ = parent_node;
}

template<typename DataType>
inline Node* Pin<DataType>::get_parent_node()
{
    return parent_node_;
}

template<typename DataType>
inline const Node* Pin<DataType>::get_parent_node()const
{
    return parent_node_;
}

template<typename DataType>
inline const std::string& Pin<DataType>::get_name() const
{
    return name_;
}

template<typename DataType>
inline void Pin<DataType>::set_name(const std::string& name)
{
    name_ = name;
}

template<typename DataType>
inline PinType Pin<DataType>::get_pin_type() const
{
    return pin_type_;
}

template<typename DataType>
inline void Pin<DataType>::set_pin_type(PinType pin_type)
{
    pin_type_ = pin_type;
}

template<typename DataType>
inline bool Pin<DataType>::is_connected() const
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

template<typename DataType>
inline bool Pin<DataType>::can_pin_be_connected()const
{
    return pin_type_ == PinType::Input ? !is_connected() : true;
}

template<typename DataType>
inline DataType* Pin<DataType>::get_data() const
{
    return data_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>
inline void Pin<DataType>::update_data(DataType* data)
{
    data_ = data;
    if (pin_type_ == PinType::Input)
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
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename DataType>
inline void Pin<DataType>::add_output_link(Link<DataType>* output_link)
{
    output_links_.push_back(output_link);
}

template<typename DataType>
inline void Pin<DataType>::set_input_link(Link<DataType>* input_link)
{
    input_link_ = input_link;
}

template<typename DataType>
inline void Pin<DataType>::remove_input_link()
{
    input_link_ = nullptr;
    update_data(nullptr);
}

template<typename DataType>
inline void Pin<DataType>::remove_output_link(Link<DataType>* output_link)
{
    output_links_.erase(output_link->get_id());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif  // INCLUDE_PIN_HPP_
