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
    void wait( bool& condition, F flush, int timeout = 100, int sleep = 100 )
    {
        while( !condition && timeout > 0 )
        {
            --timeout;
            boost::this_thread::sleep( boost::posix_time::milliseconds( sleep ) );
            flush();
        }
    }
    MOCK_BASE_CLASS( mock_base_class, base_class )
    {
        MOCK_METHOD( method, 0 )
    };
}

BOOST_AUTO_TEST_CASE( method_is_called )
{
    mock_base_class mock;
    my_class c( mock );
    bool done = false;
    MOCK_EXPECT( mock.method ).once().calls( boost::lambda::var( done ) = true ); // when method is called it will set done to true
    wait( done, boost::bind( &my_class::flush, &c ) );                            // just wait on done, flushing from time to time
}
//]
