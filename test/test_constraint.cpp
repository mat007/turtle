// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2013
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/constraint.hpp>
#include <boost/test/auto_unit_test.hpp>

#ifdef MOCK_VARIADIC_MACROS

namespace
{
    MOCK_CONSTRAINT( constraint_0, actual == 0 )
    MOCK_CONSTRAINT( constraint_1, expected, actual == expected )
    MOCK_CONSTRAINT( constraint_2, expected_0, expected_1, actual == expected_0 || actual == expected_1 )
}

BOOST_AUTO_TEST_CASE( mock_constraint_is_supported_by_compilers_with_variadic_macros )
{
    BOOST_CHECK( constraint_0.c_( 0 ) );
    BOOST_CHECK( constraint_1( 0 ).c_( 0 ) );
    BOOST_CHECK( constraint_2( 0, 0 ).c_( 0 ) );
}

#endif // MOCK_VARIADIC_MACROS

namespace
{
    MOCK_CONSTRAINT_EXT( constraint_0_ext, 0,, actual == 0 )
    MOCK_CONSTRAINT_EXT( constraint_1_ext, 1, ( expected ), actual == expected )
    MOCK_CONSTRAINT_EXT( constraint_2_ext, 2, ( expected_0, expected_1 ), actual == expected_0 || actual == expected_1 )
}

BOOST_AUTO_TEST_CASE( mock_constraint_ext_is_supported_by_all_compilers )
{
    BOOST_CHECK( constraint_0_ext.c_( 0 ) );
    BOOST_CHECK( constraint_1_ext( 0 ).c_( 0 ) );
    BOOST_CHECK( constraint_2_ext( 0, 0 ).c_( 0 ) );
}
