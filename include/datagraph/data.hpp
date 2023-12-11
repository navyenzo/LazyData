//-------------------------------------------------------------------
/**
 * @file data.hpp
 * @brief Base and derived classes for handling various types of data that offer 2d-matrix-like or 3d-matrix-like interface
 * @namespace DataFlow
 *
 * The file defines base and derived classes for managing different types of data,
 * such as 2D and 3D matrix-like structures, within a computational node graph system.
 */
//-------------------------------------------------------------------



#ifndef INCLUDE_DATA_HPP_
#define INCLUDE_DATA_HPP_



//-------------------------------------------------------------------
#include <string>
#include <stdexcept>
#include <memory>

#include <utils/unique_id.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataGraph
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/** 
 * @class Data
 * @brief Base template class for handling 2D matrix-like data.
 *
 * This class provides an interface for 2D matrix-like data, 
 * supporting various data types. It includes a unique identifier 
 * and virtual functions for common matrix operations.
 */
//-------------------------------------------------------------------
template <typename DataType>
class Data
{
public:

    Data() {}

    virtual ~Data() = default;

    std::string getId() const { return id_; }
    void set_id(const std::string& id) { id_ = id; }

    virtual uintptr_t rows() const = 0;
    virtual uintptr_t columns() const = 0;
    virtual uintptr_t size() const = 0;

    virtual const DataType& at(int64_t row, int64_t column) const = 0;
    virtual DataType& at(int64_t row, int64_t column) = 0;
    virtual const DataType& at(int64_t index) const = 0;
    virtual DataType& at(int64_t index) = 0;

    virtual const DataType& operator()(int64_t row, int64_t column) const = 0;
    virtual DataType& operator()(int64_t row, int64_t column) = 0;
    virtual const DataType& operator()(int64_t index) const = 0;
    virtual DataType& operator()(int64_t index) = 0;

    virtual const DataType& circ_at(int64_t row, int64_t column)const = 0;
    virtual DataType& circ_at(int64_t row, int64_t column) = 0;
    virtual const DataType& circ_at(int64_t index)const = 0;
    virtual DataType& circ_at(int64_t index) = 0;



private:

    std::string id_ = LazyApp::UniqueID::generate_uuid_string();
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/** 
 * @class Data3D
 * @brief Base template class for handling 3D matrix-like data.
 *
 * This class extends Data to provide an interface for 3D matrix-like data.
 * It includes additional functionality to handle the extra dimension in 3D data.
 */
//-------------------------------------------------------------------
template <typename DataType>
class Data3D
{
public:

    Data3D() {}

    virtual ~Data3D() = default;

    std::string getId() const { return id_; }
    void set_id(const std::string& id) { id_ = id; }
    
    virtual uintptr_t pages()const = 0;
    virtual uintptr_t rows()const = 0;
    virtual uintptr_t columns()const  = 0;
    virtual uintptr_t size()const = 0;
    
    virtual const DataType& at(int64_t page, int64_t row, int64_t column) const = 0;
    virtual DataType& at(int64_t page, int64_t row, int64_t column) = 0;
    virtual const DataType& at(int64_t index) const = 0;
    virtual DataType& at(int64_t index) = 0;

    virtual const DataType& operator()(int64_t page, int64_t row, int64_t column) const = 0;
    virtual DataType& operator()(int64_t page, int64_t row, int64_t column) = 0;
    virtual const DataType& operator()(int64_t index) const = 0;
    virtual DataType& operator()(int64_t index) = 0;

    virtual const DataType& circ_at(int64_t page, int64_t row, int64_t column)const = 0;
    virtual DataType& circ_at(int64_t page, int64_t row, int64_t column) = 0;
    virtual const DataType& circ_at(int64_t index)const = 0;
    virtual DataType& circ_at(int64_t index) = 0;



private:

    std::string id_ = LazyApp::UniqueID::generate_uuid_string();
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/** 
 * @class SpecializedData
 * @brief Template class for specialized 2D matrix-like data handling.
 *
 * This class extends the Data class to handle specific types of 
 * 2D matrix-like data structures. It provides implementations for 
 * the virtual functions defined in Data.
 */
//-------------------------------------------------------------------
template <typename MatrixType>
class SpecializedData : public Data<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>
{
public:

