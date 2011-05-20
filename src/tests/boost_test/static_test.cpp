//
//  Copyright Mathieu Champlon 2011
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>
#include <iostream>
#include <turtle/mock.hpp>

namespace
{
    struct fixture
    {
        fixture()
        {
            std::cout << __FUNCTION__ << std::endl;
            //BOOST_FAIL( "oups" );
        }
        ~fixture()
        {
            std::cout << __FUNCTION__ << std::endl;
            //BOOST_FAIL( "oups" );
        }
    };
    BOOST_GLOBAL_FIXTURE( fixture );

    struct s
    {
        MOCK_METHOD_EXT( f, 0, void(), f )
    };
    s s1;
}

BOOST_AUTO_TEST_CASE( some_test )
{
    MOCK_EXPECT( s1, f ).once();
}

BOOST_AUTO_TEST_CASE( some_other_test )
{
}
