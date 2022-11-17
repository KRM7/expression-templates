#ifndef EXPR_UTILITY_HPP
#define EXPR_UTILITY_HPP

#include <type_traits>
#include <functional>

namespace matexpr
{
    template<typename Spec, template<typename...> class Templ>
    struct is_specialization_of : std::false_type {};
    
    template<template<typename...> class Templ, typename... TArgs>
    struct is_specialization_of<Templ<TArgs...>, Templ> : std::true_type {};
    
    template<typename Spec, template<typename...> class Templ>
    inline constexpr bool is_specialization_of_v = is_specialization_of<Spec, Templ>::value;


    template<typename T>
    requires(std::is_lvalue_reference_v<T>)
    constexpr auto lforward(std::remove_reference_t<T>& t) noexcept
    {
        return std::ref<std::remove_reference_t<T>>(t);
    }

    template<typename T>
    requires(!std::is_lvalue_reference_v<T>)
    constexpr T&& lforward(std::remove_reference_t<T>& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template<typename T>
    requires(!std::is_lvalue_reference_v<T>)
    constexpr T&& lforward(std::remove_reference_t<T>&& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template<typename T>
    constexpr auto& unwrap(const T& value) noexcept
    {
        if constexpr (is_specialization_of_v<T, std::reference_wrapper>)
        {
            return value.get();
        }
        else
        {
            return value;
        }
    }

} // namespace matexpr

#endif // !EXPR_UTILITY_HPP