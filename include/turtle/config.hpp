// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
// Copyright 2020-2026 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// boost-no-inspect

#ifndef MOCK_CONFIG_HPP_INCLUDED
#define MOCK_CONFIG_HPP_INCLUDED

#include <boost/config.hpp>

#ifndef MOCK_ERROR_POLICY
#    define MOCK_ERROR_POLICY mock::error
#    define MOCK_USE_BOOST_TEST
#endif

#if !defined(BOOST_NO_CXX11_HDR_MUTEX) && !defined(BOOST_NO_0X_HDR_MUTEX)
#    ifndef MOCK_NO_HDR_MUTEX
#        define MOCK_HDR_MUTEX
#    endif
#endif

#ifndef MOCK_NO_UNCAUGHT_EXCEPTIONS
#    if(defined(__cpp_lib_uncaught_exceptions) && __cpp_lib_uncaught_exceptions >= 201411) || \
      (defined(_MSC_VER) && _MSC_VER >= 1900)
#        define MOCK_UNCAUGHT_EXCEPTIONS
#    endif
#endif

#if defined(__clang__) && defined(__has_warning)
#    define MOCK_DIAGNOSTIC_PUSH _Pragma("clang diagnostic push")
#    define MOCK_DIAGNOSTIC_POP _Pragma("clang diagnostic pop")
#    if __has_warning("-Wsuggest-override")
#        define MOCK_DIAGNOSTIC_IGNORE_SUGGEST_OVERRIDE _Pragma("clang diagnostic ignored \"-Wsuggest-override\"")
#    endif
#elif defined(__GNUC__)
#    define MOCK_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
#    define MOCK_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
#    if(__GNUC__ >= 9)
#        define MOCK_DIAGNOSTIC_IGNORE_SUGGEST_OVERRIDE _Pragma("GCC diagnostic ignored \"-Wsuggest-override\"")
#    endif
#endif
#ifndef MOCK_DIAGNOSTIC_PUSH
#    define MOCK_DIAGNOSTIC_PUSH
#    define MOCK_DIAGNOSTIC_POP
#endif
#ifndef MOCK_DIAGNOSTIC_IGNORE_SUGGEST_OVERRIDE
#    define MOCK_DIAGNOSTIC_IGNORE_SUGGEST_OVERRIDE
#endif

#if BOOST_VERSION >= 107700
#    define MOCK_CXX_VERSION BOOST_CXX_VERSION
#elif defined(_MSC_VER)
#    ifdef _MSVC_LANG
#        define MOCK_CXX_VERSION _MSVC_LANG
#    elif defined(_HAS_CXX17)
#        define MOCK_CXX_VERSION 201703L
#    endif
#endif
#ifndef MOCK_CXX_VERSION
#    define MOCK_CXX_VERSION __cplusplus
#endif

static_assert(MOCK_CXX_VERSION >= 201402L, "Turtle requires C++14");

#endif // MOCK_CONFIG_HPP_INCLUDED
