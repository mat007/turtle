// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ async_call_problem
namespace mock_test
{
    class base_class
    {
    public:
        virtual void method() = 0;
    };

    class my_class
    {
        base_class& b;
    public:
        explicit my_class( base_class& );

        void flush(); // repetitively calling this method will in turn call base_class::method at some point
    };
}
//]

namespace mock_test
{
    my_class::my_class( base_class& b): b(b){}
    void my_class::flush()
    {
        static int secret_value = 7;
        if(--secret_value == 0)
            b.method();
    }
}

//[ async_call_solution
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include <turtle/mock.hpp>

namespace mock_test
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
}

BOOST_AUTO_TEST_CASE( method_is_called )
{
    using namespace mock_test;
    mock_base_class m;
    my_class c( m );
    bool done = false;
    MOCK_EXPECT( m.method ).once().calls( [&done](){ done = true; } );
    check( done, [&c](){ c.flush(); } );                        // just wait on done, flushing from time to time
}
//]
