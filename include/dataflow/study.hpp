#ifndef INCLUDE_STUDY_HPP_
#define INCLUDE_STUDY_HPP_



//-------------------------------------------------------------------
#include "link_manager.hpp"
#include "node_manager.hpp"

#include "specialized_nodes/specialized_nodes.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Study
//-------------------------------------------------------------------
class Study
{
public:

    Study();
    ~Study();

    void set_name(const std::string& name);
    const std::string& get_name()const;

    bool get_is_study_open()const;
    bool get_is_study_active()const;

    template<typename TypeOfNode>
    TypeOfNode& add_node(std::string node_title = "new node");

    bool draw();
    
    static void mini_map_node_hovering_callback(int node_id, void* user_data);



private: // Private functions

    void handle_drag_and_drop();
    void handle_popup_context_menu();
    void handle_popup_context_menu_answer();



private: // Private variables

    int id_ = LazyApp::UniqueID::generate_uuid_hash();

    bool is_study_open_ = true;
    bool is_study_active_ = false;

    std::string name_ = "Study";
    std::string study_identifying_name_ = "Study";

    int popup_context_menu_answer_ = -1;

    ImVec2 size_ = ImVec2(0.0f, 0.0f);

    ImNodesEditorContext* editor_context_ = nullptr;

    DataFlow::NodeManager node_manager_;

