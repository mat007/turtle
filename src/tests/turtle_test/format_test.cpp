//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/format.hpp>
#include <ostream>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

namespace
{
    struct non_serializable_type {};

    struct serializable_type {};
    std::ostream& operator<<( std::ostream& s, const serializable_type& )
    {
        return s << "serializable_type";
    }
}

BOOST_AUTO_TEST_CASE( type_not_serializable_in_standard_stream_yields_an_interrogation_mark_when_serialized )
{
    BOOST_CHECK_EQUAL( "?", mock::format( non_serializable_type() ) );
}

BOOST_AUTO_TEST_CASE( base_type_serializable_in_standard_stream_yields_its_value_when_serialized )
{
    BOOST_CHECK_EQUAL( "42", mock::format( 42 ) );
}

BOOST_AUTO_TEST_CASE( custom_type_serializable_in_standard_stream_yields_its_value_when_serialized )
{
    BOOST_CHECK_EQUAL( "serializable_type", mock::format( serializable_type() ) );
}

namespace
{
    struct convertible_to_int
    {
        operator int() const { return 0; }
    };
}

BOOST_AUTO_TEST_CASE( custom_type_convertible_to_base_type_yields_its_value_when_serialized )
{
    BOOST_CHECK_EQUAL( "0", mock::format( convertible_to_int() ) );
}

namespace
{
    struct serializable
    {
        friend std::ostream& operator<<( std::ostream& s, const serializable& )
        {
            return s << "serializable";
        }
    };
    struct convertible_to_serializable
    {
        operator serializable() const { return serializable(); }
    };
}

BOOST_AUTO_TEST_CASE( custom_type_convertible_to_another_type_serializable_in_standard_stream_yields_its_value_when_serialized )
{
    BOOST_CHECK_EQUAL( "serializable", mock::format( convertible_to_serializable() ) );
}

BOOST_AUTO_TEST_CASE( booleans_are_serialized_as_booleans )
{
    BOOST_CHECK_EQUAL( "true", mock::format( true ) );
    BOOST_CHECK_EQUAL( "false", mock::format( false ) );
}

BOOST_AUTO_TEST_CASE( strings_are_surrounded_with_double_quotes )
{
    BOOST_CHECK_EQUAL( "\"string\"", mock::format( "string" ) );
    BOOST_CHECK_EQUAL( "\"string\"", mock::format( std::string( "string" ) ) );
}
