//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_ANY_HPP_INCLUDED
#define MOCK_ANY_HPP_INCLUDED

namespace mock
{
namespace detail
{
    struct sink
    {
        template< typename T >
        sink( const T& );
    };
}
}

#endif // #ifndef MOCK_ANY_HPP_INCLUDED
