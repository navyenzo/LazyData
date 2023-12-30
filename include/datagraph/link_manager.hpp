//-------------------------------------------------------------------
/**
 * @file link_manager.hpp
 * @brief Defines the LinkManager class for the DataGraph namespace.
 *
 * The LinkManager class manages connections (links) between pins in a graph.
 * It maintains a record of all links and provides functionalities to create
 * new links and query or remove existing ones. This class ensures the integrity
 * of the graph's structure, particularly in preventing cycles during link creation.
 * 
 * @author Vincenzo Barbato
 * @link https://www.linkedin.com/in/vincenzobarbato/
 * @namespace DataGraph
 */
//-------------------------------------------------------------------



#ifndef DATAGRAPH_LINK_MANAGER_HPP
#define DATAGRAPH_LINK_MANAGER_HPP



//-------------------------------------------------------------------
#include <vector>
#include <memory>
#include <unordered_map>

#include "link.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
class LinkManager
{
public:

    LinkManager();

    bool create_link(std::shared_ptr<BasePin> output_pin, std::shared_ptr<BasePin> input_pin);
    std::vector<std::shared_ptr<BasePin>> get_connected_input_pins(int64_t output_pin_id) const;
    std::shared_ptr<BasePin> get_connected_output_pin(int64_t input_pin_id) const;
    bool is_pin_connected(int64_t pin_id) const;
    void remove_links_connected_to_pin(int64_t pin_id);
    bool remove_link(int64_t output_pin_id, int64_t input_pin_id);
    bool remove_link(int64_t link_id);
    void clear();
    const std::vector<std::shared_ptr<Link>>& get_links() const;

private:
    std::vector<std::shared_ptr<Link>> links_;
    std::unordered_map<int64_t, std::vector<std::shared_ptr<BasePin>>> output_to_input_;
    std::unordered_map<int64_t, std::shared_ptr<BasePin>> input_to_output_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// LinkManager Implementation
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline LinkManager::LinkManager()
{
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool LinkManager::create_link(std::shared_ptr<BasePin> output_pin,
                                     std::shared_ptr<BasePin> input_pin)
{
    if (output_pin->get_data_type() != input_pin->get_data_type())
    {
        return false; // Incompatible pin types.
    }

    auto link = std::make_shared<Link>(output_pin, input_pin);
    links_.push_back(link);
    output_to_input_[output_pin->get_id()].push_back(input_pin);
    input_to_output_[input_pin->get_id()] = output_pin;
    return true;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline std::vector<std::shared_ptr<BasePin>> LinkManager::get_connected_input_pins(int64_t output_pin_id) const
{
    auto it = output_to_input_.find(output_pin_id);
    return it != output_to_input_.end() ? it->second : std::vector<std::shared_ptr<BasePin>>{};
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline std::shared_ptr<BasePin> LinkManager::get_connected_output_pin(int64_t input_pin_id) const
{
    auto it = input_to_output_.find(input_pin_id);
    return it != input_to_output_.end() ? it->second : nullptr;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool LinkManager::is_pin_connected(int64_t pin_id) const
{
    return std::any_of(links_.begin(), links_.end(), [pin_id](const auto& link)
    {
        return link->get_input_pin()->get_id() == pin_id;
    });
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void LinkManager::remove_links_connected_to_pin(int64_t pin_id)
{
    // Remove links from the vector
    links_.erase(std::remove_if(links_.begin(), links_.end(), [pin_id](const auto& link)
    {
        return link->get_input_pin()->get_id() == pin_id || link->get_output_pin()->get_id() == pin_id;
    }), links_.end());

    // Update the connection maps
    output_to_input_.erase(pin_id);
    input_to_output_.erase(pin_id);
    
    for (auto& entry : output_to_input_)
    {
        entry.second.erase(std::remove_if(entry.second.begin(), entry.second.end(),
            [pin_id](const auto& pin) { return pin->get_id() == pin_id; }), entry.second.end());
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool LinkManager::remove_link(int64_t output_pin_id, int64_t input_pin_id)
{
    auto it = std::find_if(links_.begin(), links_.end(), 
                           [output_pin_id, input_pin_id](const auto& link)
                           {
                               return link->get_output_pin()->get_id() == output_pin_id &&
                                      link->get_input_pin()->get_id() == input_pin_id;
                           });

    if (it != links_.end())
    {
        // Update the connection maps
        output_to_input_[output_pin_id].erase(
            std::remove(output_to_input_[output_pin_id].begin(), 
                        output_to_input_[output_pin_id].end(), 
                        (*it)->get_input_pin()), 
            output_to_input_[output_pin_id].end());

        input_to_output_.erase(input_pin_id);

        // Remove the link
        links_.erase(it);
        return true;
    }

    return false; // Link not found
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool LinkManager::remove_link(int64_t link_id)
{
    auto it = std::find_if(links_.begin(), links_.end(), 
                           [link_id](const auto& link)
                           {
                               return link->get_id() == link_id;
                           });

    if (it != links_.end())
    {
        // Update the connection maps
        auto output_pin_id = (*it)->get_output_pin()->get_id();
        auto input_pin_id = (*it)->get_input_pin()->get_id();

        output_to_input_[output_pin_id].erase(
            std::remove(output_to_input_[output_pin_id].begin(), 
                        output_to_input_[output_pin_id].end(), 
                        (*it)->get_input_pin()), 
            output_to_input_[output_pin_id].end());

        input_to_output_.erase(input_pin_id);

        // Remove the link
        links_.erase(it);
        return true;
    }

    return false; // Link not found
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void LinkManager::clear()
{
    links_.clear();
    output_to_input_.clear();
    input_to_output_.clear();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline const std::vector<std::shared_ptr<Link>>& LinkManager::get_links() const
{
    return links_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // DATAGRAPH_LINK_MANAGER_HPP
