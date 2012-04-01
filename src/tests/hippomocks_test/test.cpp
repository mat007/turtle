//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://turtle.sf.net for documentation.

#define BOOST_AUTO_TEST_MAIN
#include "hippomocks.h"
#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

namespace
{
    struct s
    {
        virtual void method( int ) {}
        virtual s& operator=( int ) { return *this; }
    };
    bool check( int )
    {
        return true;
    }
}

BOOST_AUTO_TEST_CASE( hmm )
{
    MockRepository mocks;
    s* m = mocks.Mock< s >();
    mocks.ExpectCall( m, s::method ).Match( check );
    mocks.ExpectCallOverload( m, (s&(s::*)(int))&s::operator= );
    m->method( 7 );
}

//namespace
//{
//   void free_function() { std::cout << "ok" << std::endl; }
//}
//
//BOOST_AUTO_TEST_CASE( hmm2 )
//{
//   MockRepository mocks;
//   mocks.ExpectCallFunc( free_function );
//   free_function();
//}
