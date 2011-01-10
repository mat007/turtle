//
//  Copyright Mathieu Champlon 2009
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/has_operator.hpp>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

namespace
{
    struct functor_with_valid_operator
    {
        bool operator()( float ) const
        {
            return true;
        }
    };
    BOOST_STATIC_ASSERT(( mock::detail::has_operator< functor_with_valid_operator, float >::type::value ));

    struct functor_with_valid_template_operator
    {
        template< typename T >
        bool operator()( T ) const
        {
            return true;
        }
    };
    BOOST_STATIC_ASSERT(( mock::detail::has_operator< functor_with_valid_template_operator, float >::type::value ));
}
