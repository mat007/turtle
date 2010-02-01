//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/format.hpp>
#include <boost/assign.hpp>
#include <ostream>
#include <deque>
#include <list>
#include <vector>
#include <map>
#include <set>

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

BOOST_AUTO_TEST_CASE( strings_are_serialized_with_double_quotes )
{
    BOOST_CHECK_EQUAL( "\"string\"", mock::format( "string" ) );
    BOOST_CHECK_EQUAL( "\"string\"", mock::format( std::string( "string" ) ) );
}

BOOST_AUTO_TEST_CASE( std_pairs_are_serialized )
{
    BOOST_CHECK_EQUAL( "(3,42)", mock::format( std::make_pair( 3, 42.f ) ) );
}

BOOST_AUTO_TEST_CASE( std_deques_are_serialized )
{
    std::deque< int > d;
    d.push_back( 12 );
    d.push_back( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", mock::format( d ) );
}

BOOST_AUTO_TEST_CASE( std_lists_are_serialized )
{
    std::list< int > l;
    l.push_back( 12 );
    l.push_back( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", mock::format( l ) );
}

BOOST_AUTO_TEST_CASE( std_vectors_are_serialized )
{
    std::vector< int > v;
    v.push_back( 12 );
    v.push_back( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", mock::format( v ) );
}

BOOST_AUTO_TEST_CASE( std_maps_are_serialized )
{
    std::map< int, std::string > m;
    m[ 12 ] = "12";
    m[ 42 ] = "42";
    BOOST_CHECK_EQUAL( "((12,\"12\"),(42,\"42\"))", mock::format( m ) );
}

BOOST_AUTO_TEST_CASE( std_multimaps_are_serialized )
{
    std::multimap< int, std::string > m;
    m.insert( std::make_pair( 12, "12" ));
    m.insert( std::make_pair( 42, "42" ));
    BOOST_CHECK_EQUAL( "((12,\"12\"),(42,\"42\"))", mock::format( m ) );
}

BOOST_AUTO_TEST_CASE( std_sets_are_serialized )
{
    std::set< int > s;
    s.insert( 12 );
    s.insert( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", mock::format( s ) );
}

BOOST_AUTO_TEST_CASE( std_multisets_are_serialized )
{
    std::multiset< int > s;
    s.insert( 12 );
    s.insert( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", mock::format( s ) );
}

BOOST_AUTO_TEST_CASE( boost_assign_list_of_are_serialized )
{
    BOOST_CHECK_EQUAL( "(12,42)", mock::format( boost::assign::list_of( 12 )( 42 ) ) );
}

BOOST_AUTO_TEST_CASE( boost_assign_map_list_of_are_serialized )
{
    BOOST_CHECK_EQUAL( "((12,\"12\"),(42,\"42\"))", mock::format( boost::assign::map_list_of( 12, "12" )( 42, "42" ) ) );
}
