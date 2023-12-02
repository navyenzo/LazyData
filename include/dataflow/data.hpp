#ifndef INCLUDE_DATA_HPP_
#define INCLUDE_DATA_HPP_



//-------------------------------------------------------------------
#include <string>
#include <stdexcept>
#include <memory>

#include <utils/unique_id.hpp>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/** 
 * @file data.hpp
 * @brief Defines Data, Data3D, SpecializedData, and SpecializedData3D classes for handling matrix-like data.
 * @namespace DataFlow
 *
 * This file includes templated classes for handling generic data types in 2D and 3D matrix-like structures.
 * These classes are designed to be used in computational node graphs, allowing for flexible data management.
 */
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataFlow
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
template <typename T>
class Data
{
public:

    Data() : id_(UniqueID::generate_uuid_string()) {}

    virtual ~Data() = default;

    std::string getId() const { return id_; }

    virtual size_t rows() const = 0;
    virtual size_t columns() const = 0;
    virtual T at(size_t i, size_t j) const = 0;
    virtual T& operator()(size_t i, size_t j) = 0;



private:

    std::string id_;
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
template <typename T>
class Data3D
{
public:

    Data3D() : id_(UniqueID::generate_uuid_string()) {}

    virtual ~Data3D() = default;

    virtual size_t pages() const = 0;
    virtual size_t rows() const = 0;
    virtual size_t columns() const = 0;
    virtual T at(size_t i, size_t j, size_t k) const = 0;
    virtual T& operator()(size_t i, size_t j, size_t k) = 0;



private:

    std::string id_;
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
template <typename MatrixType, typename T>
class SpecializedData : public Data<T>
{
public:

    SpecializedData(const MatrixType& matrix) : matrix_(matrix) {}

    size_t rows() const override { return matrix_.rows(); }
    size_t columns() const override { return matrix_.columns(); }
    T at(size_t i, size_t j) const override { return matrix_.at(i, j); }
    T& operator()(size_t i, size_t j) override { return matrix_(i, j); }



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
template <typename MatrixType, typename T>
class SpecializedData3D : public Data3D<T>
{
public:

    SpecializedData3D(const MatrixType& matrix) : matrix_(matrix) {}

    size_t pages() const override { return matrix_.pages(); }
    size_t rows() const override { return matrix_.rows(); }
    size_t columns() const override { return matrix_.columns(); }
    T at(size_t i, size_t j, size_t k) const override { return matrix_.at(i, j, k); }
    T& operator()(size_t i, size_t j, size_t k) override { return matrix_(i, j, k); }

private:

    MatrixType matrix_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Factory function to create a SpecializedData object.
 * 
 * @tparam MatrixType Type of the 2D matrix.
 * @param matrix The 2D matrix to encapsulate.
 * @return std::shared_ptr<SpecializedData<MatrixType, T>> Shared pointer to the created object.
 */
//-------------------------------------------------------------------
template <typename MatrixType>
auto create_data(const MatrixType& matrix)
{
    using DataType = decltype(matrix.at(0, 0));
    return std::make_shared<SpecializedData<MatrixType, DataType>>(matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Factory function to create a SpecializedData3D object.
 * 
 * @tparam MatrixType Type of the 3D matrix.
 * @param matrix The 3D matrix to encapsulate.
 * @return std::shared_ptr<SpecializedData3D<MatrixType, T>> Shared pointer to the created object.
 */
//-------------------------------------------------------------------
template <typename MatrixType>
auto create_data3d(const MatrixType& matrix)
{
    using DataType = decltype(matrix.at(0, 0, 0));
    return std::make_shared<SpecializedData3D<MatrixType, DataType>>(matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif // INCLUDE_DATA_HPP_
