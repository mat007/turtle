//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/is_loggable.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

BOOST_MPL_ASSERT_NOT(( mock::detail::is_loggable< std::ostream, int > ));

namespace
{
    struct non_loggable {};

    void log( std::ostream&, const non_loggable& );
}

BOOST_MPL_ASSERT_NOT(( mock::detail::is_loggable< std::ostream, non_loggable > ));

namespace
{
    struct loggable {};
}

namespace mock
{
    void log( std::ostream&, const loggable& );
}

BOOST_MPL_ASSERT(( mock::detail::is_loggable< std::ostream, loggable > ));

namespace
{
    struct derived_from_loggable : loggable
    {};
}

BOOST_MPL_ASSERT(( mock::detail::is_loggable< std::ostream, derived_from_loggable > ));

namespace
{
    struct convertible_to_loggable
    {
        operator loggable() const;
    };
}

BOOST_MPL_ASSERT(( mock::detail::is_loggable< std::ostream, convertible_to_loggable > ));
