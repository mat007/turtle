//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_IS_CONTAINER_HPP_INCLUDED
#define MOCK_IS_CONTAINER_HPP_INCLUDED

#include <boost/mpl/has_xxx.hpp>

namespace mock
{
namespace detail
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF( const_iterator )

    template< typename T >
    struct is_container : has_const_iterator< T >
    {};
}
}

#endif // #ifndef MOCK_IS_CONTAINER_HPP_INCLUDED
