//-------------------------------------------------------------------
/**
 * @file pin.hpp
 * @brief Defines the BasePin and Pin classes for the DataGraph namespace.
 *
 * The BasePin class provides a common interface for all types of pins in the graph,
 * including a unique ID and a type (input or output) for each pin. The Pin class 
 * template extends BasePin for specific data types, allowing for type-safe connections 
 * in the computational graph. These classes are crucial for representing the input 
 * and output points in the graph nodes.
 * 
 * Each pin, being uniquely identifiable and typified, can be efficiently managed and 
 * connected within the graph. Additionally, each pin is associated with a node, and 
 * the node's ID can be retrieved through the pin.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



#ifndef DATAGRAPH_PIN_HPP
#define DATAGRAPH_PIN_HPP



//-------------------------------------------------------------------
#include <memory>
#include <typeinfo>
#include "constants_and_defaults.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Pin Types
//-------------------------------------------------------------------
enum class PinType : int
{
    Input = 0,
    Output
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class Node; // Forward declaration.
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class BasePin
{
public:

    BasePin(PinType type) : type_(type)
    {
    }

    virtual ~BasePin() = default;
    virtual const std::type_info& type() const = 0;
    virtual int64_t get_node_id() const = 0;

    int64_t get_id() const
    {
        return id_;
    }

    bool is_input() const
    {
        return type_ == PinType::Input;
    }

    bool is_output() const
    {
        return type_ == PinType::Output;
    }



private:

    int64_t id_ = IncrementalID::get_id();
    PinType type_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template <typename T>
class Pin : public BasePin
{
public:

    explicit Pin(Node* owner, PinType type) : BasePin(type), owner_(owner) {}

    const std::type_info& type() const override {
        return typeid(T);
    }

    std::shared_ptr<T> get_data() {
        return data_;
    }

    int64_t get_node_id() const override {
        return owner_->get_id();
    }



private:

    Node* owner_;
    std::shared_ptr<T> data_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // DATAGRAPH_PIN_HPP