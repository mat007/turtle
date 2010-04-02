//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_ROOT_HPP_INCLUDED
#define MOCK_ROOT_HPP_INCLUDED

#include "config.hpp"
#include "node.hpp"

namespace mock
{
    inline node& root()
    {
        static node r;
        return r;
    }

    inline bool verify()
    {
        return root().verify();
    }
    inline void reset()
    {
        root().reset();
    }
}

#endif // #ifndef MOCK_ROOT_HPP_INCLUDED
