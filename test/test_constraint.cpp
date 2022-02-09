// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2013
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/constraint.hpp>
#include <boost/test/unit_test.hpp>
#include <sstream>

namespace {
MOCK_CONSTRAINT(constraint_0, actual == 0)
MOCK_CONSTRAINT(constraint_1, expected, actual == expected)
MOCK_CONSTRAINT(constraint_2, expected_0, expected_1, actual == expected_0 || actual == expected_1)

template<typename T>
std::string to_string(const mock::constraint<T>& t)
{
    std::ostringstream s;
    s << t.c_;
    return s.str();
}
} // namespace

BOOST_AUTO_TEST_CASE(mock_constraint_works_for_0_to_2_args)
{
    BOOST_TEST(constraint_0.c_(0));
    BOOST_TEST(!constraint_0.c_(42));

    BOOST_TEST(constraint_1(0).c_(0));
    BOOST_TEST(!constraint_1(1).c_(0));
    BOOST_TEST(!constraint_1(42).c_(1337));

    BOOST_TEST(constraint_2(42, 1337).c_(42));
    BOOST_TEST(constraint_2(42, 1337).c_(1337));
    BOOST_TEST(!constraint_2(42, 1337).c_(99));
}

BOOST_AUTO_TEST_CASE(mock_constraint_outputs_human_readable_representation)
{
    BOOST_TEST(to_string(constraint_0) == "constraint_0");
    BOOST_TEST(to_string(constraint_1(42)) == "constraint_1( 42 )");
    BOOST_TEST(to_string(constraint_2(42, 1337)) == "constraint_2( 42, 1337 )");
}
