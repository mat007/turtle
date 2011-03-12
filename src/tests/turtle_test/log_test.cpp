//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/log.hpp>
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
        s << ::mock::format( t );
        return s.str();
    }
}

namespace
{
    struct non_serializable
    {};

    void log( std::ostream&, const non_serializable& )
    {
        BOOST_FAIL( "should not be called" );
    }
    void log( std::ostream&, mock::protect< non_serializable > )
    {
        BOOST_FAIL( "should not be called" );
    }
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
    struct loggable {};

    std::ostream& operator<<( std::ostream& s, const loggable& )
    {
        BOOST_FAIL( "should not be called" );
        return s;
    }
}

BOOST_AUTO_TEST_CASE( mock_log_overrides_standard_stream_serialization_even_if_defined_after_being_used )
{
    BOOST_CHECK_EQUAL( "loggable", to_string( loggable() ) );
}

namespace mock
{
    void log( std::ostream& s, const loggable& )
    {
        s << "loggable";
    }
}

namespace
{
    struct protected_loggable {};

    std::ostream& operator<<( std::ostream& s, const protected_loggable& )
    {
        BOOST_FAIL( "should not be called" );
        return s;
    }
}

namespace mock
{
    void log( std::ostream&, const protected_loggable& )
    {
        BOOST_FAIL( "should not be called" );
    }
}

BOOST_AUTO_TEST_CASE( protected_mock_log_overrides_standard_stream_serialization_and_mock_log_even_if_defined_after_being_used )
{
    BOOST_CHECK_EQUAL( "protected_loggable", to_string( protected_loggable() ) );
}

namespace mock
{
    void log( std::ostream& s, mock::protect< protected_loggable > )
    {
        s << "protected_loggable";
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
    void callable_builtin()
    {}
}

BOOST_AUTO_TEST_CASE( callable_builtin_cannot_be_serialized )
{
    BOOST_CHECK_EQUAL( "?", to_string( &callable_builtin ) );
    BOOST_CHECK_EQUAL( "?", to_string( callable_builtin ) );
}

namespace
{
    struct false_positive_container
    {
        typedef int const_iterator;
    };
    BOOST_MPL_ASSERT(( mock::detail::is_container< false_positive_container > ));
}

namespace mock
{
    void log( std::ostream& s, false_positive_container )
    {
        s << "false_positive_container";
    }
}

BOOST_AUTO_TEST_CASE( false_positive_container_serialization_can_be_overriden )
{
    BOOST_CHECK_EQUAL( "false_positive_container", to_string( false_positive_container() ) );
}

namespace
{
    template< typename T >
    struct template_type
    {};
}

namespace mock
{
    template< typename T >
    void log( std::ostream& s, template_type< T > )
    {
        s << "template_type";
    }
}

BOOST_AUTO_TEST_CASE( template_type_serialization_can_be_overriden )
{
    BOOST_CHECK_EQUAL( "template_type", to_string( template_type< int >() ) );
}

namespace mock
{
    void log( std::ostream& s, const std::vector< float >& )
    {
        s << "vector< float >";
    }
}

BOOST_AUTO_TEST_CASE( vector_of_floats_serialization_can_be_overriden )
{
    BOOST_CHECK_EQUAL( "vector< float >", to_string( std::vector< float >() ) );
}

namespace
{
    struct convertible_to_many
    {
        operator bool() const;
        operator std::string() const;
        operator const char*() const;

        operator std::pair< int, int >() const;
        template< typename T1, typename T2 > operator std::pair< T1, T2 >() const;

        typedef void (*pf)();
        operator pf() const;

        operator std::set< int >() const;
        template< typename T > operator std::set< T >() const;
    };
}

BOOST_AUTO_TEST_CASE( build_in_log_customizations_do_not_yield_ambiguous_errors )
{
    BOOST_CHECK_EQUAL( "?", to_string( convertible_to_many() ) );
}
