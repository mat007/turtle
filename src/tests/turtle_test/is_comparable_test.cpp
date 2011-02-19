//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/is_comparable.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

namespace
{
    struct declared_but_not_defined;

    struct non_comparable
    {};

    struct comparable
    {};
    bool operator==( const comparable&, int );
    bool operator==( int, const comparable& );

    struct derived_from_comparable : comparable
    {};

    struct convertible_to_comparable
    {
        operator comparable() const;
    };

    template< typename T >
    struct template_comparable
    {};
    template< typename T >
    bool operator==( const template_comparable< T >&, int );
    template< typename T >
    bool operator==( int, const template_comparable< T >& );

    struct comparable_to_everything
    {
        template< typename T >
        bool operator==( const T& ) const;
    };
}

BOOST_MPL_ASSERT_NOT(( mock::detail::is_comparable< int, declared_but_not_defined > ));
BOOST_MPL_ASSERT_NOT(( mock::detail::is_comparable< declared_but_not_defined, int > ));

BOOST_MPL_ASSERT_NOT(( mock::detail::is_comparable< int, non_comparable > ));
BOOST_MPL_ASSERT_NOT(( mock::detail::is_comparable< non_comparable, int > ));

BOOST_MPL_ASSERT(( mock::detail::is_comparable< int, comparable > ));
BOOST_MPL_ASSERT(( mock::detail::is_comparable< int, comparable& > ));
BOOST_MPL_ASSERT(( mock::detail::is_comparable< int, const comparable& > ));
BOOST_MPL_ASSERT(( mock::detail::is_comparable< comparable, int > ));
BOOST_MPL_ASSERT(( mock::detail::is_comparable< const comparable&, int > ));

BOOST_MPL_ASSERT(( mock::detail::is_comparable< int, template_comparable< int > > ));
BOOST_MPL_ASSERT(( mock::detail::is_comparable< template_comparable< int >, int > ));

BOOST_MPL_ASSERT(( mock::detail::is_comparable< int, derived_from_comparable > ));
BOOST_MPL_ASSERT(( mock::detail::is_comparable< derived_from_comparable, int > ));

BOOST_MPL_ASSERT(( mock::detail::is_comparable< int, convertible_to_comparable > ));
BOOST_MPL_ASSERT(( mock::detail::is_comparable< convertible_to_comparable, int > ));

BOOST_MPL_ASSERT_NOT(( mock::detail::is_comparable< int, comparable_to_everything > ));
BOOST_MPL_ASSERT(( mock::detail::is_comparable< comparable_to_everything, int > ));
