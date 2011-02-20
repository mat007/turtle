//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/is_serializable.hpp>
#include <boost/static_assert.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

BOOST_MPL_ASSERT(( mock::detail::is_serializable< std::ostream, int > ));
BOOST_MPL_ASSERT(( mock::detail::is_serializable< std::ostream, std::string > ));

namespace
{
    struct declared_but_not_defined;
}

BOOST_MPL_ASSERT_NOT(( mock::detail::is_serializable< std::ostream, declared_but_not_defined > ));

namespace
{
    struct non_serializable
    {};
}

BOOST_MPL_ASSERT_NOT(( mock::detail::is_serializable< std::ostream, non_serializable > ));

namespace
{
    struct serializable
    {};

    std::ostream& operator<<( std::ostream& s, const serializable& );
}

BOOST_MPL_ASSERT(( mock::detail::is_serializable< std::ostream, serializable > ));

namespace
{
    template< typename T >
    struct template_serializable {};

    template< typename T >
    std::ostream& operator<<( std::ostream& s, const template_serializable< T >& );
}

BOOST_MPL_ASSERT(( mock::detail::is_serializable< std::ostream, template_serializable< int > > ));

namespace
{
    struct convertible_to_base
    {
        operator int() const;
    };
}

BOOST_MPL_ASSERT(( mock::detail::is_serializable< std::ostream, convertible_to_base > ));

namespace
{
    struct convertible_to_string
    {
        operator std::string() const;
    };
}

BOOST_MPL_ASSERT_NOT(( mock::detail::is_serializable< std::ostream, convertible_to_string > ));

namespace
{
    struct convertible_to_serializable
    {
        operator serializable() const;
    };
}

BOOST_MPL_ASSERT(( mock::detail::is_serializable< std::ostream, convertible_to_serializable > ));

namespace
{
    struct derived_from_serializable : serializable
    {};
}

BOOST_MPL_ASSERT(( mock::detail::is_serializable< std::ostream, derived_from_serializable > ));
