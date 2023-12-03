#ifndef INCLUDE_NODE_HPP_
#define INCLUDE_NODE_HPP_



//-------------------------------------------------------------------
#include <nlohmann/json.hpp>
#include "pin.hpp"
#include "node_styling.hpp"
#include <app/imgui_helpers.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataFlow 
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Represents a Node in a data flow node editor.
 * 
 * This class uses the Curiously Recurring Template Pattern (CRTP) and defines the basic
 * functionality that every Node will have.
 * 
 * When defining a new type of Node, derive from this base class as shown:
 * "class NewNode : public Node<NewNode>"
 * 
 * The derived New Node class must define the following functions:
 * - const std::string& get_node_type() const
 * - int get_number_of_input_pins() const
 * - int get_number_of_output_pins() const
 * - Pin<MatrixType>* find_pin_using_id(int pin_id)
 * - void draw_node_content()
 * - void draw_input_pins()
 * - void draw_output_pins()
 * - void save_to_json_internal(const std::string& node_name, nlohmann::json* json_file)
 * 
 * @tparam NodeType The derived type that inherits from this class.
 */
//-------------------------------------------------------------------
template<typename NodeType>
class Node 
{
public:

    /**
     * @brief Copy constructor for Node, disabled to prevent copy constructing nodes.
     * 
     * @param node Reference to the Node being copied.
     */
    Node(const Node& node)
    {
        static_assert(true, "copy constructing nodes which is not allowed");
    }

    /**
     * @brief Destructor for Node.
     */
    ~Node()
    {
    }

    
    
    // Setters/Getters
    int get_id()const { return id_; }
    void set_id(int id) { id_ = id; }
    int get_title_id()const { return title_id_; }
    const std::string& get_title()const { return title_; }
    void set_title(const std::string& title) { title_ = title; title_.resize(MAX_NAME_LENGTH, 0); }
    const std::string& get_node_type()const { return this->underlying().get_node_type(); }
    const NodeStyling& get_node_styling()const { return node_styling_; }
    void set_node_styling(const NodeStyling& node_styling) { node_styling_ = node_styling; }
    int get_number_of_input_pins()const { return underlying().get_number_of_input_pins(); }
    int get_number_of_output_pins()const { return underlying().get_number_of_output_pins(); }



    /**
     * @brief Find a pin in the node using its ID.
     * 
     * @param pin_id ID of the pin to find.
     * @return Pin<MatrixType>* Pointer to the found pin, or nullptr if not found.
     */
    Pin<MatrixType>* find_pin_using_id(int pin_id)
    {
        return underlying().find_pin_using_id(pin_id);
    }

    /**
     * @brief Draws the input pins of the node.
     */
    void draw_input_pins()
    {
        underlying().draw_input_pins();
    }

    /**
     * @brief Draws the node's content
     */
    void draw_node_content()
    {
        underlying().draw_node_content();
    }

    /**
     * @brief Draws the output pins of the node.
     */
    void draw_output_pins()
    {
        underlying().draw_output_pins();
    }



    /**
     * @brief Retrieves the current width of the node.
     * 
     * This function determines the node's width based on
     * its current expanded/collapsed state.
     * 
     * @return float The width of the node in its current state (either expanded or collapsed).
     */
    float get_node_width()const
    {
        if(show_node_contents_)
            return node_styling_.get_node_width();
        else
            return node_styling_.get_node_collapsed_width();
    }



    /**
     * @brief Draws the Node in the node editor.
     * 
     * This method handles the overall drawing of the node, including the title bar,
     * node content, input pins, and output pins.
     */
    // This function draws the Node
    void draw()
    {
        // Apply node's styling
        node_styling_.push_styling();

        // Adjust the node's width depending on
        // whether the node's content is to be shown
        ImGui::PushItemWidth(this->get_node_width());

        // Begin drawing the node
        ImNodes::BeginNode(id_);

            this->draw_title_bar();

            // Spacing between title bar and node content
            ImGui::Dummy(ImVec2(0,20));

            // Collapsable section holding the node's content
            if(show_node_contents_)
            {
                ImGui::BeginGroup();
                    underlying().draw_node_content();
                ImGui::EndGroup();
            }

            // Draw the input pins
            ImGui::BeginGroup();
                this->draw_input_pins();
            ImGui::EndGroup();

            // Draw the output pins
            ImGui::SameLine(this->get_node_width());
            ImGui::BeginGroup();
                this->draw_output_pins();
            ImGui::EndGroup();

            // Handle node resizing
            if(show_node_contents_)
                handle_node_resizing();

        // End drawing the node
        ImNodes::EndNode();

        // Restore original styling
        node_styling_.pop_styling();
    }



