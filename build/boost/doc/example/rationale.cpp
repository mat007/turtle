// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/mock.hpp>
#include "calculator.hpp"
#include "mock_view.hpp"

//[ overflow_throws
BOOST_AUTO_TEST_CASE( overflow_throws )
{
    mock_view v;
    calculator c( v );
    BOOST_CHECK_THROW( c.add( (std::numeric_limits< int >::max)(), 1 ), std::exception );
}
//]
