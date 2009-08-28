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
        BOOST_STATIC_ASSERT(( mock::detail::is_functor< T >::value ));
    }

    void f0 () {}
    bool f1( int ) { return false; }

    struct s
    {
        typedef void result_type;
    };
}

BOOST_AUTO_TEST_CASE( function_is_functor )
{
    check( f0 );
    check( f1 );
}

BOOST_AUTO_TEST_CASE( function_pointer_is_functor )
{
    check( &f0 );
    check( &f1 );
}

BOOST_AUTO_TEST_CASE( boost_bind_is_functor )
{
    check( boost::bind( &f0 ) );
}

BOOST_AUTO_TEST_CASE( boost_function_is_functor )
{
    check( boost::function< void() >() );
}

BOOST_AUTO_TEST_CASE( class_with_result_type_is_functor )
{
    check( s() );
}
