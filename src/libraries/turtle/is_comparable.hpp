//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_IS_COMPARABLE_HPP_INCLUDED
#define MOCK_IS_COMPARABLE_HPP_INCLUDED

#include "yes_no_type.hpp"
#include "sink.hpp"
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/bool.hpp>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4913 )
#endif

namespace mock
{
namespace detail
{
namespace comparable
{
    yes_type operator==( sink, sink );

    template< typename L, typename R >
    struct impl
    {
        static BOOST_DEDUCED_TYPENAME boost::remove_reference< L >::type* l;
        static BOOST_DEDUCED_TYPENAME boost::remove_reference< R >::type* r;
        enum { value = sizeof( yes_type(), *l == *r, yes_type() ) == sizeof( yes_type ) };
    };
}

    template< typename L, typename R >
    struct is_comparable : boost::mpl::bool_< comparable::impl< L, R >::value >
    {};
}
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // #ifndef MOCK_IS_COMPARABLE_HPP_INCLUDED
