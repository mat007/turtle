// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ async_call_problem
namespace
{
    class base_class
    {
    public:
        virtual void method() = 0;
    };

    class my_class
    {
    public:
        explicit my_class( base_class& );

        void flush(); // repetitively calling this method will in turn call base_class::method at some point
    };
}
//]

//[ async_call_solution
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/thread.hpp>
#include <turtle/mock.hpp>

namespace
{
    template< typename F >
    void check( bool& condition, F flush, int attempts = 100, int sleep = 100 )
    {
        while( !condition && attempts > 0 )
        {
            --attempts;
            boost::this_thread::sleep( boost::posix_time::milliseconds( sleep ) );
            flush();
        }
    }

    MOCK_BASE_CLASS( mock_base_class, base_class )
    {
        MOCK_METHOD( method, 0 )
    };
    void set_bool(bool& b)
    {
        b = true;
    }
}

BOOST_AUTO_TEST_CASE( method_is_called )
{
    mock_base_class m;
    my_class c( m );
    bool done = false;
    // when method is called it will set done to true
    // Note: Boost 1.57 introduced a bug preventing usage of the lambda with clang in C++98
    // See: https://svn.boost.org/trac10/ticket/10785
#if defined(BOOST_CLANG) && (BOOST_VERSION >= 105700)
    MOCK_EXPECT( m.method ).once().calls( boost::bind(&set_bool, done) );
#else
    MOCK_EXPECT( m.method ).once().calls( boost::lambda::var( done ) = true );
#endif
    check( done, boost::bind( &my_class::flush, &c ) );                        // just wait on done, flushing from time to time
}
//]
