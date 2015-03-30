// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/detail/invocation.hpp>
#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE( unlimited )
{
    mock::detail::unlimited invocation;
    BOOST_CHECK( invocation.verify() );
    BOOST_CHECK( ! invocation.exhausted() );
    BOOST_CHECK( invocation.invoke() );
    BOOST_CHECK( invocation.verify() );
    BOOST_CHECK( ! invocation.exhausted() );
    BOOST_CHECK( invocation.invoke() );
}

BOOST_AUTO_TEST_CASE( once )
{
    mock::detail::once invocation;
    BOOST_CHECK( ! invocation.verify() );
    BOOST_CHECK( ! invocation.exhausted() );
    BOOST_CHECK( invocation.invoke() );
    BOOST_CHECK( invocation.verify() );
    BOOST_CHECK( invocation.exhausted() );
    BOOST_CHECK( ! invocation.invoke() );
}

BOOST_AUTO_TEST_CASE( never )
{
    mock::detail::never invocation;
    BOOST_CHECK( invocation.verify() );
    BOOST_CHECK( invocation.exhausted() );
    BOOST_CHECK( ! invocation.invoke() );
}

BOOST_AUTO_TEST_CASE( at_most )
{
    mock::detail::at_most invocation( 1 );
    BOOST_CHECK( invocation.verify() );
    BOOST_CHECK( ! invocation.exhausted() );
    BOOST_CHECK( invocation.invoke() );
    BOOST_CHECK( invocation.verify() );
    BOOST_CHECK( invocation.exhausted() );
    BOOST_CHECK( ! invocation.invoke() );
}

BOOST_AUTO_TEST_CASE( at_least )
{
    mock::detail::at_least invocation( 1 );
    BOOST_CHECK( ! invocation.verify() );
    BOOST_CHECK( ! invocation.exhausted() );
    BOOST_CHECK( invocation.invoke() );
    BOOST_CHECK( invocation.verify() );
    BOOST_CHECK( ! invocation.exhausted() );
    BOOST_CHECK( invocation.invoke() );
}

BOOST_AUTO_TEST_CASE( between )
{
    mock::detail::between invocation( 1, 2 );
    BOOST_CHECK( ! invocation.verify() );
    BOOST_CHECK( ! invocation.exhausted() );
    BOOST_CHECK( invocation.invoke() );
    BOOST_CHECK( invocation.verify() );
    BOOST_CHECK( ! invocation.exhausted() );
    BOOST_CHECK( invocation.invoke() );
    BOOST_CHECK( invocation.verify() );
    BOOST_CHECK( invocation.exhausted() );
    BOOST_CHECK( ! invocation.invoke() );
}
