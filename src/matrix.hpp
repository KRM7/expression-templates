#ifndef EXPR_MATRIX_HPP
#define EXPR_MATRIX_HPP

#include <vector>
#include <algorithm>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <stdexcept>
#include <cassert>
#include <cstddef>

namespace matexpr
{
    template<typename T>
    class Matrix
    {
    public:
        using value_type = T;
        using allocator_type  = typename std::vector<T>::allocator_type;

        using size_type       = typename std::vector<T>::size_type;
        using difference_type = typename std::vector<T>::difference_type;

        using reference       = typename std::vector<T>::reference;
        using pointer         = typename std::vector<T>::pointer;
        using const_reference = typename std::vector<T>::const_reference;
        using const_pointer   = typename std::vector<T>::const_pointer;

        Matrix()                            = default;
        Matrix(const Matrix&)               = default;
        Matrix(Matrix&&)                    = default;
        Matrix& operator=(const Matrix&)    = default;
        Matrix& operator=(Matrix&&)         = default;
        ~Matrix()                           = default;

        Matrix(size_type nrows, size_type ncols, const T& init = T()) :
            data_(nrows * ncols, init), nrows_(nrows), ncols_(ncols)
        {}

        Matrix(std::initializer_list<std::initializer_list<T>> mat) :
            data_(), nrows_(mat.size()), ncols_(0)
        {
            if (mat.size() == 0) return;

            ncols_ = mat.begin()->size();

            data_.reserve(nrows_ * ncols_);
            for (auto& row : mat)
            {
                assert(row.size() == ncols_);
                for (auto& entry : row)
                {
                    data_.push_back(std::move(entry));
                }
            }
        }

        Matrix(const std::vector<T>& vec) :
            data_(vec), nrows_(1), ncols(vec.size())
        {}

        reference operator()(size_type row, size_type col) noexcept
        {
            assert(row < nrows_);
            assert(col < ncols_);

            return data_[row * ncols_ + col];
        }

        const_reference operator()(size_type row, size_type col) const noexcept
        {
            assert(row < nrows_);
            assert(col < ncols_);

            return data_[row * ncols_ + col];
        }

        pointer data() noexcept             { return data_.data(); }
        const_pointer data() const noexcept { return data_.data(); }

        size_type nrows() const noexcept { return nrows_; }
        size_type ncols() const noexcept { return ncols_; }
        size_type empty() const noexcept { return data_.empty(); }

        friend bool operator==(const Matrix& lhs, const Matrix& rhs)
        {
            return (lhs.empty() && rhs.empty()) ||
                   (lhs.nrows_ == rhs.nrows_ &&
                    lhs.ncols_ == rhs.ncols_ &&
                    lhs.data_ == rhs.data_);
        }

        friend bool operator!=(const Matrix& lhs, const Matrix& rhs)
        {
            return !(lhs == rhs);
        }

        void swap(Matrix& other) noexcept(std::is_nothrow_swappable_v<T>)
        {
            std::swap(data_, other.data_);
            std::swap(nrows_, other.nrows_);
            std::swap(ncols_, other.ncols_);
        }

    private:

        std::vector<T> data_;
        size_type nrows_ = 0;
        size_type ncols_ = 0;
    };

} // namespace matexpr

template<typename T>
inline void swap(matexpr::Matrix<T>& lhs, matexpr::Matrix<T>& rhs) noexcept(std::is_nothrow_swappable_v<T>)
{
    lhs.swap(rhs);
}

#endif // !EXPR_MATRIX_HPP