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
//[ limitations_template_base_class_method_problem
    template< typename T >
    class base
    {
    public:
        virtual ~base()
        {}

        virtual void method() = 0;
    };
//]
 
//[ limitations_template_base_class_method_solution
    template< typename T >
    MOCK_BASE_CLASS( mock_base, base< T > )
    {
        MOCK_METHOD( method, 1, void() )
    };
//]
}
