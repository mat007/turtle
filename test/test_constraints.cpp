// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/constraints.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/typeof/typeof.hpp>

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

BOOST_AUTO_TEST_CASE( equal_constraint )
{
    BOOST_CHECK( mock::equal( std::string( "string" ) ).c_( "string" ) );
    BOOST_CHECK( ! mock::equal( std::string( "string" ) ).c_( "not string" ) );
    {
        std::string s;
        BOOST_AUTO( c, mock::equal( boost::cref( s ) ) );
        s = "string";
        BOOST_CHECK( c.c_( "string" ) );
    }
#ifdef MOCK_SMART_PTR
    {
        std::unique_ptr< int > i;
        std::unique_ptr< int > j( new int( 3 ) );
        BOOST_CHECK( ! mock::equal( i ).c_( j ) );
        BOOST_CHECK( ! mock::equal( j ).c_( i ) );
        BOOST_CHECK( mock::equal( i ).c_( i ) );
        BOOST_CHECK( mock::equal( j ).c_( j ) );
    }
#endif
}

BOOST_AUTO_TEST_CASE( equal_constraint_deref )
{
    {
        int i = 3;
        BOOST_CHECK( mock::equal( 3 ).c_( &i ) );
        BOOST_CHECK( ! mock::equal( 7 ).c_( &i ) );
    }
    {
        int* i = 0;
        BOOST_CHECK( ! mock::equal( 3 ).c_( i ) );
    }
#ifdef MOCK_SMART_PTR
    {
        std::unique_ptr< int > j( new int( 3 ) );
        BOOST_CHECK( mock::equal( 3 ).c_( j ) );
        std::unique_ptr< int > i;
        BOOST_CHECK( ! mock::equal( 3 ).c_( i ) );
    }
#endif // MOCK_SMART_PTR
}

BOOST_AUTO_TEST_CASE( same_constraint )
{
    {
        int i = 0;
        int j = 0;
        BOOST_CHECK_EQUAL( i, j );
        BOOST_CHECK( ! mock::same( i ).c_( j ) );
        BOOST_CHECK( mock::same( i ).c_( i ) );
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
        BOOST_CHECK( ! c.c_( j ) );
        BOOST_CHECK( c.c_( i ) );
    }
#ifdef MOCK_NULLPTR
    {
        std::nullptr_t p;
        BOOST_CHECK( mock::same( p ).c_( p ) );
    }
#endif
}

BOOST_AUTO_TEST_CASE( assign_constraint )
{
    {
        int i = 0;
        BOOST_CHECK( mock::assign( 3 ).c_( i ) );
        BOOST_CHECK_EQUAL( 3, i );
    }
    {
        int i = 0;
        BOOST_CHECK( mock::assign( 3 ).c_( &i ) );
        BOOST_CHECK_EQUAL( 3, i );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::assign( &j ).c_( i ) );
        BOOST_CHECK_EQUAL( &j, i );
    }
    {
        int* i = 0;
        const int j = 1;
        BOOST_CHECK( ! mock::assign( j ).c_( i ) );
        BOOST_CHECK( ! i );
    }
    {
        int i = 0;
        int j = 1;
        mock::constraint<
            mock::detail::assign<
                boost::reference_wrapper< const int >
            >
        > c = mock::assign( boost::cref( j ) );
        BOOST_CHECK( c.c_( i ) );
        BOOST_CHECK_EQUAL( 1, i );
        j = 3;
        BOOST_CHECK( c.c_( i ) );
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
        BOOST_CHECK( c.c_( &i ) );
        BOOST_CHECK_EQUAL( 1, i );
        j = 3;
        BOOST_CHECK( c.c_( &i ) );
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
        BOOST_CHECK( c.c_( i ) );
        BOOST_CHECK_EQUAL( j, i );
        j = 0;
        BOOST_CHECK( c.c_( i ) );
        BOOST_CHECK_EQUAL( j, i );
    }
}

