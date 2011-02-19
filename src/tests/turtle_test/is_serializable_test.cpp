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

BOOST_STATIC_ASSERT(( mock::detail::protect::is_serializable< std::ostream, int >::value ));
BOOST_STATIC_ASSERT(( mock::detail::protect::is_serializable< std::ostream, std::string >::value ));

namespace
{
    struct declared_but_not_defined;
}

BOOST_STATIC_ASSERT(( ! mock::detail::protect::is_serializable< std::ostream, declared_but_not_defined >::value ));

namespace
{
    struct non_serializable
    {};
}

BOOST_STATIC_ASSERT(( ! mock::detail::protect::is_serializable< std::ostream, non_serializable >::value ));

namespace
{
    struct serializable
    {};

    std::ostream& operator<<( std::ostream& s, const serializable& );
}

BOOST_STATIC_ASSERT(( mock::detail::protect::is_serializable< std::ostream, serializable >::value ));

namespace
{
    template< typename T >
    struct template_serializable {};

    template< typename T >
    std::ostream& operator<<( std::ostream& s, const template_serializable< T >& );
}

BOOST_STATIC_ASSERT(( mock::detail::protect::is_serializable< std::ostream, template_serializable< int > >::value ));

namespace
{
    struct convertible_to_base
    {
        operator int() const;
    };
}

BOOST_STATIC_ASSERT(( mock::detail::protect::is_serializable< std::ostream, convertible_to_base >::value ));

namespace
{
    struct convertible_to_string
    {
        operator std::string() const;
    };
}

BOOST_STATIC_ASSERT(( ! mock::detail::protect::is_serializable< std::ostream, convertible_to_string >::value ));

namespace
{
    struct convertible_to_serializable
    {
        operator serializable() const;
    };
}

BOOST_STATIC_ASSERT(( mock::detail::protect::is_serializable< std::ostream, convertible_to_serializable >::value ));

namespace
{
    struct derived_from_serializable : serializable
    {};
}

BOOST_STATIC_ASSERT(( mock::detail::protect::is_serializable< std::ostream, derived_from_serializable >::value ));
