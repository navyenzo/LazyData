#ifndef INCLUDE_DATA_HPP_
#define INCLUDE_DATA_HPP_



//-------------------------------------------------------------------
#include <memory>
//-------------------------------------------------------------------



//-------------------------------------------------------------------
namespace DataFlow
{
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Abstract base class for data types.
 *
 * The Data class serves as a base for all data types, providing
 * a common interface for accessing elements and size information.
 */
//-------------------------------------------------------------------
template<typename ReturnType>
class Data
{
public:

    virtual ~Data() = default;

    /**
     * @brief Get the element at the specified row and column.
     * 
     * @param row The row index.
     * @param column The column index.
     * @return The value at the specified location.
     */
    virtual ReturnType at(int row, int column) const = 0;

    /**
     * @brief Get the element at the specified row and column.
     * 
     * @param row The row index.
     * @param column The column index.
     * @return The value at the specified location.
     */
    ReturnType operator()(int row, int column)
    {
        return this->at(row, column);
    }

    /**
     * @brief Get the number of rows.
     * 
     * @return Number of rows.
     */
    virtual int rows() const = 0;

    /**
     * @brief Get the number of columns.
     * 
     * @return Number of columns.
     */
    virtual int columns() const = 0;



    /**
     * @brief Get the size of the matrix as if it were a 1d array
     * 
     * @return size of the matrix as a 1d array
     */
    int side() const { return this->rows() * this->columns(); }
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Concrete data class for matrix data.
 *
 * MatrixData encapsulates matrix data and provides access
 * to elements, rows, and columns.
 *
 * @tparam MatrixType Type of the underlying matrix.
 * @tparam ReturnType The return type of the at function.
 */
//-------------------------------------------------------------------
template <typename MatrixType, typename ReturnType>
class MatrixData : public Data<ReturnType>
{
public:

    /**
     * @brief Construct a new MatrixData object.
     * 
     * @param matrix The matrix to encapsulate.
     */
    explicit MatrixData(const MatrixType& matrix) 
        : matrix_(std::make_shared<MatrixType>(matrix)) 
    { 
    }

    virtual ReturnType at(int row, int column) const override
    {
        return static_cast<ReturnType>((*matrix_)(row, column));
    }

    virtual int rows() const override
    {
        return matrix_->rows();
    }

    virtual int columns() const override
    {
        return matrix_->columns();
    }

private:

    std::shared_ptr<MatrixType> matrix_;
};
//-------------------------------------------------------------------



//-------------------------------------------------------------------
/**
 * @brief Factory function to create a MatrixData object with automatic return type deduction.
 * 
 * @tparam MatrixType Type of the matrix.
 * @param matrix The matrix to encapsulate.
 * @return std::shared_ptr<Data<ReturnType>> Shared pointer to the created MatrixData object.
 */
//-------------------------------------------------------------------
template <typename MatrixType>

auto create_data(const MatrixType& matrix)
{
    using ReturnType = decltype(matrix.at(0, 0));
    return std::make_shared<MatrixData<MatrixType, ReturnType>>(matrix);
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
} // namespace DataFlow
//-------------------------------------------------------------------



#endif // INCLUDE_DATA_HPP_