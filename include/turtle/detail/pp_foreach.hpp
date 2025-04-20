// http://turtle.sourceforge.net
//
// Copyright 2025 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_PP_FOREACH_HPP_INCLUDED
#define MOCK_PP_FOREACH_HPP_INCLUDED

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/facilities/overload.hpp>
#include <boost/preprocessor/tuple/rem.hpp>

/// Expand to `macro(data, elem)` for each element in the tuple.
/// Same as BOOST_PP_TUPLE_FOR_EACH but supports empty elements,
/// without causing a C4003 "not enough arguments for function-like macro invocation" warning on MSVC
#define MOCK_PP_TUPLE_FOR_EACH(macro, data, tuple) MOCK_PP_FOR_EACH(macro, data, BOOST_PP_REM tuple)

/// Expand to `macro(data, elem)` for each variadic element passed
#if BOOST_PP_VARIADICS_MSVC
#    define MOCK_PP_FOR_EACH(macro, data, ...) \
        BOOST_PP_CAT(BOOST_PP_OVERLOAD(MOCK_PP_INVOKE_, __VA_ARGS__)(macro, data, __VA_ARGS__), BOOST_PP_EMPTY())
#else
#    define MOCK_PP_FOR_EACH(macro, data, ...) BOOST_PP_OVERLOAD(MOCK_PP_INVOKE_, __VA_ARGS__)(macro, data, __VA_ARGS__)
#endif

#define MOCK_PP_INVOKE_1(macro, data, x) macro(data, x)
#define MOCK_PP_INVOKE_2(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_1(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_3(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_2(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_4(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_3(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_5(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_4(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_6(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_5(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_7(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_6(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_8(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_7(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_9(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_8(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_10(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_9(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_11(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_10(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_12(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_11(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_13(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_12(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_14(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_13(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_15(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_14(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_16(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_15(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_17(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_16(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_18(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_17(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_19(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_18(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_20(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_19(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_21(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_20(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_22(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_21(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_23(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_22(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_24(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_23(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_25(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_24(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_26(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_25(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_27(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_26(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_28(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_27(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_29(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_28(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_30(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_29(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_31(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_30(macro, data, __VA_ARGS__)
#define MOCK_PP_INVOKE_32(macro, data, x, ...) macro(data, x) MOCK_PP_INVOKE_31(macro, data, __VA_ARGS__)

#endif // MOCK_PP_FOREACH_HPP_INCLUDED
