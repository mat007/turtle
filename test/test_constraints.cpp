// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/constraints.hpp>
#include <turtle/detail/operators.hpp>
#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE( all_comparison_constraints_can_be_instanciated )
{
    mock::equal( 0 );
    mock::less( 0 );
    mock::greater( 0 );
    mock::less_equal( 0 );
    mock::greater_equal( 0 );
}

BOOST_AUTO_TEST_CASE( constraints_can_be_negated_using_the_not_operator )
{
    ! mock::any;
    ! mock::affirm;
    ! mock::negate;
    ! mock::evaluate;
    ! mock::equal( 0 );
    ! mock::less( 0 );
    ! mock::greater( 0 );
    ! mock::less_equal( 0 );
    ! mock::greater_equal( 0 );
}

BOOST_AUTO_TEST_CASE( constraints_can_be_combined_using_the_or_operator )
{
    mock::less( 0 ) || mock::greater( 0 );
}

BOOST_AUTO_TEST_CASE( constraints_can_be_combined_using_the_and_operator )
{
    mock::less( 0 ) && mock::greater( 0 );
}

BOOST_AUTO_TEST_CASE( equal )
{
    BOOST_CHECK( mock::equal( std::string( "string" ) ).f_( "string" ) );
    BOOST_CHECK( ! mock::equal( std::string( "string" ) ).f_( "not string" ) );
    {
        std::string s;
        mock::constraint<
            mock::detail::equal<
                boost::reference_wrapper< const std::string >
            >
        > c = mock::equal( boost::cref( s ) );
        s = "string";
        BOOST_CHECK( c.f_( "string" ) );
    }
}

BOOST_AUTO_TEST_CASE( same )
{
    {
        int i = 0;
        int j = 0;
        BOOST_CHECK_EQUAL( i, j );
        BOOST_CHECK( ! mock::same( i ).f_( j ) );
        BOOST_CHECK( mock::same( i ).f_( i ) );
    }
    {
        int i = 0;
        int j = 0;
        BOOST_CHECK_EQUAL( i, j );
        mock::constraint<
            mock::detail::same<
                const boost::reference_wrapper< const int >
            >
        > c = mock::same( boost::cref( i ) );
        BOOST_CHECK( ! c.f_( j ) );
        BOOST_CHECK( c.f_( i ) );
    }
}

BOOST_AUTO_TEST_CASE( assign )
{
    {
        int i = 0;
        BOOST_CHECK( mock::assign( 3 ).f_( i ) );
        BOOST_CHECK_EQUAL( 3, i );
    }
    {
        int i = 0;
        BOOST_CHECK( mock::assign( 3 ).f_( &i ) );
        BOOST_CHECK_EQUAL( 3, i );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::assign( &j ).f_( i ) );
        BOOST_CHECK_EQUAL( &j, i );
    }
    {
        int i = 0;
        int j = 1;
        mock::constraint<
            mock::detail::assign<
                boost::reference_wrapper< const int >
            >
        > c = mock::assign( boost::cref( j ) );
        BOOST_CHECK( c.f_( i ) );
        BOOST_CHECK_EQUAL( 1, i );
        j = 3;
        BOOST_CHECK( c.f_( i ) );
        BOOST_CHECK_EQUAL( 3, i );
    }
    {
        int i = 0;
        int j = 1;
        mock::constraint<
            mock::detail::assign<
                boost::reference_wrapper< const int >
            >
        > c = mock::assign( boost::cref( j ) );
        BOOST_CHECK( c.f_( &i ) );
        BOOST_CHECK_EQUAL( 1, i );
        j = 3;
        BOOST_CHECK( c.f_( &i ) );
        BOOST_CHECK_EQUAL( 3, i );
    }
    {
        const int* i = 0;
        int k = 1;
        int* j = &k;
        mock::constraint<
            mock::detail::assign<
                boost::reference_wrapper< int* const >
            >
        > c = mock::assign( boost::cref( j ) );
        BOOST_CHECK( c.f_( i ) );
        BOOST_CHECK_EQUAL( j, i );
        j = 0;
        BOOST_CHECK( c.f_( i ) );
        BOOST_CHECK_EQUAL( j, i );
    }
}