    using DataType = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    SpecializedData(const MatrixType& matrix) : matrix_(matrix) {}

    uintptr_t rows() const override { return matrix_.rows(); }
    uintptr_t columns() const override { return matrix_.columns(); }
    uintptr_t size() const override { return matrix_.size(); }

    const DataType& at(int64_t row, int64_t column) const override { return matrix_.at(row, column); }
    DataType& at(int64_t row, int64_t column) override { return matrix_.at(row, column); }
    const DataType& at(int64_t index) const override { return matrix_.at(index); }
    DataType& at(int64_t index) override { return matrix_.at(index); }

    const DataType& operator()(int64_t row, int64_t column) const override { return matrix_(row, column); }
    DataType& operator()(int64_t row, int64_t column) override { return matrix_(row, column); }
    const DataType& operator()(int64_t index) const override { return matrix_(index); }
    DataType& operator()(int64_t index) override { return matrix_(index); }

    const DataType& circ_at(int64_t row, int64_t column)const override { return matrix_.circ_at(row, column); }
    DataType& circ_at(int64_t row, int64_t column) override { return matrix_.circ_at(row, column); }
    const DataType& circ_at(int64_t index)const override { return matrix_.circ_at(index); }
    DataType& circ_at(int64_t index) override { return matrix_.circ_at(index); }



private:

    MatrixType matrix_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/** 
 * @class SpecializedData3D
 * @brief Template class for specialized 3D matrix-like data handling.
 *
 * This class extends the Data3D class to handle specific types of 
 * 3D matrix-like data structures. It provides implementations for 
 * the virtual functions defined in Data3D.
 */
//-------------------------------------------------------------------
template <typename MatrixType>
class SpecializedData3D : public Data3D<typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type>
{
public:

    using DataType = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;

    SpecializedData3D(const MatrixType& matrix) : matrix_(matrix) {}

    uintptr_t pages() const override { return matrix_.pages(); }
    uintptr_t rows() const override { return matrix_.rows(); }
    uintptr_t columns() const override { return matrix_.columns(); }
    uintptr_t size() const override { return matrix_.size(); }

    const DataType& at(int64_t page, int64_t row, int64_t column) const override { return matrix_.at(page, row, column); }
    DataType& at(int64_t page, int64_t row, int64_t column) override { return matrix_.at(page, row, column); }
    const DataType& at(int64_t index) const override { return matrix_.at(index); }
    DataType& at(int64_t index) override { return matrix_.at(index); }

    const DataType& operator()(int64_t page, int64_t row, int64_t column) const override { return matrix_(page, row, column); }
    DataType& operator()(int64_t page, int64_t row, int64_t column) override { return matrix_(page, row, column); }
    const DataType& operator()(int64_t index) const override { return matrix_(index); }
    DataType& operator()(int64_t index) override { return matrix_(index); }

    const DataType& circ_at(int64_t page, int64_t row, int64_t column)const override { return matrix_.circ_at(page, row, column); }
    DataType& circ_at(int64_t page, int64_t row, int64_t column) override { return matrix_.circ_at(page, row, column); }
    const DataType& circ_at(int64_t index)const override { return matrix_.circ_at(index); }
    DataType& circ_at(int64_t index) override { return matrix_.circ_at(index); }



private:

    MatrixType matrix_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Factory function to wrap a matrix object with Data
 * 
 * @tparam MatrixType Type of the 2D matrix.
 * @param matrix The 2D matrix to encapsulate.
 * @return std::shared_ptr<SpecializedData<MatrixType, T>> Shared pointer to the created object.
 */
//-------------------------------------------------------------------
template <typename MatrixType>
auto wrap_matrix(MatrixType& matrix)
{
    return std::make_shared<SpecializedData<MatrixType>>(matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Factory function to wrap a 3d matrix object with Data3D
 * 
 * @tparam MatrixType Type of the 3D matrix.
 * @param matrix The 3D matrix to encapsulate.
 * @return std::shared_ptr<SpecializedData3D<MatrixType, T>> Shared pointer to the created object.
 */
//-------------------------------------------------------------------
template <typename MatrixType>
auto wrap_matrix3d(MatrixType& matrix)
{
    return std::make_shared<SpecializedData3D<MatrixType>>(matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataGraph
//-------------------------------------------------------------------



#endif // INCLUDE_DATA_HPP_