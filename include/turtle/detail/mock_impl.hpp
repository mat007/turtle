// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_MOCK_IMPL_HPP_INCLUDED
#define MOCK_MOCK_IMPL_HPP_INCLUDED

#include "function.hpp"
#include "functor.hpp"
#include "parameter.hpp"
#include "signature.hpp"
#include "type_name.hpp"
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/stringize.hpp>

// Internal compatibility macro if function signature is passed via BOOST_IDENTITY_TYPE
// TODO: Remove support for doing that and move remove_pointer_t to MOCK_PROTECT_FUNCTION_SIG
#define MOCK_FUNCTION_TYPE(...) std::remove_pointer_t<__VA_ARGS__>

#define MOCK_HELPER(t) t##_mock(mock::detail::root, BOOST_PP_STRINGIZE(t))
#define MOCK_ANONYMOUS_HELPER(t) t##_mock(mock::detail::root, "?.")

#define MOCK_METHOD_HELPER(S, t)                                                                                  \
    mutable mock::detail::function<MOCK_FUNCTION_TYPE(S)> t##_mock_;                                              \
    mock::detail::function<MOCK_FUNCTION_TYPE(S)>& t##_mock(const mock::detail::context&,                         \
                                                            const boost::unit_test::const_string& instance) const \
    {                                                                                                             \
        mock::detail::configure(*this,                                                                            \
                                t##_mock_,                                                                        \
                                instance.substr(0, instance.rfind(BOOST_PP_STRINGIZE(t))),                        \
                                mock::detail::make_type_name(*this),                                              \
                                BOOST_PP_STRINGIZE(t));                                                           \
        return t##_mock_;                                                                                         \
    }

#define MOCK_PARAM(S) mock::detail::parameter_t < MOCK_FUNCTION_TYPE(S)
#define MOCK_DECL_PARAM(z, n, d) BOOST_PP_COMMA_IF(n) d, n > p##n
#define MOCK_DECL_PARAMS(n, S) BOOST_PP_REPEAT(n, MOCK_DECL_PARAM, MOCK_PARAM(S))
#define MOCK_DECL(M, n, S, c) mock::detail::result_type_t<MOCK_FUNCTION_TYPE(S)> M(MOCK_DECL_PARAMS(n, S)) c

#define MOCK_FORWARD_PARAM(z, n, d) BOOST_PP_COMMA_IF(n) d, n >> (p##n)
#define MOCK_FORWARD_PARAMS(n, S) BOOST_PP_REPEAT(n, MOCK_FORWARD_PARAM, std::forward < MOCK_PARAM(S))
#define MOCK_METHOD_AUX(M, n, S, t, c)                                                                    \
    MOCK_DECL(M, n, S, c)                                                                                 \
    {                                                                                                     \
        static_assert(n == mock::detail::function_arity<MOCK_FUNCTION_TYPE(S)>::value, "Arity mismatch"); \
        return MOCK_ANONYMOUS_HELPER(t)(MOCK_FORWARD_PARAMS(n, S));                                       \
    }

#define MOCK_METHOD_EXT(M, n, S, t)    \
    MOCK_METHOD_AUX(M, n, S, t, )      \
    MOCK_METHOD_AUX(M, n, S, t, const) \
    MOCK_METHOD_HELPER(S, t)
#define MOCK_CONST_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_AUX(M, n, S, t, const)    \
    MOCK_METHOD_HELPER(S, t)
#define MOCK_NON_CONST_METHOD_EXT(M, n, S, t) \
    MOCK_METHOD_AUX(M, n, S, t, )             \
    MOCK_METHOD_HELPER(S, t)

#define MOCK_FUNCTION_HELPER(S, t, s)                                                                  \
    s mock::detail::function<MOCK_FUNCTION_TYPE(S)>& t##_mock(mock::detail::context& context,          \
                                                              boost::unit_test::const_string instance) \
    {                                                                                                  \
        static mock::detail::function<MOCK_FUNCTION_TYPE(S)> f;                                        \
        return f(context, instance);                                                                   \
    }

#define MOCK_CONSTRUCTOR_AUX(T, n, A, t)                                               \
    T(MOCK_DECL_PARAMS(n, void A)) { MOCK_HELPER(t)(MOCK_FORWARD_PARAMS(n, void A)); } \
    MOCK_FUNCTION_HELPER(void A, t, static)

#define MOCK_FUNCTION_AUX(F, n, S, t, s)                                                                  \
    MOCK_FUNCTION_HELPER(S, t, s)                                                                         \
    s MOCK_DECL(F, n, S, )                                                                                \
    {                                                                                                     \
        static_assert(n == mock::detail::function_arity<MOCK_FUNCTION_TYPE(S)>::value, "Arity mismatch"); \
        return MOCK_HELPER(t)(MOCK_FORWARD_PARAMS(n, S));                                                 \
    }

#define MOCK_VARIADIC_ELEM_0(e0, ...) e0
#define MOCK_VARIADIC_ELEM_1(e0, e1, ...) e1
#define MOCK_VARIADIC_ELEM_2(e0, e1, e2, ...) e2

#endif // MOCK_MOCK_IMPL_HPP_INCLUDED