BOOST_AUTO_TEST_CASE( retrieve )
{
    {
        int i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( i ).f_( j ) );
        BOOST_CHECK_EQUAL( i, j );
    }
    {
        int* i = 0;
        int j = 1;
        BOOST_CHECK( mock::retrieve( i ).f_( &j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( i ).f_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int* i = 0;
        int j = 1;
        BOOST_CHECK( mock::retrieve( i ).f_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        int* i = 0;
        int j = 1;
        BOOST_CHECK( mock::retrieve( i ).f_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( i ).f_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        int** i = 0;
        int* j = 0;
        BOOST_CHECK( mock::retrieve( i ).f_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int** i = 0;
        const int* j = 0;
        BOOST_CHECK( mock::retrieve( i ).f_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        int i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( boost::ref( i ) ).f_( j ) );
        BOOST_CHECK_EQUAL( i, j );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( boost::ref( i ) ).f_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
}

namespace
{
    struct A
    {
    };
    struct B
    {
        B& operator=( const A& )
        {
            return *this;
        }
    };
}

BOOST_AUTO_TEST_CASE( retrieve_uses_assignment_operator )
{
    B b;
    const A a = A();
    mock::retrieve( b ).f_( a );
}

BOOST_AUTO_TEST_CASE( affirm )
{
    int* i = 0;
    int j;
    BOOST_CHECK( ! mock::affirm.f_( i ) );
    BOOST_CHECK( mock::affirm.f_( &j ) );
}

BOOST_AUTO_TEST_CASE( negate )
{
    int* i = 0;
    int j;
    BOOST_CHECK( mock::negate.f_( i ) );
    BOOST_CHECK( ! mock::negate.f_( &j ) );
}

namespace
{
    bool return_true()
    {
        return true;
    }
    bool return_false()
    {
        return false;
    }
}

BOOST_AUTO_TEST_CASE( call )
{
    BOOST_CHECK( mock::call( &return_true ).f_() );
    BOOST_CHECK( ! mock::call( &return_false ).f_() );
}

BOOST_AUTO_TEST_CASE( evaluate )
{
    BOOST_CHECK( mock::evaluate.f_( &return_true ) );
    BOOST_CHECK( ! mock::evaluate.f_( &return_false ) );
}

BOOST_AUTO_TEST_CASE( contain_with_const_char_ptr )
{
    BOOST_CHECK( mock::contain( "string" ).f_( "this is a string" ) );
    BOOST_CHECK( mock::contain( "string" ).f_( std::string( "this is a string" ) ) );
    BOOST_CHECK( ! mock::contain( "not found" ).f_( "this is a string" ) );
    BOOST_CHECK( ! mock::contain( "not found" ).f_( std::string( "this is a string" ) ) );
    {
        const char* s;
        mock::constraint<
            mock::detail::contain<
                boost::reference_wrapper< const char* const >
            >
        > c = mock::contain( boost::cref( s ) );
        s = "string";
        BOOST_CHECK( c.f_( "this is a string" ) );
        BOOST_CHECK( c.f_( std::string( "this is a string" ) ) );
        s = "not found";
        BOOST_CHECK( ! c.f_( "this is a string" ) );
        BOOST_CHECK( ! c.f_( std::string( "this is a string" ) ) );
    }
}

BOOST_AUTO_TEST_CASE( contain_with_strings )
{
    BOOST_CHECK( mock::contain( std::string( "string" ) ).f_( "this is a string" ) );
    BOOST_CHECK( mock::contain( std::string( "string" ) ).f_( std::string( "this is a string" ) ) );
    BOOST_CHECK( ! mock::contain( std::string( "not found" ) ).f_( "this is a string" ) );
    BOOST_CHECK( ! mock::contain( std::string( "not found" ) ).f_( std::string( "this is a string" ) ) );
    {
        std::string s;
        mock::constraint<
            mock::detail::contain<
                boost::reference_wrapper< const std::string >
            >
        > c = mock::contain( boost::cref( s ) );
        s = "string";
        BOOST_CHECK( c.f_( "this is a string" ) );
        BOOST_CHECK( c.f_( std::string( "this is a string" ) ) );
        s = "not found";
        BOOST_CHECK( ! c.f_( "this is a string" ) );
        BOOST_CHECK( ! c.f_( std::string( "this is a string" ) ) );
    }
}

namespace
{
    struct type_with_overloaded_address_operator
    {
        void operator&() {}
        void operator&() const {}
    };
}

BOOST_AUTO_TEST_CASE( type_with_overloaded_address_operator_can_be_used_in_constraints )
{
    type_with_overloaded_address_operator t;
    mock::same( t ).f_( t );
    mock::retrieve( t ).f_( t );
    type_with_overloaded_address_operator* pt;
    mock::retrieve( pt ).f_( t );
}
