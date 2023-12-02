#ifndef INCLUDE_SELECTOR_UI_HPP_
#define INCLUDE_SELECTOR_UI_HPP_



//-------------------------------------------------------------------
#include <vector>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "constants_and_defaults.hpp"
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Class that represents the UI for selecting rows and columns
//-------------------------------------------------------------------
class SelectorUI
{
public:

    SelectorUI()
    {
    }



    bool draw(MatrixType& matrix,
              bool are_we_selecting_rows,
              bool are_we_selecting_columns,
              bool can_user_select_multiple,
              const std::string& rows_selection_title = "Select rows",
              const std::string& columns_selection_title = "Select columns")
    {
        bool was_data_selected_or_deselected = false;

        if(are_we_selecting_rows)
            was_data_selected_or_deselected = draw_row_selection(matrix, rows_selection_title, can_user_select_multiple);

        if(are_we_selecting_columns)
        {
            ImGui::SameLine();
            was_data_selected_or_deselected = was_data_selected_or_deselected || draw_column_selection(matrix, columns_selection_title, can_user_select_multiple);
        }

        previous_number_of_rows_ = matrix.rows();
        previous_number_of_columns_ = matrix.columns();

        return was_data_selected_or_deselected;
    }



    const std::unordered_map<std::string, int64_t>& get_selected_rows()const
    {
        return selected_rows_;
    }

    const std::unordered_map<std::string, int64_t>& get_selected_columns()const
    {
        return selected_columns_;
    }

    const std::vector<int64_t>& get_selected_rows_vector()const
    {
        return selected_rows_vector_;
    }

    const std::vector<int64_t>& get_selected_columns_vector()const
    {
        return selected_columns_vector_;
    }



    void save_to_json_internal(const std::string& node_name, const std::string& selector_ui_name, nlohmann::json* json_file)
    {
        (*json_file)["nodes"][node_name][selector_ui_name]["selected rows"] = selected_rows_vector_;
        (*json_file)["nodes"][node_name][selector_ui_name]["selected columns"] = selected_columns_vector_;

        (*json_file)["nodes"][node_name][selector_ui_name]["are all rows selected"] = are_all_rows_selected_;
        (*json_file)["nodes"][node_name][selector_ui_name]["are all columns selected"] = are_all_columns_selected_;
    }



private: // Private functions

    bool draw_row_selection(MatrixType& matrix,
                            const std::string& rows_selection_title,
                            bool can_user_select_multiple)
    {
        bool were_rows_selected_or_deselected = false;

        // Check if matrix size has shrunk since last time
        // and unselect the rows that are no longer there
        if(matrix.rows() < previous_number_of_rows_)
        {
            for(int i = matrix.rows() - 1; i < previous_number_of_rows_; ++i)
                selected_rows_.erase(std::to_string(i));

                were_rows_selected_or_deselected = true;
        }
        
        ImGui::BeginGroup();

            bool is_row_selected = false;
                
            ImGui::Text("%s", rows_selection_title.c_str());

            std::string list_box_title = std::string("##") + rows_selection_title;
            
            if(ImGui::BeginListBox(list_box_title.c_str()))
            {
                for(int row_index = 0; row_index < matrix.rows(); ++row_index)
                {
                    if(selected_rows_.find(std::to_string(row_index)) != selected_rows_.end())
                    {
                        is_row_selected = true;
                    }

                    if(ImGui::Checkbox((std::string("##") + std::to_string(row_index)).c_str(), &is_row_selected))
                    {
                        if(is_row_selected)
                        {
                            if(!can_user_select_multiple)
                                selected_rows_.clear();

                            selected_rows_[std::to_string(row_index)] = row_index;
                            were_rows_selected_or_deselected = true;
                        }
                        else
                        {
                            selected_rows_.erase(std::to_string(row_index));
                            are_all_rows_selected_ = false;
                            were_rows_selected_or_deselected = true;
                        }
                    }

                    ImGui::SameLine();

                    if(ImGui::Selectable((std::string("row ") + std::to_string(row_index)).c_str(), &is_row_selected))
                    {
                        if(is_row_selected)
                        {
                            if(!can_user_select_multiple)
                                selected_rows_.clear();

                            selected_rows_[std::to_string(row_index)] = row_index;
                            were_rows_selected_or_deselected = true;
                        }
                        else
                        {
                            selected_rows_.erase(std::to_string(row_index));
                            are_all_rows_selected_ = false;
                            were_rows_selected_or_deselected = true;
                        }
                    }

                    is_row_selected = false;
                }

                ImGui::EndListBox();
            }

            if(can_user_select_multiple)
            {
                if(ImGui::Checkbox("Select all##rows", &are_all_rows_selected_))
                {
                    if(are_all_rows_selected_)
                    {
                        for(int i = 0; i < matrix.rows(); ++i)
                            selected_rows_[std::to_string(i)] = i;

                        were_rows_selected_or_deselected = true;
                    }
                    else
                    {
                        selected_rows_.clear();
                        were_rows_selected_or_deselected = true;
                    }
                }
            }

            if(were_rows_selected_or_deselected)
            {
                selected_rows_vector_.clear();

                for(const auto& row : selected_rows_)
                    selected_rows_vector_.push_back(row.second);
                
                std::sort(selected_rows_vector_.begin(), selected_rows_vector_.end());
            }

        ImGui::EndGroup();

        return were_rows_selected_or_deselected;
    }



