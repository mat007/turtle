// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>
#include "calculator.hpp"
#include "mock_view.hpp"

namespace simple
{
//[ simple_calculator
class calculator
{
public:
    int add( int a, int b );
};
//]

//[ simple_zero_plus_zero_is_zero
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero )
{
    calculator c;
    BOOST_CHECK_EQUAL( 0, c.add( 0, 0 ) );
}
//]
}

namespace without_mock_object
{
//[ my_view
class my_view : public view
{
public:
    my_view()
        : called( false )
    {}
    virtual void display( int result )
    {
        called = true;
        value = result;
    }
    bool called;
    int value;
};
//]

//[ zero_plus_zero_is_zero_without_mock_object
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero )
{
    my_view v;
    calculator c( v );
    c.add( 0, 0 );
    BOOST_REQUIRE( v.called );
    BOOST_CHECK_EQUAL( 0, v.value );
}
//]
}

namespace with_mock_object
{
//[ zero_plus_zero_is_zero_with_mock_object
BOOST_AUTO_TEST_CASE( zero_plus_zero_is_zero )
{
    mock_view v;
    calculator c( v );
    MOCK_EXPECT( v.display ).once().with( 0 ); // expect the 'display' method to be called once (and only once) with a parameter value equal to 0
    c.add( 0, 0 );
}
//]
}
