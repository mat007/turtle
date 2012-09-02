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
        return mock::detail::check< Actual, Expected >( expected )( actual );
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
    BOOST_CHECK( check( 3, boost::cref( 3 ) ) );
    BOOST_CHECK( ! check( 4, boost::cref( 3 ) ) );
}

namespace
{
    struct fixture
    {
        fixture()
            : text( "same text" )
            , actual( text.c_str() )
        {
            const char* static_string = "same text";
            BOOST_REQUIRE( actual != static_string );
            BOOST_REQUIRE( actual == std::string( static_string ) );
        }
        std::string text;
        const char* actual;
    };
}

BOOST_FIXTURE_TEST_CASE( const_char_pointer_and_const_char_pointer_can_be_compared, fixture )
{
    const char* expected = "same text";
    BOOST_CHECK( check( expected, actual ) );
    const char* unexpected = "different text";
    BOOST_CHECK( ! check( actual, unexpected ) );
}

BOOST_FIXTURE_TEST_CASE( const_char_pointer_and_string_literal_can_be_compared, fixture )
{
    BOOST_CHECK( check( "same text", actual ) );
    BOOST_CHECK( ! check( "different text", actual ) );
}

BOOST_FIXTURE_TEST_CASE( const_char_pointer_and_const_char_array_can_be_compared, fixture )
{
    const char expected[10] = "same text";
    BOOST_CHECK( check( expected, actual ) );
    const char unexpected[15] = "different text";
    BOOST_CHECK( ! check( unexpected, actual ) );
}

BOOST_FIXTURE_TEST_CASE( const_char_pointer_and_std_string_can_be_compared, fixture )
{
    BOOST_CHECK( check( std::string( "same text" ), actual ) );
    BOOST_CHECK( ! check( std::string( "different text" ), actual ) );
}
