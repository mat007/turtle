// Copyright Alexander Grund 2026
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Test the hard failure triggered by the default policy when an unexpected call happens.
// As this causes an unrecoverable error this is a separate file that is expected exit with an error.

#include <turtle/mock.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(unexpected_call_causes_fatal_failure)
{
    MOCK_FUNCTOR(func, void());
    MOCK_EXPECT(func).never();
    // Trigger an unexpected call, test aborts with failure
    func();
}
