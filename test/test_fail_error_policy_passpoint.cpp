// http://turtle.sourceforge.net
//
// Copyright Alexander Grund 2026
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <turtle/mock.hpp>
#include <boost/test/unit_test.hpp>
#include <stdexcept>

BOOST_AUTO_TEST_CASE(Setting_up_assertion_creates_passpoint)
{
    MOCK_FUNCTOR(f, void());
// Enforce the line for the CMake check
#line 42
    MOCK_EXPECT(f).once();
    throw std::runtime_error("Intentionally uncaught");
}
