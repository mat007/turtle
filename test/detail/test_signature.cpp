// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/detail/signature.hpp>
#include <boost/test/unit_test.hpp>
#include <type_traits>

namespace
{
    struct base
    {
        void method_1();
        float method_2( int ) const;
    };
    typedef base base_type;
}

BOOST_AUTO_TEST_CASE( mock_signature_generates_signature )
{
    static_assert( std::is_same< void(), MOCK_SIGNATURE(method_1) >::value, "!");
    static_assert( std::is_same< float( int ), MOCK_SIGNATURE(method_2) >::value, "!");
}
