// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ invoke_functor_problem
#include <boost/function.hpp>

namespace
{
    class base_class
    {
    public:
        virtual void method( const boost::function< void( int ) >& functor ) = 0;
    };

    void function( base_class& ); // the function will call 'method' with a functor to be applied
}
//]

//[ invoke_functor_solution
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/bind/apply.hpp>
#include <turtle/mock.hpp>

namespace
{
    MOCK_BASE_CLASS( mock_class, base_class )
    {
        MOCK_METHOD( method, 1 )
    };
}

BOOST_AUTO_TEST_CASE( how_to_invoke_a_functor_passed_as_parameter_of_a_mock_method )
{
    mock_class mock;
    MOCK_EXPECT( mock.method ).calls( boost::bind( boost::apply< void >(), _1, 42 ) ); // whenever 'method' is called, invoke the functor with 42
    function( mock );
}
//]
