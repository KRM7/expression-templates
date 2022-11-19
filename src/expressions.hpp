#ifndef EXPR_EXPRESSIONS_HPP
#define EXPR_EXPRESSIONS_HPP

#include "utility.hpp"
#include <type_traits>
#include <functional>
#include <utility>
#include <cassert>

namespace matexpr
{
    template<typename T>
    class Matrix;

    template<typename Matrix, typename UnaryOp>
    class UnaryMatrixExpr;

    template<typename Left, typename Right, typename BinaryOp>
    class BinaryMatrixExpr;


    template<typename LeftExpr, typename RightExpr>
    constexpr size_t ncols(const LeftExpr& lhs, const RightExpr& rhs) noexcept;

    template<typename LeftExpr, typename RightExpr>
    constexpr size_t nrows(const LeftExpr& lhs, const RightExpr& rhs) noexcept;

    template<typename Expr>
    constexpr decltype(auto) get_element(Expr&& expr, size_t row, size_t col) noexcept;

    template<typename MatrixExpr, typename T = std::remove_cvref_t<decltype(std::declval<MatrixExpr>()(0, 0))>>
    inline Matrix<T> to_matrix(const MatrixExpr& expr);


    template<typename T, typename T_ = std::remove_cvref_t<T>>
    concept Scalar = std::is_arithmetic_v<T_>;

    template<typename T, typename T_ = std::remove_cvref_t<T>>
    concept MatrixExpr = is_specialization_of_v<T_, Matrix> || is_specialization_of_v<T_, UnaryMatrixExpr> || is_specialization_of_v<T_, BinaryMatrixExpr>;

} // namespace matexpr


namespace matexpr
{
    template<typename MatrixExpr, typename UnaryOp>
    class UnaryMatrixExpr
    {
    public:
        constexpr explicit UnaryMatrixExpr(MatrixExpr matrix, UnaryOp) noexcept : /* Last arg is for type deduction */
            matrix_(std::move(matrix))
        {}

        constexpr size_t ncols() const noexcept { return unwrap(matrix_).ncols(); }
        constexpr size_t nrows() const noexcept { return unwrap(matrix_).nrows(); }

        constexpr decltype(auto) operator()(size_t row, size_t col) const noexcept
        {
            static_assert(!std::is_same_v<std::identity, UnaryOp>);
            return std::invoke(op_, get_element(unwrap(matrix_), row, col));
        }

        /* implicit */ operator auto() const { return to_matrix(*this); } /* Conversion should be in matrix ctor/assignment if it can be. */

    private:
        MatrixExpr matrix_;
        inline static UnaryOp op_{}; /* Assume no state */
    };


    template<typename LeftExpr, typename RightExpr, typename BinaryOp>
    class BinaryMatrixExpr
    {
    public:
        constexpr BinaryMatrixExpr(LeftExpr lhs, RightExpr rhs, BinaryOp) noexcept : /* Last arg is for type deduction */
            lhs_(std::move(lhs)), rhs_(std::move(rhs))
        {}

        constexpr size_t ncols() const noexcept { return matexpr::ncols(unwrap(lhs_), unwrap(rhs_)); }
        constexpr size_t nrows() const noexcept { return matexpr::nrows(unwrap(lhs_), unwrap(rhs_)); }

        constexpr decltype(auto) operator()(size_t row, size_t col) const noexcept
        {
            return std::invoke(op_, get_element(unwrap(lhs_), row, col), get_element(unwrap(rhs_), row, col));
        }

        /* implicit */ operator auto() const { return to_matrix(*this); } /* Conversion should be in matrix ctor/assignment if it can be. */

    private:
        LeftExpr lhs_;
        RightExpr rhs_;
        inline static BinaryOp op_{}; /* Assume no state */
    };


    template<typename LeftExpr, typename RightExpr>
    constexpr size_t ncols(const LeftExpr& lhs, const RightExpr& rhs) noexcept
    {
        /* The expressions should already be unwrapped. */
        static_assert(!is_specialization_of_v<LeftExpr, std::reference_wrapper>);
        static_assert(!is_specialization_of_v<RightExpr, std::reference_wrapper>);

        /* Only one of the args will be scalar because of ADL. */
        static_assert(!std::is_arithmetic_v<LeftExpr> || !std::is_arithmetic_v<RightExpr>);

        if constexpr (std::is_arithmetic_v<LeftExpr>)
        {
            return rhs.ncols();
        }
        else if constexpr (std::is_arithmetic_v<RightExpr>)
        {
            return lhs.ncols();
        }
        else /* matexpr-matexpr */
        {
            assert(lhs.ncols() == rhs.ncols()); /* Assumes no mat-mat mul. */
            return lhs.ncols();
        }
    }

    template<typename LeftExpr, typename RightExpr>
    constexpr size_t nrows(const LeftExpr& lhs, const RightExpr& rhs) noexcept
    {
        /* The expressions should already be unwrapped. */
        static_assert(!is_specialization_of_v<LeftExpr, std::reference_wrapper>);
        static_assert(!is_specialization_of_v<RightExpr, std::reference_wrapper>);

        /* Only one of the args will be scalar because of ADL. */
        static_assert(!std::is_arithmetic_v<LeftExpr> || !std::is_arithmetic_v<RightExpr>);

        if constexpr (std::is_arithmetic_v<LeftExpr>)
        {
            return rhs.nrows();
        }
        else if constexpr (std::is_arithmetic_v<RightExpr>)
        {
            return lhs.nrows();
        }
        else /* matexpr-matexpr */
        {
            assert(lhs.nrows() == rhs.nrows()); /* Assumes no mat-mat mul. */
            return lhs.nrows();
        }
    }

    template<typename Expr>
    constexpr decltype(auto) get_element(Expr&& expr, size_t row, size_t col) noexcept
    {
        /* The expression should already be unwrapped. */
        static_assert(!is_specialization_of_v<std::remove_cvref_t<Expr>, std::reference_wrapper>);

        if constexpr (std::is_arithmetic_v<std::remove_cvref_t<Expr>>)
        {
            return Expr(expr);
        }
        else /* matrix expr */
        {
            return expr(row, col);
        }
    }

    template<typename MatrixExpr, typename T>
    inline Matrix<T> to_matrix(const MatrixExpr& expr)
    {
        Matrix<T> matrix(expr.nrows(), expr.ncols());

        for (size_t row = 0; row < matrix.nrows(); row++)
            for (size_t col = 0; col < matrix.ncols(); col++)
                matrix(row, col) = expr(row, col);

        return matrix;
    }

} // namespace matexpr

#endif // !EXPR_EXPRESSIONS_HPP