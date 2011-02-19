//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/is_formattable.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

BOOST_STATIC_ASSERT(( ! mock::detail::is_formattable< std::ostream, int >::value ));

namespace
{
    struct non_formattable {};
}

BOOST_STATIC_ASSERT(( ! mock::detail::is_formattable< std::ostream, non_formattable >::value ));

namespace
{
    struct formattable {};

    void format( std::ostream&, const formattable& );
}

BOOST_STATIC_ASSERT(( mock::detail::is_formattable< std::ostream, formattable >::value ));

namespace nm
{
    struct formattable {};
}

namespace mock
{
    void format( std::ostream&, nm::formattable );
}

BOOST_STATIC_ASSERT(( mock::detail::is_formattable< std::ostream, nm::formattable >::value ));

namespace
{
    struct derived_from_formattable : formattable
    {};
}

BOOST_STATIC_ASSERT(( mock::detail::is_formattable< std::ostream, derived_from_formattable >::value ));

namespace
{
    struct convertible_to_formattable
    {
        operator formattable() const;
    };
}

BOOST_STATIC_ASSERT(( mock::detail::is_formattable< std::ostream, convertible_to_formattable >::value ));
