// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
// Copyright 2022-2025 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_MOCK_IMPL_HPP_INCLUDED
#define MOCK_MOCK_IMPL_HPP_INCLUDED

#include "function.hpp"
#include "functor.hpp"
#include "pp_foreach.hpp"
#include "signature.hpp"
#include "signature_traits.hpp"
#include "type_name.hpp"
#include <boost/preprocessor/comparison/less_equal.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <type_traits>

namespace mock { namespace detail {
    /// Simplified trait to extract the argument type of a function signature with 1 argument
    template<typename T>
    struct arg_type;
    template<typename T, typename U>
    struct arg_type<T(U)>
    {
        using type = U;
    };
    /// Used in MOCK_PROTECT_SIGNATURE to unwrap the passed function signature
    /// T is something like `void(std::map<int, float>)`
    template<typename T>
    using unwrap_signature_t = std::remove_pointer_t<typename arg_type<T>::type>;
}} // namespace mock::detail

#define MOCK_HELPER(t) t##_mock(mock::detail::root, BOOST_PP_STRINGIZE(t))
#define MOCK_ANONYMOUS_HELPER(t) t##_mock(mock::detail::root, "?.")

#define MOCK_METHOD_HELPER(S, t)                                                                                      \
    mutable mock::detail::function<S> t##_mock_;                                                                      \
    mock::detail::function<S>& t##_mock(const mock::detail::context&, const boost::unit_test::const_string& instance) \
      const                                                                                                           \
    {                                                                                                                 \
        mock::detail::configure(*this,                                                                                \
                                t##_mock_,                                                                            \
                                instance.substr(0, instance.rfind(BOOST_PP_STRINGIZE(t))),                            \
                                mock::detail::make_type_name(*this),                                                  \
                                BOOST_PP_STRINGIZE(t));                                                               \
        return t##_mock_;                                                                                             \
    }

#define MOCK_PARAM(S, n) mock::detail::parameter_t<S, n>

#define MOCK_DECL_PARAM(z, n, S) MOCK_PARAM(S, n) p##n
#define MOCK_DECL_PARAMS(n, S) BOOST_PP_ENUM(n, MOCK_DECL_PARAM, S)
#define MOCK_DECL(name, arity, signature, qualifier) \
    mock::detail::result_type_t<signature> name(MOCK_DECL_PARAMS(arity, signature)) qualifier

#define MOCK_FORWARD_PARAM(z, n, S) std::forward<MOCK_PARAM(S, n)>(p##n)
#define MOCK_FORWARD_PARAMS(n, S) BOOST_PP_ENUM(n, MOCK_FORWARD_PARAM, S)
#define MOCK_METHOD_AUX(name, arity, signature, identifier, qualifier)                   \
    MOCK_DECL(name, arity, signature, qualifier)                                         \
    {                                                                                    \
        return MOCK_ANONYMOUS_HELPER(identifier)(MOCK_FORWARD_PARAMS(arity, signature)); \
    }

/// MOCK_METHOD_ITER((name, arity, signature, identifier), qualifier)
#define MOCK_METHOD_ITER(M_n_S_t, qualifier)         \
    MOCK_METHOD_AUX(BOOST_PP_TUPLE_ELEM(0, M_n_S_t), \
                    BOOST_PP_TUPLE_ELEM(1, M_n_S_t), \
                    BOOST_PP_TUPLE_ELEM(2, M_n_S_t), \
                    BOOST_PP_TUPLE_ELEM(3, M_n_S_t), \
                    qualifier)

/// MOCK_METHOD_EXT_IMPL( name, arity, signature, identifier, qualifiers... )
#define MOCK_METHOD_EXT_IMPL(name, arity, signature, identifier, ...)                           \
    static_assert(arity == mock::detail::function_arity_t<signature>::value, "Arity mismatch"); \
    MOCK_PP_FOR_EACH(MOCK_METHOD_ITER, (name, arity, signature, identifier), __VA_ARGS__)       \
    MOCK_METHOD_HELPER(signature, identifier)

