// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2009
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/args.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/mpl/assert.hpp>

BOOST_MPL_ASSERT(( boost::is_same< float, mock::detail::arg< void( float ), 1, 1 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< float, mock::detail::arg< void( float, int ), 1, 2 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< int, mock::detail::arg< void( float, int ), 2, 2 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< mock::detail::invalid_type, mock::detail::arg< void( float ), 1, 2 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< mock::detail::invalid_type, mock::detail::arg< void( float ), 2, 2 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< mock::detail::invalid_type, mock::detail::arg< void( float, int ), 1, 1 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< mock::detail::invalid_type, mock::detail::arg< void( float, int ), 1, 3 >::type > ));
BOOST_MPL_ASSERT(( boost::is_same< mock::detail::invalid_type, mock::detail::arg< void( float, int ), 2, 3 >::type > ));
