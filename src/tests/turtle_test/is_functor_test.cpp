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
#ifdef _MSC_VER
#   pragma warning( push, 0 )
#endif
#include <boost/lambda/lambda.hpp>
#include <boost/spirit/home/phoenix.hpp>
#ifdef _MSC_VER
#   pragma warning( pop )
#endif

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

namespace
{
    BOOST_STATIC_ASSERT( sizeof( mock::detail::yes_type ) != sizeof( mock::detail::no_type ) );

    template< typename T >
    void check( T )
    {
        BOOST_STATIC_ASSERT(( mock::detail::is_functor< T >::type::value ));
    }

    void f0 () {}
    bool f1( int ) { return false; }
    bool f2( std::string, int ) { return false; }
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

namespace
{
    struct unary_functor0 : public std::unary_function< void, void >
    {
    };
    struct unary_functor1 : public std::unary_function< int, void >
    {
    };
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

BOOST_AUTO_TEST_CASE( boost_lambda_is_functor )
{
    check( boost::lambda::_1 < 42 );
}

BOOST_AUTO_TEST_CASE( boost_phoenix_is_functor )
{
    check( boost::phoenix::arg_names::arg1 < 42 );
    check( boost::phoenix::arg_names::_1 < 42 );
}

BOOST_AUTO_TEST_CASE( boost_function_is_functor )
{
    check( boost::function< void() >() );
}

namespace
{
    struct result_type_functor
    {
        typedef void result_type;
    };
}
BOOST_AUTO_TEST_CASE( class_with_result_type_is_functor )
{
    check( result_type_functor() );
}

namespace
{
    struct sig_functor
    {
        template< typename Args >
        struct sig
        {
            typedef void type;
        };
    };
}
BOOST_AUTO_TEST_CASE( class_with_sig_is_functor )
{
    check( sig_functor() );
}
