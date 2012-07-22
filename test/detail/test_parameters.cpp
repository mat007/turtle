// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/detail/parameters.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/mpl/assert.hpp>

BOOST_MPL_ASSERT(( boost::is_same< float, mock::detail::parameters< void( float ), 1 >::at< 0 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< float, mock::detail::parameters< void( float, int ), 2 >::at< 0 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< int, mock::detail::parameters< void( float, int ), 2 >::at< 1 >::type > ));