    /**
     * @brief Handles mouse hovering over the Node.
     * 
     * This method shows tooltips and handles double-clicks for collapsing/expanding the node.
     */
    void handle_hovering()
    {
        // Show the node's tooltip if being hovered
        ImGui::BeginTooltip();
            ImGui::Text((this->get_node_type() + std::string(": ") + title_).c_str());
        ImGui::EndTooltip();

        // Double clicking on the node will make it
        // collapse/expand
        if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            show_node_contents_ = !show_node_contents_;
        }
    }



    /**
     * @brief Handles resizing of the node.
     */
    void handle_node_resizing()
    {
        // Get the top-left position of the node in screen space
        ImVec2 node_pos = ImNodes::GetNodeScreenSpacePos(id_);

        // Get the dimensions of the node
        ImVec2 node_dimensions = ImNodes::GetNodeDimensions(id_);

        // Calculate the position for the resizing handle at the bottom-right corner of the node
        ImVec2 resize_area_center = ImVec2(node_pos.x + node_dimensions.x, node_pos.y + node_dimensions.y);
        float handle_radius = 5.0f; // Adjust as needed
        ImVec2 resize_area_start = ImVec2(resize_area_center.x - handle_radius, resize_area_center.y - handle_radius);
        ImVec2 resize_area_end = ImVec2(resize_area_center.x + handle_radius, resize_area_center.y + handle_radius);

        // Check if mouse is hovering over the resizing handle
        bool is_hovering = ImGui::IsMouseHoveringRect(resize_area_start, resize_area_end);

        // Draw the resizing handle
        ImU32 handle_color = is_hovering ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 255, 255, 255);
        ImGui::GetWindowDrawList()->AddCircleFilled(resize_area_center, handle_radius, handle_color);

        // Show tooltip and change cursor when hovering or resizing
        if (is_hovering || is_resizing_)
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            ImGui::BeginTooltip();
            ImGui::Text("Drag to resize");
            ImGui::EndTooltip();
        }

        // Check for mouse interaction
        ImGuiIO& io = ImGui::GetIO();
        if (io.MouseClicked[0] && is_hovering)
        {
            is_resizing_ = true;
            initial_mouse_x_ = io.MousePos.x; // Store the initial mouse x position
        }
        else if (io.MouseReleased[0])
        {
            is_resizing_ = false;
        }

        if (is_resizing_)
        {
            // Calculate the amount of width change based on the mouse movement
            float width_change = io.MousePos.x - initial_mouse_x_;

            // Update the node width by the calculated amount
            node_styling_.change_node_width_by_amount(width_change);

            // Update initial mouse x position for the next frame
            initial_mouse_x_ = io.MousePos.x;
        }
    }



    /**
     * @brief Saves the current state of the Node as a JSON object.
     * 
     * @param json_file Pointer to the JSON object where the node's state is saved.
     */
    void save_to_json(nlohmann::json* json_file)
    {
        std::string node_name = std::string("node ") + std::to_string(id_);

        (*json_file)["nodes"][node_name]["title"] = title_.c_str();
        (*json_file)["nodes"][node_name]["x_pos"] = ImNodes::GetNodeGridSpacePos(id_).x;
        (*json_file)["nodes"][node_name]["y_pos"] = ImNodes::GetNodeGridSpacePos(id_).y;

        underlying().save_to_json_internal(node_name, json_file);
    }



private:

    /**
     * @brief Construct a new node 
     * 
     * This constructor is private to enforce the use of CRTP pattern, 
     * meaning that this base class should only be instantiated through 
     * derived classes. The derived class is declared as a friend to 
     * allow it to access this private constructor.
     * 
     * @param app_properties Application properties.
     */
    Node<NodeType>(std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback)
    {
        pin_deleted_link_manager_callback_ = pin_deleted_link_manager_callback;
    }

    friend NodeType;

    NodeType& underlying()
    {
        return static_cast<NodeType&>(*this);
    }

    const NodeType& underlying() const
    {
        return static_cast<const NodeType&>(*this);
    }



    /**
     * @brief Draws the title bar of the node.
     * 
     * This function is responsible for rendering the
     * title bar of a node within the node editor.
     */
    void draw_title_bar()
    {
        ImGui::PushItemWidth(this->get_node_width());

        ImNodes::BeginNodeTitleBar();

                ImGui::PushID(title_id_);
                ImGui::InputText("", title_.data(), title_.size());
                ImGui::PopID();
        
        ImNodes::EndNodeTitleBar();

        ImGui::PopItemWidth();
    }



    // New member variable for tracking resizing state
    bool is_resizing_ = false;

    float initial_mouse_x_ = 0;
    


    int id_ = LazyApp::UniqueID::generate_uuid_hash();

    int title_id_ = LazyApp::UniqueID::generate_uuid_hash();
    std::string title_ = std::string(MAX_NAME_LENGTH, 0);
    
    bool show_node_contents_ = true;
    NodeStyling node_styling_;
    bool selected_ = false;
    
    std::function<void(Pin<MatrixType>*)> pin_deleted_link_manager_callback_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_NODE_HPP_
