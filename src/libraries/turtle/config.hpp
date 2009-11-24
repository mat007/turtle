//
//  Copyright Mathieu Champlon 2009
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_CONFIG_HPP_INCLUDED
#define MOCK_CONFIG_HPP_INCLUDED

#include <boost/preprocessor/comparison/less_equal.hpp>
#include <boost/preprocessor/debug/assert.hpp>
#include <boost/function.hpp>

#ifndef MOCK_MAX_ARGS
#  define MOCK_MAX_ARGS 10
#endif // MOCK_MAX_ARGS

BOOST_PP_ASSERT( BOOST_PP_LESS_EQUAL(MOCK_MAX_ARGS, BOOST_FUNCTION_MAX_ARGS) )

#ifdef BOOST_TEST_DECL
#   define MOCK_USE_BOOST_TEST
#endif

#ifndef MOCK_ERROR_POLICY
#   ifdef MOCK_USE_BOOST_TEST
#       define MOCK_ERROR_POLICY boost_test_error_policy
#   else
#       define MOCK_ERROR_POLICY basic_error_policy
#   endif
#endif

#endif // #ifndef MOCK_CONFIG_HPP_INCLUDED
