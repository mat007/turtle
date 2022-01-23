// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <functional>
#include <string>
#include <sstream>

std::function<void()> error_handler_abort;
std::function<void(const char*, int)> error_handler_pass;
std::function<void(const std::string&, const char*, int)> error_handler_call;
std::function<void(const char* message, const std::string&, const char*, int)> error_handler_fail;

template< typename Result >
struct configurable_mock_error
{
    static Result abort()
    {
        error_handler_abort();
        return Result();
    }

    static void pass( const char* file, int line )
    {
        error_handler_pass(file, line);
    }

    template< typename Context >
    static void call( const Context& context,  const char* file, int line )
    {
        std::stringstream s;
        s << context;
        error_handler_call( s.str(), file, line );
    }

    template< typename Context >
    static void fail( const char* message, const Context& context, const char* file = "", int line = 0 )
    {
        std::stringstream s;
        s << context;
        error_handler_fail( message, s.str(), file, line );
    }
};

#define MOCK_ERROR_POLICY configurable_mock_error
#define MOCK_USE_BOOST_TEST

//[ prerequisite
#include <boost/test/unit_test.hpp>
#include <turtle/mock.hpp>
//]
#include "calculator.hpp"
#include "mock_view.hpp"

struct Fixture
{
    Fixture()
    {
        error_handler_abort = mock::error<void>::abort;
        error_handler_pass = mock::error<void>::pass;
        error_handler_call = mock::error<void>::call<std::string>;
        error_handler_fail = mock::error<void>::fail<std::string>;
    }
};

BOOST_FIXTURE_TEST_SUITE(GettingStarted, Fixture)

namespace phases
{
//[ phases
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero )
{
   mock_view v;                               // create mock objects
   calculator c( v );                         // create object under test
   MOCK_EXPECT( v.display ).once().with( 0 ); // configure mock objects
   c.add( 0, 0 );                             // exercise object under test
}                                             // verify mock objects
//]
}

namespace verify_reset
{
//[ verify_reset
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero_reset )
{
    mock_view v;
    calculator c( v );
    MOCK_EXPECT( v.display ).once().with( 0 );
    c.add( 0, 0 );
    MOCK_VERIFY( v.display );                  // verify all expectations are fulfilled for the 'display' method
    mock::verify( v );                         // verify all expectations are fulfilled for all methods of 'v'
    mock::verify();                            // verify all expectations are fulfilled for all existing mock objects
    MOCK_RESET( v.display );                   // reset all expectations for the 'display' method
    mock::reset( v );                          // reset all expectations for all methods of 'v'
    mock::reset();                             // reset all expectations for all existing mock objects
}                                              // automatically verify all expectations are fulfilled for all mock objects going out of scope
//]
}

namespace expectations
{
//[ expectations
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero_expect )
{
    mock_view v;
    calculator c( v );
    MOCK_EXPECT( v.display ).once().with( 0 ); // this call must occur once (and only once)
    MOCK_EXPECT( v.display ).with( 1 );        // this call can occur any number of times (including never)
    c.add( 0, 0 );
}
//]
}

namespace sequence
{
//[ sequence
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero_then_1_plus_0_is_1 )
{
    mock_view v;
    calculator c( v );
    mock::sequence s;
    MOCK_EXPECT( v.display ).once().with( 0 ).in( s ); // add this expectation to the sequence
    MOCK_EXPECT( v.display ).with( 1 ).in( s );        // add this expectation to the sequence after the previous call
    c.add( 0, 0 );
    c.add( 1, 0 );
}
//]
}

namespace several_sequences
{
//[ several_sequences
BOOST_AUTO_TEST_CASE( add_several_numbers_in_sequences )
{
    mock_view v;
    calculator c( v );
    mock::sequence s1, s2;
    MOCK_EXPECT( v.display ).once().with( 0 ).in( s1 );
    MOCK_EXPECT( v.display ).once().with( 1 ).in( s2 );
    MOCK_EXPECT( v.display ).with( 2 ).in( s1, s2 );    // add this expectation to both sequences after the previous calls
    c.add( 0, 0 );
    c.add( 1, 0 );
    c.add( 1, 1 );
    c.add( 2, 0 );
}
//]
}
BOOST_AUTO_TEST_SUITE_END()

namespace action
{
//[ action_view
class view
{
public:
    virtual bool display( int result ) = 0; // returns a boolean
};
//]

MOCK_BASE_CLASS( mock_view, view )
{
    MOCK_METHOD( display, 1 )
};

class calculator
{
    view& v;
public:
    calculator( view& v ): v(v) {}

    void add( int a, int b ){ v.display(a + b); }
};

struct CatchFailureFixture: Fixture
{
    static bool aborted;
    static std::string fail_msg;

    static void abort()
    {
        aborted = true;
    }
    static void fail( const std::string& message, const std::string&, const char* = "", int = 0 ){
        fail_msg = message;
    }
    CatchFailureFixture()
    {
        error_handler_abort = abort;
        error_handler_fail = fail;
    }
    void assert_failure(const std::string& required_message)
    {
        BOOST_CHECK(aborted);
        BOOST_CHECK(fail_msg.find(required_message) != std::string::npos);
    }
};
bool CatchFailureFixture::aborted = false;
std::string CatchFailureFixture::fail_msg;

struct AssertMissingAction: CatchFailureFixture{
    void teardown(){
        assert_failure("missing action");
    }
};

BOOST_FIXTURE_TEST_SUITE( MissingReturnActionSuite, AssertMissingAction)
//[ action_test
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero_with_action )
{
   mock_view v;
   calculator c( v );
   MOCK_EXPECT( v.display ).once().with( 0 ); // missing returns( true )
   c.add( 0, 0 );
}
//]
}

BOOST_AUTO_TEST_SUITE_END()
