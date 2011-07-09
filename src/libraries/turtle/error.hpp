//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_ERROR_HPP_INCLUDED
#define MOCK_ERROR_HPP_INCLUDED

#include "config.hpp"

#ifndef MOCK_ERROR_POLICY
#   ifdef MOCK_USE_BOOST_TEST
#       define MOCK_ERROR_POLICY boost_test_error_policy
#       include "boost_test_error.hpp"
#   else
#       define MOCK_ERROR_POLICY basic_error_policy
#       include "default_error.hpp"
#   endif
#endif

#endif // MOCK_ERROR_HPP_INCLUDED
