// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/detail/function.hpp>
#include <boost/test/auto_unit_test.hpp>

namespace
{
    template< typename Expected, typename Actual >
    bool match( Expected expected, Actual actual )
    {
        return mock::matcher< Actual, Expected >( expected )( actual );
    }
}

BOOST_AUTO_TEST_CASE( int_and_int_can_be_compared )
{
    BOOST_CHECK( match( 3, 3 ) );
    BOOST_CHECK( ! match( 3, 4 ) );
    BOOST_CHECK( ! match( 4, 3 ) );
}

BOOST_AUTO_TEST_CASE( ref_to_int_and_int_can_be_compared )
{
    const int i = 3;
    BOOST_CHECK( match( 3, boost::cref( i ) ) );
    BOOST_CHECK( ! match( 4, boost::cref( i ) ) );
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
    BOOST_CHECK( match( expected, actual ) );
    const char* unexpected = "different text";
    BOOST_CHECK( ! match( actual, unexpected ) );
}

BOOST_FIXTURE_TEST_CASE( const_char_pointer_and_string_literal_can_be_compared, fixture )
{
    BOOST_CHECK( match( "same text", actual ) );
    BOOST_CHECK( ! match( "different text", actual ) );
}

BOOST_FIXTURE_TEST_CASE( const_char_pointer_and_const_char_array_can_be_compared, fixture )
{
    const char expected[10] = "same text";
    BOOST_CHECK( match( expected, actual ) );
    const char unexpected[15] = "different text";
    BOOST_CHECK( ! match( unexpected, actual ) );
}

BOOST_FIXTURE_TEST_CASE( const_char_pointer_and_std_string_can_be_compared, fixture )
{
    BOOST_CHECK( match( std::string( "same text" ), actual ) );
    BOOST_CHECK( ! match( std::string( "different text" ), actual ) );
}
