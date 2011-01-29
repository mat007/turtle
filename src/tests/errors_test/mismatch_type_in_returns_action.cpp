//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <turtle/mock.hpp>

namespace
{
    MOCK_CLASS( cl )
    {
        MOCK_METHOD_EXT( m, 0, int(), m ) // add another test with void() and another with std::string()
    };
    void test_case()
    {
        cl c;
        MOCK_EXPECT( c, m ).returns( "42" );
    }
}
