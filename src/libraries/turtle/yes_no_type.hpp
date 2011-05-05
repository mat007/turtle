//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_YES_NO_TYPE_HPP_INCLUDED
#define MOCK_YES_NO_TYPE_HPP_INCLUDED

#include <boost/static_assert.hpp>

namespace mock
{
namespace detail
{
    typedef char no_type;
    struct yes_type
    {
        char no_type[2];
    };
    BOOST_STATIC_ASSERT( sizeof( yes_type ) != sizeof( no_type ) );

    template< typename T > void operator,( yes_type, const T& );
    no_type operator,( yes_type, yes_type );
    no_type operator,( no_type, yes_type );
}
}

#endif // MOCK_YES_NO_TYPE_HPP_INCLUDED
