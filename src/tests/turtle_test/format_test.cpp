//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/format.hpp>
#include <boost/assign.hpp>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

namespace
{
    template< typename T >
    std::string to_string( T t )
    {
        std::stringstream s;
        s << mock::format( t );
        return s.str();
    }
}

namespace
{
    struct non_serializable
    {};
}

BOOST_AUTO_TEST_CASE( non_serializable_type_yields_an_interrogation_mark_when_serialized )
{
    BOOST_CHECK_EQUAL( "?", to_string( non_serializable() ) );
}

namespace
{
    struct serializable
    {};

    std::ostream& operator<<( std::ostream& s, const serializable& )
    {
        return s << "serializable";
    }
}

BOOST_AUTO_TEST_CASE( serializable_type_yields_its_value_when_serialized )
{
    BOOST_CHECK_EQUAL( "serializable", to_string( serializable() ) );
}

namespace
{
    struct formattable {};

    std::ostream& operator<<( std::ostream& s, const formattable& )
    {
        BOOST_FAIL( "should not be called" );
        return s;
    }
}

BOOST_AUTO_TEST_CASE( format_overrides_standard_stream_serialization_even_if_defined_after_being_used )
{
    BOOST_CHECK_EQUAL( "formattable", to_string( formattable() ) );
}

namespace
{
    void format( std::ostream& s, const formattable& )
    {
        s << "formattable";
    }
}

namespace
{
    struct protected_formattable {};

    std::ostream& operator<<( std::ostream& s, const protected_formattable& )
    {
        BOOST_FAIL( "should not be called" );
        return s;
    }

    void format( std::ostream&, const protected_formattable& )
    {
        BOOST_FAIL( "should not be called" );
    }
}

BOOST_AUTO_TEST_CASE( protected_format_overrides_standard_stream_serialization_and_format_even_if_defined_after_being_used )
{
    BOOST_CHECK_EQUAL( "protected_formattable", to_string( protected_formattable() ) );
}

namespace
{
    void format( std::ostream& s, mock::protect< protected_formattable > )
    {
        s << "protected_formattable";
    }
}

BOOST_AUTO_TEST_CASE( base_type_yields_its_value_when_serialized )
{
    BOOST_CHECK_EQUAL( "42", to_string( 42 ) );
}

BOOST_AUTO_TEST_CASE( booleans_are_serialized_as_bool_alpha )
{
    BOOST_CHECK_EQUAL( "true", to_string( true ) );
    BOOST_CHECK_EQUAL( "false", to_string( false ) );
}

BOOST_AUTO_TEST_CASE( strings_are_serialized_with_double_quotes )
{
    BOOST_CHECK_EQUAL( "\"string\"", to_string( "string" ) );
    BOOST_CHECK_EQUAL( "\"string\"", to_string( std::string( "string" ) ) );
}

BOOST_AUTO_TEST_CASE( std_pairs_are_serialized )
{
    BOOST_CHECK_EQUAL( "(3,42)", to_string( std::make_pair( 3, 42.f ) ) );
}

BOOST_AUTO_TEST_CASE( std_deques_are_serialized )
{
    std::deque< int > d;
    d.push_back( 12 );
    d.push_back( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", to_string( d ) );
}

BOOST_AUTO_TEST_CASE( std_lists_are_serialized )
{
    std::list< int > l;
    l.push_back( 12 );
    l.push_back( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", to_string( l ) );
}

BOOST_AUTO_TEST_CASE( std_vectors_are_serialized )
{
    std::vector< int > v;
    v.push_back( 12 );
    v.push_back( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", to_string( v ) );
}

BOOST_AUTO_TEST_CASE( std_maps_are_serialized )
{
    std::map< int, std::string > m;
    m[ 12 ] = "12";
    m[ 42 ] = "42";
    BOOST_CHECK_EQUAL( "((12,\"12\"),(42,\"42\"))", to_string( m ) );
}

BOOST_AUTO_TEST_CASE( std_multimaps_are_serialized )
{
    std::multimap< int, std::string > m;
    m.insert( std::make_pair( 12, "12" ));
    m.insert( std::make_pair( 42, "42" ));
    BOOST_CHECK_EQUAL( "((12,\"12\"),(42,\"42\"))", to_string( m ) );
}

BOOST_AUTO_TEST_CASE( std_sets_are_serialized )
{
    std::set< int > s;
    s.insert( 12 );
    s.insert( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", to_string( s ) );
}

BOOST_AUTO_TEST_CASE( std_multisets_are_serialized )
{
    std::multiset< int > s;
    s.insert( 12 );
    s.insert( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", to_string( s ) );
}

BOOST_AUTO_TEST_CASE( boost_assign_list_of_are_serialized )
{
    BOOST_CHECK_EQUAL( "(12,42)", to_string( boost::assign::list_of( 12 )( 42 ) ) );
}

BOOST_AUTO_TEST_CASE( boost_assign_map_list_of_are_serialized )
{
    BOOST_CHECK_EQUAL( "((12,\"12\"),(42,\"42\"))", to_string( boost::assign::map_list_of( 12, "12" )( 42, "42" ) ) );
}

namespace
{
    struct false_positive_container
    {
        typedef int const_iterator;
    };
    BOOST_MPL_ASSERT(( mock::detail::is_container< false_positive_container > ));

    void format( std::ostream& s, mock::protect< false_positive_container > )
    {
        s << "false_positive_container";
    }
}

BOOST_AUTO_TEST_CASE( false_positive_container_serialization_can_still_be_overriden )
{
    BOOST_CHECK_EQUAL( "false_positive_container", to_string( false_positive_container() ) );
}
