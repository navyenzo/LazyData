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
template <typename T>
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
 * 2D matrix-like data structures. It provides im
ChatGPT
Certainly! Here's the revised code for the Data and Pin classes with Doxygen comments and variables and functions named using lowercase with underscores.

Modified Data Classes with Doxygen Comments
cpp
￼Copy code
/**
 * @file data.hpp
 * @brief Base and derived classes for handling various types of data.
 * @namespace DataFlow
 *
 * The file defines base and derived classes for managing different types of data,
 * such as 2D and 3D matrix-like structures, within a computational node graph system.
 */

namespace DataFlow {

/**
 * @class Data
 * @brief Base class for different data types.
 *
 * This class provides a common interface for various data plementations for 
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

    MatrixType matrix_;//-------------------------------------------------------------------
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
template <typename T>
class Data3D
{
public:

    Data3D() : id_(LazyApp::UniqueID::generate_uuid_string()) {}

    virtual ~Data3D() = default;

    std::string getId() const { return id_; }
    void set_id(const std::string& id) { id_ = id; }
    
    virtual uintptr_t pages()const = 0;
    virtual uintptr_t rows()const = 0;
    virtual uintptr_t columns()const  = 0;
    virtual uintptr_t size()const = 0;

    // Accessors for matrix elements.
    decltype(auto) at(int64_t page, int64_t row, int64_t column)const { return this->at_(page, row, column); }
    decltype(auto) at(int64_t page, int64_t row, int64_t column) { return this->at_(page, row, column); }
    decltype(auto) at(int64_t index) const { return (*this)(index); }
    decltype(auto) at(int64_t index) { return (*this)(index); }

    // Operator overloads for element access.
    decltype(auto) operator()(int64_t page, int64_t row, int64_t column)const { return this->at_(page, row, column); }
    decltype(auto) operator()(int64_t page, int64_t row, int64_t column) { return this->at_(page, row, column); }

    decltype(auto) operator()(int64_t index)const
    {
        int64_t page = index / (this->rows() * this->columns());

        int64_t remainder = index % (this->rows() * this->columns());

        return this->at_(page, remainder / this->columns(), remainder % this->columns());
    }

    decltype(auto) operator()(int64_t index)
    {
        int64_t page = index / (this->rows() * this->columns());

        int64_t remainder = index % (this->rows() * this->columns());

        return this->at_(page, remainder / this->columns(), remainder % this->columns());
    }

    // Circular accessors for matrix elements.
    decltype(auto) circ_at(int64_t page, int64_t row, int64_t column)const
    {
        int64_t circ_page = (this->pages() + page % this->pages) % this->pages();
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return this->at_(circ_page, circ_row, circ_column);
    }
    
    decltype(auto) circ_at(int64_t page, int64_t row, int64_t column)
    {
        int64_t circ_page = (this->pages() + page % this->pages) % this->pages();
        int64_t circ_row = (this->rows() + row % this->rows()) % this->rows();
        int64_t circ_column = (this->columns() + column % this->columns()) % this->columns();
        return this->at_(circ_page, circ_row, circ_column);
    }

    decltype(auto) circ_at(int64_t index)const
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return this->at_(circ_index);
    }

    decltype(auto) circ_at(int64_t index)
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return this->at_(circ_index);
    }



    // Implementation for element access, to be provided by derived classes.
    decltype(auto) at_(int64_t page, int64_t row, int64_t column)const
    {
        return underlying().at_(page, row, column);
    }

    decltype(auto) at_(int64_t page, int64_t row, int64_t column)
    {
        return underlying().at_(page, row, column);
    }



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
 * 2D matrix-like data structures. It provides im
ChatGPT
Certainly! Here's the revised code for the Data and Pin classes with Doxygen comments and variables and functions named using lowercase with underscores.

Modified Data Classes with Doxygen Comments
cpp
￼Copy code
/**
 * @file data.hpp
 * @brief Base and derived classes for handling various types of data.
 * @namespace DataFlow
 *
 * The file defines base and derived classes for managing different types of data,
 * such as 2D and 3D matrix-like structures, within a computational node graph system.
 */

namespace DataFlow {

/**
 * @class Data
 * @brief Base class for different data types.
 *
 * This class provides a common interface for various data plementations for 
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
    using DataType = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;
    return std::make_shared<SpecializedData<MatrixType, DataType>>(matrix);
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
    using DataType = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0,0))>::type;
    return std::make_shared<SpecializedData3D<MatrixType, DataType>>(matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif // INCLUDE_DATA_HPP_

//-------------------------------------------------------------------
template <typename MatrixType>
auto wrap_matrix(MatrixType& matrix)
{
    using DataType = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;
    return std::make_shared<SpecializedData<MatrixType, DataType>>(matrix);
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
    using DataType = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0,0))>::type;
    return std::make_shared<SpecializedData3D<MatrixType, DataType>>(matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif // INCLUDE_DATA_HPP_
4_t index)const
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return this->at_(circ_index);
    }

    decltype(auto) circ_at(int64_t index)
    {
        int64_t circ_index = (this->size() + index % this->size()) % this->size();
        return this->at_(circ_index);
    }



    // Implementation for element access, to be provided by derived classes.
    decltype(auto) at_(int64_t page, int64_t row, int64_t column)const
    {
        return underlying().at_(page, row, column);
    }

    decltype(auto) at_(int64_t page, int64_t row, int64_t column)
    {
        return underlying().at_(page, row, column);
    }



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
 * 2D matrix-like data structures. It provides im
ChatGPT
Certainly! Here's the revised code for the Data and Pin classes with Doxygen comments and variables and functions named using lowercase with underscores.

Modified Data Classes with Doxygen Comments
cpp
￼Copy code
/**
 * @file data.hpp
 * @brief Base and derived classes for handling various types of data.
 * @namespace DataFlow
 *
 * The file defines base and derived classes for managing different types of data,
 * such as 2D and 3D matrix-like structures, within a computational node graph system.
 */

namespace DataFlow {

/**
 * @class Data
 * @brief Base class for different data types.
 *
 * This class provides a common interface for various data plementations for 
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
    using DataType = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0))>::type;
    return std::make_shared<SpecializedData<MatrixType, DataType>>(matrix);
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
    using DataType = typename std::remove_reference<decltype(std::declval<MatrixType>()(0,0,0))>::type;
    return std::make_shared<SpecializedData3D<MatrixType, DataType>>(matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif // INCLUDE_DATA_HPP_
