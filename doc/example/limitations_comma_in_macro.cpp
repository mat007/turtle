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
//[ limitations_comma_in_macro_problem
    template< typename T1, typename T2 >
    struct my_base_class
    {};
//]
}

namespace limitations_comma_in_macro_solution_1
{
//[ limitations_comma_in_macro_solution_1
    typedef my_base_class< int, int > my_base_type;
 
    MOCK_BASE_CLASS( my_mock, my_base_type )
    {};
//]
}

namespace limitations_comma_in_macro_solution_2
{
//[ limitations_comma_in_macro_solution_2
    template< typename T1, typename T2 >
    MOCK_BASE_CLASS( my_mock, my_base_class< T1 BOOST_PP_COMMA() T2 > )
    {};
//]
}

namespace limitations_comma_in_macro_solution_3
{
//[ limitations_comma_in_macro_solution_3
    template< typename T1, typename T2 >
    struct my_mock : my_base_class< T1, T2 >, mock::object
    {};
//]
}
