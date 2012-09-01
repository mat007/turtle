// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/detail/check.hpp>
#include <boost/test/auto_unit_test.hpp>

namespace
{
    template< typename Expected, typename Actual >
    bool check( Expected expected, Actual actual )
    {
        return mock::detail::check< Expected, Actual >( expected )( actual );
    }
}

BOOST_AUTO_TEST_CASE( int_and_int_can_be_compared )
{
    BOOST_CHECK( check( 3, 3 ) );
    BOOST_CHECK( ! check( 3, 4 ) );
    BOOST_CHECK( ! check( 4, 3 ) );
}

BOOST_AUTO_TEST_CASE( ref_to_int_and_int_can_be_compared )
{
    BOOST_CHECK( check< int >( boost::cref( 3 ), 3 ) );
    BOOST_CHECK( ! check< int >( boost::cref( 3 ), 4 ) );
}
