// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_PARAMETER_HPP_INCLUDED
#define MOCK_PARAMETER_HPP_INCLUDED

#include "../config.hpp"

namespace mock { namespace detail {
    template<class...>
    struct tuple;

    template<std::size_t I, class T>
    struct tuple_element;

    template<std::size_t I, class H, class... T>
    struct tuple_element<I, tuple<H, T...>> : tuple_element<I - 1, tuple<T...>>
    {};

    template<class H, class... T>
    struct tuple_element<0, tuple<H, T...>>
    {
        using type = H;
    };

    template<typename Signature>
    struct result_type;

    template<typename R, typename... Args>
    struct result_type<R(Args...)>
    {
        using type = R;
    };
    template<typename Signature>
    using result_type_t = typename result_type<Signature>::type;

    template<typename Signature>
    struct function_arity;

    template<typename R, typename... Args>
    struct function_arity<R(Args...)>
    {
        static constexpr size_t value = sizeof...(Args);
    };

    template<typename Signature>
    struct parameter_types;

    template<typename R, typename... Args>
    struct parameter_types<R(Args...)>
    {
        using type = tuple<Args...>;
    };

    template<typename Signature, std::size_t n>
    struct parameter
    {
        static_assert(n < function_arity<Signature>::value, "Function signature has not that many parameters");
        using type = typename tuple_element<n, typename parameter_types<Signature>::type>::type;
    };
    template<typename T, std::size_t n>
    using parameter_t = typename parameter<T, n>::type;

    template<typename T>
    struct parameter_type;
    template<typename T, typename U>
    struct parameter_type<T(U)>
    {
        using type = U;
    };
    template<typename T>
    using parameter_type_t = typename parameter_type<T>::type;
}} // namespace mock::detail

#endif // MOCK_PARAMETER_HPP_INCLUDED