/// MOCK_METHOD_EXT( name, arity, signature, identifier [ , qualifiers...] )
/// If qualifiers is not given, defaults to (const, ), i.e. const and non-const
#define MOCK_METHOD_EXT(...) \
    MOCK_METHOD_EXT_IMPL     \
    BOOST_PP_IF(BOOST_PP_LESS_EQUAL(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 4), (__VA_ARGS__, const, ), (__VA_ARGS__))

#define MOCK_CONST_METHOD_EXT(M, n, S, t) MOCK_METHOD_EXT(M, n, S, t, const)
#define MOCK_NON_CONST_METHOD_EXT(M, n, S, t) MOCK_METHOD_EXT(M, n, S, t, )

#define MOCK_FUNCTION_HELPER(signature, identifier, prefix)                                              \
    prefix mock::detail::function<signature>& identifier##_mock(mock::detail::context& context,          \
                                                                boost::unit_test::const_string instance) \
    {                                                                                                    \
        static mock::detail::function<signature> f;                                                      \
        return f(context, instance);                                                                     \
    }

#define MOCK_FUNCTION_AUX(name, arity, signature, identifier, prefix)                           \
    MOCK_FUNCTION_HELPER(signature, identifier, prefix)                                         \
    static_assert(arity == mock::detail::function_arity_t<signature>::value, "Arity mismatch"); \
    prefix MOCK_DECL(name, arity, signature, )                                                  \
    {                                                                                           \
        return MOCK_HELPER(identifier)(MOCK_FORWARD_PARAMS(arity, signature));                  \
    }

#define MOCK_REPLACED_MACRO_ERROR(oldName, newName) static_assert(false, #oldName " has been replaced by " #newName)
// Replaced macros
#define MOCK_CONST_CONVERSION_OPERATOR_TPL(...) \
    MOCK_REPLACED_MACRO_ERROR(MOCK_CONST_CONVERSION_OPERATOR_TPL, MOCK_CONST_CONVERSION_OPERATOR)
#define MOCK_CONST_METHOD_EXT_TPL(...) MOCK_REPLACED_MACRO_ERROR(MOCK_CONST_METHOD_EXT_TPL, MOCK_CONST_METHOD)
#define MOCK_CONST_METHOD_TPL(...) MOCK_REPLACED_MACRO_ERROR(MOCK_CONST_METHOD_TPL, MOCK_CONST_METHOD)
#define MOCK_CONVERSION_OPERATOR_TPL(...) \
    MOCK_REPLACED_MACRO_ERROR(MOCK_CONVERSION_OPERATOR_TPL, MOCK_CONVERSION_OPERATOR)
#define MOCK_FUNCTOR_TPL(...) MOCK_REPLACED_MACRO_ERROR(MOCK_FUNCTOR_TPL, MOCK_FUNCTOR)
#define MOCK_METHOD_EXT_TPL(...) MOCK_REPLACED_MACRO_ERROR(MOCK_METHOD_EXT_TPL, MOCK_METHOD)
#define MOCK_METHOD_TPL(...) MOCK_REPLACED_MACRO_ERROR(MOCK_METHOD_TPL, MOCK_METHOD)
#define MOCK_NON_CONST_CONVERSION_OPERATOR_TPL(...) \
    MOCK_REPLACED_MACRO_ERROR(MOCK_NON_CONST_CONVERSION_OPERATOR_TPL, MOCK_NON_CONST_CONVERSION_OPERATOR)
#define MOCK_NON_CONST_METHOD_EXT_TPL(...) \
    MOCK_REPLACED_MACRO_ERROR(MOCK_NON_CONST_METHOD_EXT_TPL, MOCK_NON_CONST_METHOD)
#define MOCK_NON_CONST_METHOD_TPL(...) MOCK_REPLACED_MACRO_ERROR(MOCK_NON_CONST_METHOD_TPL, MOCK_NON_CONST_METHOD)
#define MOCK_STATIC_METHOD_TPL(...) MOCK_REPLACED_MACRO_ERROR(MOCK_STATIC_METHOD_TPL, MOCK_STATIC_METHOD)
#define MOCK_CONSTRUCTOR_TPL(...) MOCK_REPLACED_MACRO_ERROR(MOCK_CONSTRUCTOR_TPL, MOCK_CONSTRUCTOR)

#endif // MOCK_MOCK_IMPL_HPP_INCLUDED
