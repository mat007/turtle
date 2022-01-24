// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2013
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/constraint.hpp>
#include <boost/test/unit_test.hpp>

namespace {
MOCK_CONSTRAINT(constraint_0, actual == 0)
MOCK_CONSTRAINT(constraint_1, expected, actual == expected)
MOCK_CONSTRAINT(constraint_2, expected_0, expected_1, actual == expected_0 || actual == expected_1)
} // namespace

BOOST_AUTO_TEST_CASE(mock_constraint_is_supported_by_compilers_with_variadic_macros)
{
    BOOST_CHECK(constraint_0.c_(0));
    BOOST_CHECK(constraint_1(0).c_(0));
    BOOST_CHECK(constraint_2(0, 0).c_(0));
}
