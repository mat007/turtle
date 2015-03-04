// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2013
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "undefined.hpp"

struct undefined
{};

undefined& get_undefined()
{
    static undefined u;
    return u;
}
