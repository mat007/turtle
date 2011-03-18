//
//  Copyright Mathieu Champlon 2009
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/is_container.hpp>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

BOOST_MPL_ASSERT_NOT(( mock::detail::is_container< int > ));

BOOST_MPL_ASSERT(( mock::detail::is_container< std::vector< int > > ));
BOOST_MPL_ASSERT(( mock::detail::is_container< std::deque< int > > ));
BOOST_MPL_ASSERT(( mock::detail::is_container< std::list< int > > ));
BOOST_MPL_ASSERT(( mock::detail::is_container< std::map< int, float > > ));
BOOST_MPL_ASSERT(( mock::detail::is_container< std::multimap< int, float > > ));
BOOST_MPL_ASSERT(( mock::detail::is_container< std::multiset< int > > ));
