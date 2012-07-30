// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_CONFIG_HPP_INCLUDED
#define MOCK_CONFIG_HPP_INCLUDED

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/comparison/less.hpp>

#ifndef MOCK_ERROR_POLICY
#   define MOCK_ERROR_POLICY mock::error
#   define MOCK_USE_BOOST_TEST
#endif

#ifndef MOCK_MAX_ARGS
#   define MOCK_MAX_ARGS 9
#elif BOOST_PP_LESS(9, MOCK_MAX_ARGS)
#   define MOCK_USE_BOOST_PHOENIX
#endif

#ifndef BOOST_FUNCTION_MAX_ARGS
#   define BOOST_FUNCTION_MAX_ARGS MOCK_MAX_ARGS
#elif BOOST_PP_LESS(BOOST_FUNCTION_MAX_ARGS, MOCK_MAX_ARGS)
#   error BOOST_FUNCTION_MAX_ARGS must be set to MOCK_MAX_ARGS or higher
#endif

#ifndef BOOST_FT_MAX_ARITY
#   define BOOST_FT_MAX_ARITY BOOST_PP_INC(MOCK_MAX_ARGS)
#elif BOOST_PP_LESS_EQUAL(BOOST_FT_MAX_ARITY, MOCK_MAX_ARGS)
#   error BOOST_FT_MAX_ARITY must be set to MOCK_MAX_ARGS + 1 or higher
#endif

#ifdef MOCK_USE_BOOST_PHOENIX
#   ifndef BOOST_RESULT_OF_NUM_ARGS
#       define BOOST_RESULT_OF_NUM_ARGS MOCK_MAX_ARGS
#   elif BOOST_PP_LESS(BOOST_RESULT_OF_NUM_ARGS, MOCK_MAX_ARGS)
#       error BOOST_RESULT_OF_NUM_ARGS must be set to MOCK_MAX_ARGS or higher
#   endif
#   ifndef PHOENIX_LIMIT
#       define PHOENIX_LIMIT MOCK_MAX_ARGS
#   elif BOOST_PP_LESS(PHOENIX_LIMIT, MOCK_MAX_ARGS)
#       error PHOENIX_LIMIT must be set to MOCK_MAX_ARGS or higher
#   endif
#endif

#endif // MOCK_CONFIG_HPP_INCLUDED
