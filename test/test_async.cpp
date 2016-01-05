// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "mock_error.hpp"
#include <turtle/mock.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/lexical_cast.hpp>

#if defined(MOCK_HDR_MUTEX)
#include <thread>
#else
#include <boost/thread.hpp>
#endif

namespace
{
    MOCK_CLASS( mock_class )
    {
        MOCK_METHOD_EXT( my_method, 1, void( const std::string& ), my_tag )
    };
}

BOOST_FIXTURE_TEST_CASE( mock_object_asynchonous_call_expectation, mock_error_fixture )
{
#if defined(MOCK_THREAD_SAFE)
    const mock_class m;
    MOCK_EXPECT( m.my_tag ).async(MOCK_THREAD_NAMESPACE::chrono::milliseconds(50)).once().with( "some parameter" );
    MOCK_THREAD_NAMESPACE::thread context([&](){ 
        MOCK_THREAD_NAMESPACE::this_thread::sleep_for(MOCK_THREAD_NAMESPACE::chrono::milliseconds(10));
        m.my_method("some parameter"); 
    });
    mock::verify();
    CHECK_CALLS( 1 );
    context.join();
#endif

}

BOOST_AUTO_TEST_CASE( mock_object_asynchonous_call_expectation_fails )
{
#if defined(MOCK_THREAD_SAFE)
    const mock_class m;
    bool unexpected_call_received = false;
    MOCK_EXPECT( m.my_tag ).async(MOCK_THREAD_NAMESPACE::chrono::milliseconds(50)).once().with( "some parameter" );
    MOCK_THREAD_NAMESPACE::thread context([&](){ 
        MOCK_THREAD_NAMESPACE::this_thread::sleep_for(MOCK_THREAD_NAMESPACE::chrono::milliseconds(100));
        try{
            m.my_method("some parameter");
        }
        catch (std::exception &)
        {
            unexpected_call_received = true;
        }
    });
    CHECK_ERROR(
        BOOST_CHECK(!mock::verify()),
        "verification failed", 0, "m.mock_class::my_tag\n. once().with( \"some parameter\" )");
    context.join();
    BOOST_CHECK(unexpected_call_received == true);
#endif
}