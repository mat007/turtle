// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MOCK_VIEW
#define MOCK_VIEW

#include <turtle/mock.hpp>
#include "view.hpp"

//[ mock_view
MOCK_BASE_CLASS( mock_view, view ) // declare a 'mock_view' class implementing 'view'
{
    MOCK_METHOD( display, 1 )      // implement the 'display' method from 'view' (taking 1 argument)
};
//]

#endif // MOCK_VIEW
