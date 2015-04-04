// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ prerequisite
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>
//]
#include "calculator.hpp"
#include "mock_view.hpp"

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
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero )
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
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero )
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
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero )
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
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero )
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
public:
    calculator( view& v );

    void add( int a, int b );
};
//[ action_test
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero )
{
   mock_view v;
   calculator c( v );
   MOCK_EXPECT( v.display ).once().with( 0 ); // missing returns( true )
   c.add( 0, 0 );
}
//]
}