    DataFlow::LinkManager link_manager_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline Study::Study()
{
    // The fixed ImGui Window ID that allows us to
    // have dynamic window titles
    study_identifying_name_ = std::string("Study:") + std::to_string(id_);

    // Initialize the imnodes editor context
    editor_context_ = ImNodes::EditorContextCreate();
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline Study::~Study()
{
    if(editor_context_)
    {
        ImNodes::EditorContextFree(editor_context_);
        editor_context_ = nullptr;
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Study::set_name(const std::string& name)
{
    name_ = name;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline const std::string& Study::get_name()const
{
    return name_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Study::get_is_study_open()const
{
    return is_study_open_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Study::get_is_study_active()const
{
    return is_study_active_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename TypeOfNode>

inline TypeOfNode& Study::add_node(std::string node_title)
{
    auto& new_node = node_manager_.construct_and_add_node<TypeOfNode>(std::bind(&LinkManager::remove_link_that_belongs_to_pin, &link_manager_, std::placeholders::_1));
    new_node.set_title(node_title);
    ImNodes::SetNodeGridSpacePos(new_node.get_id(), ImVec2(-ImNodes::EditorContextGetPanning().x, -ImNodes::EditorContextGetPanning().y));
    return new_node;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline bool Study::draw()
{
    if(is_study_open_)
    {
        if(ImGui::BeginTabItem(study_identifying_name_.c_str(), &is_study_open_))
        {
            ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelY);

            is_study_active_ = true;

            if(ImGui::Button("save"))
            {
                nlohmann::json json_file;

                node_manager_.save_to_json(&json_file);
                link_manager_.save_to_json(node_manager_, &json_file);

                std::cout << "json file =\n\n" << json_file.dump(4) << "\n\n\n";
            }
            
            ImNodes::EditorContextSet(editor_context_);

            ImNodes::BeginNodeEditor();

                node_manager_.draw();
                link_manager_.draw();

                handle_popup_context_menu();

                if(node_manager_.size() > 0)
                {
                    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight, &Study::mini_map_node_hovering_callback, nullptr);
                }
            
            ImNodes::EndNodeEditor();

            link_manager_.handle_hovering();
            node_manager_.handle_hovering();

            link_manager_.handle_link_creation(node_manager_);

            if(ImGui::IsKeyReleased(ImGuiKey_Delete))
            {
                link_manager_.handle_link_deletion();
                node_manager_.handle_node_deletion();
            }

            handle_drag_and_drop();

            handle_popup_context_menu_answer();
        
            ImGui::EndTabItem();
        }
        else
        {
            is_study_active_ = false;
        }
    }

    return is_study_open_;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Study::mini_map_node_hovering_callback(int node_id, void* user_data)
{
    ImGui::SetTooltip("Node: %d", node_id);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Study::handle_drag_and_drop()
{
    if(ImGui::BeginDragDropTarget())
    {
        if(ImGui::AcceptDragDropPayload("MATRIX_SOURCE_NODE"))
        {
            auto& new_node = add_node<MatrixSourceNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }

        if(ImGui::AcceptDragDropPayload("IMAGE_LOADER_NODE"))
        {
            auto& new_node = add_node<ImageLoaderNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }

        if(ImGui::AcceptDragDropPayload("UNARY_OPERATOR_NODE"))
        {
            auto& new_node = add_node<UnaryOperatorNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }

        if(ImGui::AcceptDragDropPayload("AUGMENT_NODE"))
        {
            auto& new_node = add_node<AugmentNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }

        if(ImGui::AcceptDragDropPayload("TABLE_NODE"))
        {
            auto& new_node = add_node<TableNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }

        if(ImGui::AcceptDragDropPayload("PLOT_NODE"))
        {
            auto& new_node = add_node<PlotNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }

        if(ImGui::AcceptDragDropPayload("HEAT_MAP_NODE"))
        {
            auto& new_node = add_node<HeatMapNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }

        if(ImGui::AcceptDragDropPayload("ROI_NODE"))
        {
            auto& new_node = add_node<ROINode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }

        if(ImGui::AcceptDragDropPayload("SELECTOR_NODE"))
        {
            auto& new_node = add_node<SelectorNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }

        ImGui::EndDragDropTarget();
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Study::handle_popup_context_menu()
{
    if (ImGui::BeginPopupContextWindow())
    {
        ImGui::Dummy(ImVec2(0,20));

        ImGui::Dummy(ImVec2(20,0));
        ImGui::SameLine();

        ImGui::BeginGroup();

            ImGui::Separator();
            ImGui::TextColored(ImVec4(1,1,0,1), "Add a Node...");
            ImGui::Separator();

            if(ImGui::Selectable(" * Matrix Source Node"))
                popup_context_menu_answer_ = 0;
            if(ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

            if(ImGui::Selectable(" * Image Loader Node"))
                popup_context_menu_answer_ = 0;
            if(ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

            if(ImGui::Selectable(" * Unary Operator Node"))
                popup_context_menu_answer_ = 1;
            if(ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                
            if(ImGui::Selectable(" * Augment Node"))
                popup_context_menu_answer_ = 2;
            if(ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                
            if(ImGui::Selectable(" * Table Node"))
                popup_context_menu_answer_ = 3;
            if(ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                
            if (ImGui::Selectable(" * Plot Node"))
                popup_context_menu_answer_ = 4;
            if(ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                
            if(ImGui::Selectable(" * Heat Map Node"))
                popup_context_menu_answer_ = 5;
            if(ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                
            if (ImGui::Selectable(" * Region Of Interest (ROI) Node"))
                popup_context_menu_answer_ = 6;
            if(ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                
            if (ImGui::Selectable(" * Selector Node"))
                popup_context_menu_answer_ = 7;
            if(ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        
        ImGui::EndGroup();

        ImGui::Dummy(ImVec2(0,20));
    
        ImGui::EndPopup();
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
inline void Study::handle_popup_context_menu_answer()
{
    switch(popup_context_menu_answer_)
    {
        default:
        break;

        case 0: // Matrix Source Node
        {
            auto& new_node = add_node<MatrixSourceNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }
        break;

        case 1: // Image Loader Node
        {
            auto& new_node = add_node<ImageLoaderNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }
        break;

        case 2: // Unary Operator Node
        {
            auto& new_node = add_node<UnaryOperatorNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }
        break;

        case 3: // Augment Node
        {
            auto& new_node = add_node<AugmentNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }
        break;

        case 4: // Table Node
        {
            auto& new_node = add_node<TableNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }
        break;

        case 5: // Plot Node
        {
            auto& new_node = add_node<PlotNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }
        break;

        case 6: // Heat Map Node
        {
            auto& new_node = add_node<HeatMapNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }
        break;

        case 7: // Region Of Interest (ROI) Node
        {
            auto& new_node = add_node<ROINode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }
        break;

        case 8: // Selector Node
        {
            auto& new_node = add_node<SelectorNode>();
            ImNodes::SetNodeScreenSpacePos(new_node.get_id(), ImGui::GetMousePos());
        }
        break;
    }

    // Reset the answer so we only add
    // the requested node once
    popup_context_menu_answer_ = -1;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_STUDY_HPP_