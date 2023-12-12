#ifndef INCLUDE_NODE_STYLING_HPP_
#define INCLUDE_NODE_STYLING_HPP_



//-------------------------------------------------------------------
#include <app/app_properties.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @class NodeStyling
 * @brief Represents the styling of a node in an ImGui node editor.
 *
 * This class holds style attributes for nodes, including colors for
 * various states and dimensions for node elements.
 */
//-------------------------------------------------------------------
class NodeStyling 
{
public:

    /**
     * @brief Construct a new Node Styling object with default or specified styles.
     * 
     * @param title_bar_color Default color of the node's title bar.
     * @param title_bar_hovered_color Color of the node's title bar when hovered.
     * @param title_bar_selected_color Color of the node's title bar when selected.
     * @param node_outline_color Color of the node's outline.
     * @param node_expanded_size Size of the node when expanded.
     * @param node_collapsed_size Size of the node when collapsed.
     * @param node_background_color Background color of the node.
     * @param node_background_hovered_color Background color of the node when hovered.
     * @param node_background_selected_color Background color of the node when selected.
     */
    NodeStyling(const ImVec4& title_bar_color = ImVec4(135/255.f, 167/255.f, 220/255.f, 1.0f),
                const ImVec4& title_bar_hovered_color = ImVec4(165/255.f, 197/255.f, 250/255.f, 1.0f),
                const ImVec4& title_bar_selected_color = ImVec4(165/255.f, 197/255.f, 250/255.f, 1.0f),
                const ImVec4& node_outline_color = ImVec4(0, 0, 0, 1.0f),
                const ImVec2& node_expanded_size = ImVec2(100.0f, 100.0f),
                const ImVec2& node_collapsed_size = ImVec2(30.0f, 30.0f),
                const ImVec4& node_background_color = ImVec4(15/255.f, 15/255.f, 25/255.f, 1.0f),
                const ImVec4& node_background_hovered_color = ImVec4(25/255.f, 25/255.f, 50/255.f, 1.0f),
                const ImVec4& node_background_selected_color = ImVec4(25/255.f, 25/255.f, 50/255.f, 1.0f))
        : title_bar_color_(title_bar_color),
          title_bar_hovered_color_(title_bar_hovered_color),
          title_bar_selected_color_(title_bar_selected_color),
          node_outline_color_(node_outline_color),
          node_expanded_size_(node_expanded_size),
          node_size_(node_expanded_size),
          node_collapsed_size_(node_collapsed_size),
          node_background_color_(node_background_color),
          node_background_hovered_color_(node_background_hovered_color),
          node_background_selected_color_(node_background_selected_color)
    {
    }



    /**
     * @brief Apply the styling to the current ImGui context.
     * 
     * This method pushes the current node styling settings onto the ImGui style stack.
     */
    void push_styling()
    {
        // First we save the current style settings
        saved_style_ = ImGui::GetStyle();

        // We then apply the style settings
        auto& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_TitleBg] = title_bar_color_;
        style.Colors[ImGuiCol_TitleBgActive] = title_bar_hovered_color_;
        style.Colors[ImGuiCol_TitleBgCollapsed] = title_bar_selected_color_;
    }



    /**
     * @brief Revert the styling changes made by push_styling.
     * 
     * This method should restore the original ImGui styles that were active before 
     * the push_styling method was called. The implementation depends on how styles 
     * are managed in the application.
     */
    void pop_styling()
    {
        ImGui::GetStyle() = saved_style_;
    }



    // Getters and setters
    const ImVec4& get_title_bar_color() const { return title_bar_color_; }
    void set_title_bar_color(const ImVec4& color) { title_bar_color_ = color; }

    const ImVec4& get_title_bar_hovered_color() const { return title_bar_hovered_color_; }
    void set_title_bar_hovered_color(const ImVec4& color) { title_bar_hovered_color_ = color; }

    const ImVec4& get_title_bar_selected_color() const { return title_bar_selected_color_; }
    void set_title_bar_selected_color(const ImVec4& color) { title_bar_selected_color_ = color; }


    const ImVec4& get_node_outline_color() const { return node_outline_color_; }
    void set_node_outline_color(const ImVec4& color) { node_outline_color_ = color; }

    const ImVec2& get_node_size() const { return node_size_; }
    void set_node_size(const ImVec2& node_size) { node_size_.x = std::max(node_expanded_size_.x, node_size.x); node_size_.y = std::max(node_expanded_size_.y, node_size.y); }
    void change_node_size_by_amount(const ImVec2& amount) { ImVec2 node_size = node_size_ + amount; this->set_node_size(node_size); }

    const ImVec2& get_node_expanded_size() const { return node_expanded_size_; }
    void set_node_expanded_size(const ImVec2& node_expanded_size) { node_expanded_size_ = node_expanded_size; }

    const ImVec2& get_node_collapsed_size() const { return node_collapsed_size_; }
    void set_node_collapsed_size(const ImVec2& node_collapsed_size) { node_collapsed_size_ = node_collapsed_size; }

    const ImVec4& get_node_background_color() const { return node_background_color_; }
    void set_node_background_color(const ImVec4& color) { node_background_color_ = color; }

    const ImVec4& get_node_background_hovered_color() const { return node_background_hovered_color_; }
    void set_node_background_hovered_color(const ImVec4& color) { node_background_hovered_color_ = color; }

    const ImVec4& get_node_background_selected_color() const { return node_background_selected_color_; }
    void set_node_background_selected_color(const ImVec4& color) { node_background_selected_color_ = color; }



protected:

    ImVec4 title_bar_color_;
    ImVec4 title_bar_hovered_color_;
    ImVec4 title_bar_selected_color_;

    ImVec4 node_outline_color_;

    ImVec2 node_expanded_size_;
    ImVec2 node_collapsed_size_;
    ImVec2 node_size_;
    
    ImVec4 node_background_color_;
    ImVec4 node_background_hovered_color_;
    ImVec4 node_background_selected_color_;

    ImGuiStyle saved_style_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif // INCLUDE_NODE_STYLING_HPP_
