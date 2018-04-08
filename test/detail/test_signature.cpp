// http://turtle.sourceforge.net
//
// Copyright Mathieu Champlon 2012
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/detail/signature.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/mpl/assert.hpp>

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
    BOOST_MPL_ASSERT((
        boost::is_same< void(), MOCK_SIGNATURE(method_1) > ));
    BOOST_MPL_ASSERT((
        boost::is_same< float( int ), MOCK_SIGNATURE(method_2) > ));
}
