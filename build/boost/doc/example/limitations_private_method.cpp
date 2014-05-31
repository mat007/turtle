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
//[ limitations_private_method_problem
    class base
    {
    private:
        virtual void method() = 0;
    };
//]

//[ limitations_private_method_solution
    MOCK_BASE_CLASS( mock_base, base )
    {
        MOCK_METHOD( method, 0, void() )
    };
//]
}
