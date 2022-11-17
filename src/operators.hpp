#ifndef EXPR_OPERATORS_HPP
#define EXPR_OPERATORS_HPP

#include "expressions.hpp"
#include "utility.hpp"
#include <functional>

namespace matexpr
{
    constexpr auto operator-(MatrixExpr auto&& expr) noexcept
    {
        return UnaryMatrixExpr{ lforward<decltype(expr)>(expr), std::negate{} };
    }

    constexpr auto operator+(MatrixExpr auto&& lhs, MatrixExpr auto&& rhs) noexcept
    {
        return BinaryMatrixExpr{ lforward<decltype(lhs)>(lhs), lforward<decltype(rhs)>(rhs), std::plus{} };
    }

    constexpr auto operator-(MatrixExpr auto&& lhs, MatrixExpr auto&& rhs) noexcept
    {
        return BinaryMatrixExpr{ lforward<decltype(lhs)>(lhs), lforward<decltype(rhs)>(rhs), std::minus{} };
    }

    constexpr auto operator*(MatrixExpr auto&& lhs, Scalar auto rhs) noexcept
    {
        return BinaryMatrixExpr{ lforward<decltype(lhs)>(lhs), rhs, std::multiplies{} };
    }

    constexpr auto operator*(Scalar auto lhs, MatrixExpr auto&& rhs) noexcept
    {
        return BinaryMatrixExpr{ lhs, lforward<decltype(rhs)>(rhs), std::multiplies{} };
    }

    constexpr auto operator/(MatrixExpr auto&& lhs, Scalar auto rhs) noexcept
    {
        return BinaryMatrixExpr{ lforward<decltype(lhs)>(lhs), rhs, std::divides{} };
    }

} // namespace matexpr

#endif // !EXPR_OPERATORS_HPP