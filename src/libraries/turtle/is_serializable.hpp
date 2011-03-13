//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_IS_SERIALIZABLE_HPP_INCLUDED
#define MOCK_IS_SERIALIZABLE_HPP_INCLUDED

#include "yes_no_type.hpp"
#include <boost/mpl/bool.hpp>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4913 )
#endif

namespace mock
{
namespace detail
{
namespace serializable
{
    template< typename S, typename T >
    yes_type operator<<( S&, const T& );

    template< typename S, typename T >
    struct impl
    {
        static S* s;
        static T* t;
        enum { value = sizeof( yes_type(), (*s << *t), yes_type() ) == sizeof( yes_type ) };
    };
}
    template< typename S, typename T >
    struct is_serializable : boost::mpl::bool_< serializable::impl< S, T >::value >
    {};
}
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // #ifndef MOCK_IS_SERIALIZABLE_HPP_INCLUDED
