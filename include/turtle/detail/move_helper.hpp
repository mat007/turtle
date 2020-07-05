// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_MOVE_HELPER_HPP_INCLUDED
#define MOCK_MOVE_HELPER_HPP_INCLUDED

#include <boost/type_traits/conditional.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/add_rvalue_reference.hpp>

namespace mock
{
namespace detail
{
    template< typename T >
    struct ref_arg
    {
        typedef typename boost::conditional<
          boost::is_reference< T >::value,
          T,
          typename boost::add_rvalue_reference< T >::type >::type type;
    };
}
}

#endif  // MOCK_MOVE_HELPER_HPP_INCLUDED
