//
//  Copyright Mathieu Champlon 2009
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_CONFIG_HPP_INCLUDED
#define MOCK_CONFIG_HPP_INCLUDED

#include <boost/preprocessor/inc.hpp>

#ifndef MOCK_MAX_ARGS
#  define MOCK_MAX_ARGS 9
#endif
#define MOCK_NUM_ARGS BOOST_PP_INC(MOCK_MAX_ARGS)

#if defined(MOCK_USE_BOOST_PHOENIX) || (MOCK_MAX_ARGS > 9)
#   ifndef PHOENIX_LIMIT
#       define PHOENIX_LIMIT MOCK_MAX_ARGS
#   elif (PHOENIX_LIMIT < MOCK_MAX_ARGS)
#       error "PHOENIX_LIMIT is set too low"
#   endif
#else
#   define MOCK_USE_BOOST_BIND
#endif

#ifndef BOOST_FUNCTION_MAX_ARGS
#   define BOOST_FUNCTION_MAX_ARGS MOCK_MAX_ARGS
#elif (BOOST_FUNCTION_MAX_ARGS < MOCK_MAX_ARGS)
#   error "BOOST_FUNCTION_MAX_ARGS is set too low"
#endif

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