BOOST_AUTO_TEST_CASE( retrieve_constraint )
{
    {
        int i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( i ).c_( j ) );
        BOOST_CHECK_EQUAL( i, j );
    }
    {
        int* i = 0;
        int j = 1;
        BOOST_CHECK( mock::retrieve( i ).c_( &j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( i ).c_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int* i = 0;
        int j = 1;
        BOOST_CHECK( mock::retrieve( i ).c_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        int* i = 0;
        int j = 1;
        BOOST_CHECK( mock::retrieve( i ).c_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( i ).c_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        int** i = 0;
        int* j = 0;
        BOOST_CHECK( mock::retrieve( i ).c_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int** i = 0;
        const int* j = 0;
        BOOST_CHECK( mock::retrieve( i ).c_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        int i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( boost::ref( i ) ).c_( j ) );
        BOOST_CHECK_EQUAL( i, j );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( boost::ref( i ) ).c_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
#ifdef MOCK_NULLPTR
    {
        std::nullptr_t* i = 0;
        std::nullptr_t j;
        BOOST_CHECK( mock::retrieve( i ).c_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
#endif
#ifdef MOCK_SMART_PTR
    {
        std::unique_ptr< int > i;
        std::unique_ptr< int > j( new int( 3 ) );
        BOOST_CHECK( mock::retrieve( i ).c_( boost::move( j ) ) );
        BOOST_REQUIRE( i );
        BOOST_CHECK_EQUAL( 3, *i );
        BOOST_CHECK( !j );
    }
#endif
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

BOOST_AUTO_TEST_CASE( retrieve_constraint_uses_assignment_operator )
{
    B b;
    const A a = A();
    mock::retrieve( b ).c_( a );
}

BOOST_AUTO_TEST_CASE( affirm_constraint )
{
    {
        int* i = 0;
        int j;
        BOOST_CHECK( ! mock::affirm.c_( i ) );
        BOOST_CHECK( mock::affirm.c_( &j ) );
    }
    {
#ifdef MOCK_SMART_PTR
        std::unique_ptr< int > i;
        std::unique_ptr< int > j( new int( 3 ) );
        BOOST_CHECK( ! mock::affirm.c_( i ) );
        BOOST_CHECK( mock::affirm.c_( j ) );
#endif
    }
}

BOOST_AUTO_TEST_CASE( negate_constraint )
{
    int* i = 0;
    int j;
    BOOST_CHECK( mock::negate.c_( i ) );
    BOOST_CHECK( ! mock::negate.c_( &j ) );
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

BOOST_AUTO_TEST_CASE( call_constraint )
{
    BOOST_CHECK( mock::call( &return_true ).c_() );
    BOOST_CHECK( ! mock::call( &return_false ).c_() );
}

BOOST_AUTO_TEST_CASE( evaluate_constraint )
{
    BOOST_CHECK( mock::evaluate.c_( &return_true ) );
    BOOST_CHECK( ! mock::evaluate.c_( &return_false ) );
}

BOOST_AUTO_TEST_CASE( contain_constraint_with_const_char_ptr )
{
    BOOST_CHECK( mock::contain( "string" ).c_( "this is a string" ) );
    BOOST_CHECK( mock::contain( "string" ).c_( std::string( "this is a string" ) ) );
    BOOST_CHECK( ! mock::contain( "not found" ).c_( "this is a string" ) );
    BOOST_CHECK( ! mock::contain( "not found" ).c_( std::string( "this is a string" ) ) );
    {
        const char* s = 0;
        mock::constraint<
            mock::detail::contain<
                boost::reference_wrapper< const char* const >
            >
        > c = mock::contain( boost::cref( s ) );
        s = "string";
        BOOST_CHECK( c.c_( "this is a string" ) );
        BOOST_CHECK( c.c_( std::string( "this is a string" ) ) );
        s = "not found";
        BOOST_CHECK( ! c.c_( "this is a string" ) );
        BOOST_CHECK( ! c.c_( std::string( "this is a string" ) ) );
    }
}

BOOST_AUTO_TEST_CASE( contain_constraint_with_strings )
{
    BOOST_CHECK( mock::contain( std::string( "string" ) ).c_( "this is a string" ) );
    BOOST_CHECK( mock::contain( std::string( "string" ) ).c_( std::string( "this is a string" ) ) );
    BOOST_CHECK( ! mock::contain( std::string( "not found" ) ).c_( "this is a string" ) );
    BOOST_CHECK( ! mock::contain( std::string( "not found" ) ).c_( std::string( "this is a string" ) ) );
    {
        std::string s;
        mock::constraint<
            mock::detail::contain<
                boost::reference_wrapper< const std::string >
            >
        > c = mock::contain( boost::cref( s ) );
        s = "string";
        BOOST_CHECK( c.c_( "this is a string" ) );
        BOOST_CHECK( c.c_( std::string( "this is a string" ) ) );
        s = "not found";
        BOOST_CHECK( ! c.c_( "this is a string" ) );
        BOOST_CHECK( ! c.c_( std::string( "this is a string" ) ) );
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
    mock::same( t ).c_( t );
    mock::retrieve( t ).c_( t );
    type_with_overloaded_address_operator* pt;
    mock::retrieve( pt ).c_( t );
}

BOOST_AUTO_TEST_CASE( close_constraint )
{
    BOOST_CHECK( mock::close( 12.0, 0.0001 ).c_( 12 ) );
    BOOST_CHECK( ! mock::close( 12.0, 0.0001 ).c_( 13 ) );
}

BOOST_AUTO_TEST_CASE( close_fraction_constraint )
{
    BOOST_CHECK( mock::close_fraction( 12.0, 0.0001 ).c_( 12 ) );
    BOOST_CHECK( ! mock::close_fraction( 12.0, 0.0001 ).c_( 13 ) );
}

BOOST_AUTO_TEST_CASE( small_constraint )
{
    BOOST_CHECK( mock::small( 0.0001 ).c_( 0. ) );
    BOOST_CHECK( ! mock::small( 0.0001 ).c_( 12. ) );
}

BOOST_AUTO_TEST_CASE( near_constraint )
{
    BOOST_CHECK( mock::near( 12.0, 0.0001 ).c_( 12 ) );
    BOOST_CHECK( ! mock::near( 12.0, 0.0001 ).c_( 13 ) );
}
