// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/detail/is_functor.hpp>
#include <boost/test/auto_unit_test.hpp>
#ifdef BOOST_MSVC
#pragma warning( push, 0 )
#endif
#include <boost/lambda/lambda.hpp>
#include <boost/phoenix/phoenix.hpp>
#ifdef BOOST_MSVC
#pragma warning( pop )
#endif
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <functional>

namespace
{
    struct declared_but_not_defined;
    static_assert( !mock::detail::is_functor< declared_but_not_defined, int >::value, "Should not be a functor" );

    template< typename T >
    void is_functor( T )
    {
        static_assert( mock::detail::is_functor< T, int >::value, "Should be functor");
    }
    template< typename T >
    void is_not_functor( T )
    {
        static_assert( !mock::detail::is_functor< T, int >::value, "Should not be a functor" );
    }

    void f0() {}
    bool f1( int ) { return false; }
    bool f2( std::string, int ) { return false; }
}

BOOST_AUTO_TEST_CASE( data_is_not_functor )
{
    is_not_functor( 42 );
}

BOOST_AUTO_TEST_CASE( function_is_functor )
{
    is_not_functor( f0 );
    is_functor( f1 );
    is_not_functor( f2 );
}

BOOST_AUTO_TEST_CASE( function_pointer_is_functor )
{
    is_not_functor( &f0 );
    is_functor( &f1 );
    is_not_functor( &f2 );
}

BOOST_AUTO_TEST_CASE( std_ptr_fun_is_functor )
{
    is_functor( std::ptr_fun( &f1 ) );
    is_not_functor( std::ptr_fun( &f2 ) );
}

BOOST_AUTO_TEST_CASE( std_bind_first_is_functor )
{
    is_functor( std::bind1st( std::ptr_fun( &f2 ), "" ) );
}

BOOST_AUTO_TEST_CASE( boost_bind_is_functor )
{
    is_functor( boost::bind( &f0 ) );
    is_functor( boost::bind( &f1, _1 ) );
    is_functor( boost::bind( &f2, "", _1 ) );
}

BOOST_AUTO_TEST_CASE( boost_lambda_is_functor )
{
    is_functor( boost::lambda::_1 < 42 );
}

BOOST_AUTO_TEST_CASE( boost_phoenix_is_functor )
{
    is_functor( boost::phoenix::arg_names::arg1 < 42 );
    is_functor( boost::phoenix::arg_names::_1 < 42 );
}

BOOST_AUTO_TEST_CASE( boost_function_is_functor )
{
    is_functor( boost::function< void(int) >() );
}

BOOST_AUTO_TEST_CASE( std_function_is_functor )
{
    is_functor( std::function< void(int) >() );
}

#ifdef MOCK_LAMBDAS

BOOST_AUTO_TEST_CASE( cxx11_lambda_is_functor )
{
    is_not_functor( []() {} );
    is_functor( []( int ) {} );
    is_not_functor( []( const std::string&, int ) {} );
    is_not_functor( []( int, const std::string& ) {} );
}

#endif
