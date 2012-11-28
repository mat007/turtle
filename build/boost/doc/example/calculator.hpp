// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef CALCULATOR
#define CALCULATOR

class view;

//[ calculator
class calculator
{
public:
    calculator( view& v );

    void add( int a, int b ); // the result will be sent to the view 'v'
};
//]

#endif // CALCULATOR
