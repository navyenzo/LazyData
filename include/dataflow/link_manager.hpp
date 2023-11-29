#ifndef INCLUDE_LINK_MANAGER_HPP_
#define INCLUDE_LINK_MANAGER_HPP_



//-------------------------------------------------------------------
#include <algorithm>

#include "link.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Link Manager
//-------------------------------------------------------------------
class LinkManager
{
public:

    LinkManager()
    {
        links_.reserve(1000);
    }

    ~LinkManager()
    {
        remove_all_links();
    }



    void handle_link_creation(NodeManager& node_manager)
    {
        int first_pin_id = 0;
        int second_pin_id = 0;
        
        if (ImNodes::IsLinkCreated(&first_pin_id, &second_pin_id))
        {
            Pin<MatrixType>* first_pin = node_manager.find_pin_using_id(first_pin_id);
            Pin<MatrixType>* second_pin = node_manager.find_pin_using_id(second_pin_id);

            add_link(first_pin, second_pin);
        }
    }

    void handle_hovering()
    {
        int hovered_link = 0;

        if(ImNodes::IsLinkHovered(&hovered_link))
        {
            for(auto& link : links_)
            {
                if(link.get_id() == hovered_link)
                {
                    // link.draw_tooltip();
                    return;
                }
            }
        }
    }

    void handle_link_deletion()
    {
        int number_of_selected_links = ImNodes::NumSelectedLinks();
        
        if(number_of_selected_links > 0)
        {
            std::vector<int> selected_links(number_of_selected_links, 0);
            ImNodes::GetSelectedLinks(selected_links.data());

            for (const auto& link_id : selected_links)
                remove_link(link_id);
        }
    }



    void add_link(Pin<MatrixType>* first_pin, Pin<MatrixType>* second_pin)
    {
        if(first_pin && second_pin)
        {
            if(first_pin->can_pin_be_connected() && second_pin->can_pin_be_connected())
            {
                if(first_pin->get_pin_type() == PinType::Output && second_pin->get_pin_type() == PinType::Input)
                {
                    links_.emplace_back();
                    links_.back().connect(first_pin, second_pin);
                }
                else if(first_pin->get_pin_type() == PinType::Input && second_pin->get_pin_type() == PinType::Output)
                {
                    links_.emplace_back();
                    links_.back().connect(second_pin, first_pin);
                }
            }
        }
    }
    


    bool remove_link(int link_id)
    {
        for(auto& link : links_)
        {
            if(link.get_id() == link_id)
            {
                link.disconnect();
                links_.erase(links_.begin() + (&link - &links_[0]));
                return true;
            }
        }

        return false;
    }

    void remove_link_that_belongs_to_pin(Pin<MatrixType>* pin_about_to_be_removed)
    {
        if(pin_about_to_be_removed->get_pin_type() == PinType::Input)
        {
            for(auto iter = links_.begin(); iter != links_.end(); ++iter)
            {
                if(iter->get_input_pin() == pin_about_to_be_removed)
                {
                    iter->disconnect();
                    links_.erase(iter);
                    return;
                }
            }
        }
        else
        {
            for(auto iter = links_.begin(); iter != links_.end(); ++iter)
            {
                if(iter->get_output_pin() == pin_about_to_be_removed)
                {
                    iter->disconnect();
                    links_.erase(iter--);
                }
            }
        }
    }

    void remove_all_links()
    {
        for(auto& link : links_)
            link.disconnect();
        
        links_.clear();
    }



    void draw()const
    {
        for(auto& link : links_)
            link.draw();
    }



    nlohmann::json* save_to_json(NodeManager& node_manager, nlohmann::json* json_file)
    {
        (*json_file)["links"]["number of links"] = links_.size();
        
        std::vector<int> link_ids;        
        for(auto& link : links_)
            link_ids.push_back(link.get_id());

        (*json_file)["links"]["link IDs"] = link_ids;

        for(auto& link : links_)
            json_file = link.save_to_json(json_file);

        return json_file;
    }



private:

    std::vector< Link<MatrixType> > links_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_LINK_MANAGER_HPP_