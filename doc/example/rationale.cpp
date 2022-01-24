// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "calculator.hpp"
#include "mock_view.hpp"
#include <turtle/mock.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <limits>
#include <stdexcept>

// Dummy to detect if the assertion unexpectedly succeeded to test what is explained
#undef BOOST_CHECK_THROW
#define BOOST_CHECK_THROW(expr, exc)                    \
    try                                                 \
    {                                                   \
        expr;                                           \
    } catch(const exc&)                                 \
    {                                                   \
        std::cerr << "Exception thrown but should not"; \
    }

//[ overflow_throws
BOOST_AUTO_TEST_CASE(overflow_throws)
{
    mock_view v;
    calculator c(v);
    BOOST_CHECK_THROW(c.add((std::numeric_limits<int>::max)(), 1), std::exception);
}
//]
