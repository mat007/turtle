//
//  Copyright Mathieu Champlon 2009
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/is_functor.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

namespace
{
    template< typename T >
    void check( T )
    {
        BOOST_STATIC_ASSERT(( mock::detail::is_functor< T >::type::value ));
    }

    void f0 () {}
    bool f1( int ) { return false; }
    bool f2( std::string, int ) { return false; }

    struct unary_functor0 : public std::unary_function< void, void >
    {
    };
    struct unary_functor1 : public std::unary_function< int, void >
    {
    };

    struct s
    {
        typedef void result_type;
    };
}

BOOST_AUTO_TEST_CASE( function_is_functor )
{
    check( f0 );
    check( f1 );
    check( f2 );
}

BOOST_AUTO_TEST_CASE( function_pointer_is_functor )
{
    check( &f0 );
    check( &f1 );
    check( &f2 );
}

BOOST_AUTO_TEST_CASE( std_ptr_fun_is_functor )
{
    check( std::ptr_fun( &f1 ) );
    check( std::ptr_fun( &f2 ) );
}

BOOST_AUTO_TEST_CASE( std_bind_first_is_functor )
{
    check( std::bind1st( std::ptr_fun( &f2 ), "" ) );
}

BOOST_AUTO_TEST_CASE( std_unary_functor_is_functor )
{
    check( unary_functor0() );
    check( unary_functor1() );
}

BOOST_AUTO_TEST_CASE( boost_bind_is_functor )
{
    check( boost::bind( &f0 ) );
    check( boost::bind( &f1, _1 ) );
    check( boost::bind( &f2, "", _1 ) );
}

BOOST_AUTO_TEST_CASE( boost_function_is_functor )
{
    check( boost::function< void() >() );
}

BOOST_AUTO_TEST_CASE( class_with_result_type_is_functor )
{
    check( s() );
}