    bool draw_column_selection(MatrixType& matrix,
                               const std::string& columns_selection_title,
                               bool can_user_select_multiple)
    {
        bool were_columns_selected_or_deselected = false;

        // Check if matrix size has shrunk since last time
        // and unselect the rows that are no longer there
        if(matrix.columns() < previous_number_of_columns_)
        {
            for(int i = matrix.columns() - 1; i < previous_number_of_columns_; ++i)
                selected_columns_.erase(std::to_string(i));

            were_columns_selected_or_deselected = true;
        }
        
        ImGui::BeginGroup();

            bool is_column_selected = false;
                
            ImGui::Text("%s", columns_selection_title.c_str());
            
            std::string list_box_title = std::string("##") + columns_selection_title;
            
            if(ImGui::BeginListBox(list_box_title.c_str()))
            {
                for(int column_index = 0; column_index < matrix.columns(); ++column_index)
                {
                    if(selected_columns_.find(std::to_string(column_index)) != selected_columns_.end())
                    {
                        is_column_selected = true;
                    }

                    if(ImGui::Checkbox((std::string("##") + std::to_string(column_index)).c_str(), &is_column_selected))
                    {
                        if(is_column_selected)
                        {
                            if(!can_user_select_multiple)
                                selected_columns_.clear();
                                
                            selected_columns_[std::to_string(column_index)] = column_index;
                            were_columns_selected_or_deselected = true;
                        }
                        else
                        {
                            selected_columns_.erase(std::to_string(column_index));
                            are_all_columns_selected_ = false;
                            were_columns_selected_or_deselected = true;
                        }
                    }

                    ImGui::SameLine();

                    if(ImGui::Selectable((std::string("col ") + std::to_string(column_index)).c_str(), &is_column_selected))
                    {
                        if(is_column_selected)
                        {
                            if(!can_user_select_multiple)
                                selected_columns_.clear();

                            selected_columns_[std::to_string(column_index)] = column_index;
                            were_columns_selected_or_deselected = true;
                        }
                        else
                        {
                            selected_columns_.erase(std::to_string(column_index));
                            are_all_columns_selected_ = false;
                            were_columns_selected_or_deselected = true;
                        }
                    }

                    is_column_selected = false;
                }

                ImGui::EndListBox();
            }

            if(can_user_select_multiple)
            {
                if(ImGui::Checkbox("Select all##columns", &are_all_columns_selected_))
                {
                    if(are_all_columns_selected_)
                    {
                        for(int i = 0; i < matrix.columns(); ++i)
                            selected_columns_[std::to_string(i)] = i;

                        were_columns_selected_or_deselected = true;
                    }
                    else
                    {
                        selected_columns_.clear();
                        were_columns_selected_or_deselected = true;
                    }
                }
            }

            if(were_columns_selected_or_deselected)
            {
                selected_columns_vector_.clear();

                for(const auto& column : selected_columns_)
                    selected_columns_vector_.push_back(column.second);
                
                std::sort(selected_columns_vector_.begin(), selected_columns_vector_.end());
            }

        ImGui::EndGroup();

        return were_columns_selected_or_deselected;
    }



private: // Private variables

    std::unordered_map<std::string, int64_t> selected_rows_;
    std::unordered_map<std::string, int64_t> selected_columns_;

    bool are_all_rows_selected_ = false;
    bool are_all_columns_selected_ = false;

    std::vector<int64_t> selected_rows_vector_;
    std::vector<int64_t> selected_columns_vector_;

    int previous_number_of_rows_ = 0;
    int previous_number_of_columns_ = 0;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_SELECTOR_UI_HPP_