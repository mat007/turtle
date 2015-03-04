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

namespace
{
//[ limitations_literal_zero_problem
    class base
    {
    public:
        virtual void method( int* i ) = 0;
    };

    MOCK_BASE_CLASS( mock_base, base )
    {
        MOCK_METHOD( method, 1 )
    };
//]
}

BOOST_AUTO_TEST_CASE( literal_zero )
{
    mock_base m;
//[ limitations_literal_zero_solution_1
    MOCK_EXPECT( m.method ).with( mock::equal< int* >( 0 ) ); // this compiles
//]
//[ limitations_literal_zero_solution_2
    MOCK_EXPECT( m.method ).with( mock::negate );
//]
#ifdef MOCK_NULLPTR
//[ limitations_literal_zero_solution_3
    MOCK_EXPECT( m.method ).with( nullptr );
//]
#endif
}
