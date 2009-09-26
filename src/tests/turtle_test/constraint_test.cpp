//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/constraint.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

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

BOOST_AUTO_TEST_CASE( same )
{
    int i = 0;
    int j = 0;
    BOOST_CHECK_EQUAL( i, j );
    BOOST_CHECK( ! mock::same( i ).functor_( j ) );
    BOOST_CHECK( mock::same( i ).functor_( i ) );
}

BOOST_AUTO_TEST_CASE( assign )
{
    {
        int i = 0;
        BOOST_CHECK( mock::assign( 3 ).functor_( i ) );
        BOOST_CHECK_EQUAL( 3, i );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::assign( &j ).functor_( i ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
}

BOOST_AUTO_TEST_CASE( retrieve )
{
    {
        int i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( i ).functor_( j ) );
        BOOST_CHECK_EQUAL( i, j );
    }
    {
        int* i = 0;
        int j = 1;
        BOOST_CHECK( mock::retrieve( i ).functor_( &j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( i ).functor_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        int* i = 0;
        int j = 1;
        BOOST_CHECK( mock::retrieve( i ).functor_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int* i = 0;
        const int j = 1;
        BOOST_CHECK( mock::retrieve( i ).functor_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        int** i = 0;
        int* j = 0;
        BOOST_CHECK( mock::retrieve( i ).functor_( j ) );
        BOOST_CHECK_EQUAL( i, &j );
    }
    {
        const int** i = 0;
        const int* j = 0;
        BOOST_CHECK( mock::retrieve( i ).functor_( j ) );
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
    mock::retrieve( b ).functor_( a );
}

BOOST_AUTO_TEST_CASE( negate )
{
    int* i = 0;
    int j;
    BOOST_CHECK( mock::negate.functor_( i ) );
    BOOST_CHECK( ! mock::negate.functor_( &j ) );
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

BOOST_AUTO_TEST_CASE( evaluate )
{
    BOOST_CHECK( mock::evaluate.functor_( &return_true ) );
    BOOST_CHECK( ! mock::evaluate.functor_( &return_false ) );
}
