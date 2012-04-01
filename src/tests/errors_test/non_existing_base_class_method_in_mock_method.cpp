//
// Copyright Mathieu Champlon 2011
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://turtle.sf.net for documentation.

#include <turtle/mock.hpp>

namespace
{
    struct my_base
    {
        virtual ~my_base() {}
    };

    MOCK_BASE_CLASS( my_class, my_base )
    {
        MOCK_METHOD( my_method, 0 )
    };
}
