#ifndef INCLUDE_DATA_FLOW_CONSTANTS_AND_DEFAULTS_HPP_
#define INCLUDE_DATA_FLOW_CONSTANTS_AND_DEFAULTS_HPP_



//-------------------------------------------------------------------
#include <string>
#include <vector>
#include <functional>
#include <complex>

#include <app/base_node_editor_app.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Define every thing within the namespace DataFlow
//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Constants used in the data-flow node editor
//-------------------------------------------------------------------

// Maximum length for names (used for GUI interface)
static const int MAX_NAME_LENGTH = 1000;

//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Data Types used throughout DataFlow
//-------------------------------------------------------------------
using MatrixType = LazyMatrix::Matrix<double>;
using MatrixUpdatedCallback = std::function<void(const MatrixType&)>;

using ComplexMatrixType = LazyMatrix::Matrix< std::complex<double> >;
using ComplexMatrixUpdatedCallback = std::function<void(const ComplexMatrixType&)>;

using Color3MatrixType = LazyMatrix::Matrix< dlib::rgb_pixel >;
using Color3MatrixUpdatedCallback = std::function<void(const Color3MatrixType&)>;

using Color4MatrixType = LazyMatrix::Matrix< dlib::rgb_alpha_pixel >;
using Color4MatrixUpdatedCallback = std::function<void(const Color4MatrixType&)>;

using CsvMatrixType = LazyMatrix::CSVMatrix<std::string>;

using VectorOfMatricesType = std::vector<MatrixType>;
using VectorOfMatricesUpdatedCallback = std::function<void(const VectorOfMatricesType&)>;

using VectorOfComplexMatricesType = std::vector<ComplexMatrixType>;
using VectorOfComplexMatricesUpdatedCallback = std::function<void(const VectorOfComplexMatricesType&)>;
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
// Forward Declarations
//-------------------------------------------------------------------
template<typename DataType>
class Pin;

template<typename DataType>
class Link;

template<typename NodeType>
class Node;

class MatrixSourceNode;
class ImageLoaderNode;
class CsvLoaderNode;
class UnaryOperatorNode;
class AugmentNode;
class TableNode;
class PlotNode;
class HeatMapNode;
class ROINode;
class SelectorNode;
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to apply the style of a pin before drawing it
//-------------------------------------------------------------------
template<typename DataType>
inline void apply_pin_style(bool is_pin_connected)
{
    if(is_pin_connected)
        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(0, 255, 0, 255));
    else
        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(0, 180, 0, 255));
}



template<>
inline void apply_pin_style<MatrixType>(bool is_pin_connected)
{
    if(is_pin_connected)
        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(0, 255, 0, 255));
    else
        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(0, 180, 0, 255));
}



template<>
inline void apply_pin_style<VectorOfMatricesType>(bool is_pin_connected)
{
    if(is_pin_connected)
        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(0, 0, 255, 255));
    else
        ImNodes::PushColorStyle(ImNodesCol_Pin, IM_COL32(0, 0, 180, 255));
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Function used to pick the pin shape when drawing a pin
//-------------------------------------------------------------------
template<typename DataType>
inline int pick_pin_shape(bool is_pin_connected)
{
    if(is_pin_connected)
        return 1;
    else
        return 0;
}



template<>
inline int pick_pin_shape<MatrixType>(bool is_pin_connected)
{
    if(is_pin_connected)
        return 1;
    else
        return 0;
}



template<>
inline int pick_pin_shape<VectorOfMatricesType>(bool is_pin_connected)
{
    if(is_pin_connected)
        return 3;
    else
        return 2;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Node Types
//-------------------------------------------------------------------
template<typename NodeType>
inline std::string get_node_type_name()
{
    return "NODE";
}

template<>
inline std::string get_node_type_name<MatrixSourceNode>()
{
    return "MATRIX_SOURCE_NODE";
}

template<>
inline std::string get_node_type_name<ImageLoaderNode>()
{
    return "IMAGE_LOADER_NODE";
}
    

template<>
inline std::string get_node_type_name<CsvLoaderNode>()
{
    return "CSV_LOADER_NODE";
}

template<>
inline std::string get_node_type_name<UnaryOperatorNode>()
{
    return "UNARY_OPERATOR_NODE";
}

template<>
inline std::string get_node_type_name<AugmentNode>()
{
    return "AUGMENT_NODE";
}

template<>
inline std::string get_node_type_name<TableNode>()
{
    return "TABLE_NODE";
}

template<>
inline std::string get_node_type_name<PlotNode>()
{
    return "PLOT_NODE";
}

template<>
inline std::string get_node_type_name<HeatMapNode>()
{
    return "HEAT_MAP_NODE";
}

template<>
inline std::string get_node_type_name<ROINode>()
{
    return "ROI_NODE";
}

template<>
inline std::string get_node_type_name<SelectorNode>()
{
    return "SELECTOR_NODE";
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif  // INCLUDE_DATA_FLOW_CONSTANTS_AND_DEFAULTS_HPP_