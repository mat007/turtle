// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/detail/invocation.hpp>
#include <boost/test/unit_test.hpp>
#include <sstream>

/// Serialize using the base class as done by e.g. the expectation class
std::string to_string(const mock::detail::invocation& invocation)
{
    std::ostringstream s;
    s << invocation;
    return s.str();
}

BOOST_AUTO_TEST_CASE(unlimited)
{
    mock::detail::unlimited invocation;
    BOOST_TEST(invocation.verify());
    BOOST_TEST(!invocation.exhausted());
    BOOST_TEST(invocation.invoke());
    BOOST_TEST(invocation.verify());
    BOOST_TEST(!invocation.exhausted());
    BOOST_TEST(invocation.invoke());
    BOOST_TEST(to_string(invocation) == "unlimited()");
}

BOOST_AUTO_TEST_CASE(once)
{
    mock::detail::once invocation;
    BOOST_TEST(!invocation.verify());
    BOOST_TEST(!invocation.exhausted());
    BOOST_TEST(invocation.invoke());
    BOOST_TEST(invocation.verify());
    BOOST_TEST(invocation.exhausted());
    BOOST_TEST(!invocation.invoke());
    BOOST_TEST(to_string(invocation) == "once()");
}

BOOST_AUTO_TEST_CASE(never)
{
    mock::detail::never invocation;
    BOOST_TEST(invocation.verify());
    BOOST_TEST(invocation.exhausted());
    BOOST_TEST(!invocation.invoke());
    BOOST_TEST(to_string(invocation) == "never()");
}

BOOST_AUTO_TEST_CASE(at_most)
{
    mock::detail::at_most invocation(1);
    BOOST_TEST(to_string(invocation) == "at_most( 0/1 )");
    BOOST_TEST(invocation.verify());
    BOOST_TEST(!invocation.exhausted());
    BOOST_TEST(invocation.invoke());
    BOOST_TEST(to_string(invocation) == "at_most( 1/1 )");
    BOOST_TEST(invocation.verify());
    BOOST_TEST(invocation.exhausted());
    BOOST_TEST(!invocation.invoke());
    BOOST_TEST(to_string(invocation) == "at_most( 1/1 )");
}

BOOST_AUTO_TEST_CASE(at_least)
{
    mock::detail::at_least invocation(1);
    BOOST_TEST(to_string(invocation) == "at_least( 0/1 )");
    BOOST_TEST(!invocation.verify());
    BOOST_TEST(!invocation.exhausted());
    BOOST_TEST(invocation.invoke());
    BOOST_TEST(invocation.verify());
    BOOST_TEST(to_string(invocation) == "at_least( 1/1 )");
    BOOST_TEST(!invocation.exhausted());
    BOOST_TEST(invocation.invoke());
    BOOST_TEST(invocation.verify());
    BOOST_TEST(to_string(invocation) == "at_least( 2/1 )");
}

BOOST_AUTO_TEST_CASE(between)
{
    {
        mock::detail::between invocation(1, 2);
        BOOST_TEST(to_string(invocation) == "between( 0/[1,2] )");
        BOOST_TEST(!invocation.verify());
        BOOST_TEST(!invocation.exhausted());
        BOOST_TEST(invocation.invoke());
        BOOST_TEST(invocation.verify());
        BOOST_TEST(to_string(invocation) == "between( 1/[1,2] )");
        BOOST_TEST(!invocation.exhausted());
        BOOST_TEST(invocation.invoke());
        BOOST_TEST(invocation.verify());
        BOOST_TEST(to_string(invocation) == "between( 2/[1,2] )");
        BOOST_TEST(invocation.exhausted());
        BOOST_TEST(!invocation.invoke());
        BOOST_TEST(to_string(invocation) == "between( 2/[1,2] )");
    }
    {
        mock::detail::between invocation(2, 2);
        BOOST_TEST(to_string(invocation) == "between( 0/[2,2] )");
        BOOST_TEST(!invocation.verify());
        BOOST_TEST(!invocation.exhausted());
        BOOST_TEST(invocation.invoke());
        BOOST_TEST(!invocation.verify());
        BOOST_TEST(to_string(invocation) == "between( 1/[2,2] )");
        BOOST_TEST(!invocation.exhausted());
        BOOST_TEST(invocation.invoke());
        BOOST_TEST(invocation.verify());
        BOOST_TEST(to_string(invocation) == "between( 2/[2,2] )");
        BOOST_TEST(invocation.exhausted());
        BOOST_TEST(!invocation.invoke());
        BOOST_TEST(to_string(invocation) == "between( 2/[2,2] )");
    }

    // First must be equal or less than 2nd
    BOOST_CHECK_THROW(mock::detail::between invalid(2, 1), std::invalid_argument);
}
